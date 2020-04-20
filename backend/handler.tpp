#include "handler.h"

template < class T >
Handler<T>::Handler(utility::string_t url): m_listener(url)
{
    m_listener.support(methods::GET, std::bind(&Handler<T>::handle_get, this, std::placeholders::_1));
    m_listener.support(methods::PUT, std::bind(&Handler<T>::handle_put, this, std::placeholders::_1));
    m_listener.support(methods::POST, std::bind(&Handler<T>::handle_post, this, std::placeholders::_1));
    m_listener.support(methods::DEL, std::bind(&Handler<T>::handle_delete, this, std::placeholders::_1));

}


template < class T >
void Handler<T>::handle_error(pplx::task<void>& t)
{
    try
    {
        t.get();
    }
    catch(...)
    {
        // Ignore the error, Log it if a logger is available
    }
}


//
// Get Request
//
template < class T >
void Handler<T>::handle_get(http_request message)
{
    auto paths = web::http::uri::split_path(web::http::uri::decode(message.relative_uri().path()));

    ucout << "relative uri GET " << message.relative_uri().to_string() << "\n";

    // URL: /
    //check for frontend files.
    QDirIterator dirIt((std::string(CODE_BASE_DIRECTORY)+"frontend/").c_str(), QDirIterator::NoIteratorFlags);
    if(message.relative_uri().to_string().compare("/") == 0) {
        returnFrontendFile(message);
        return;
    }
    while(dirIt.hasNext()){
        std::string s = "/" + dirIt.fileName().toStdString();

        if(s.compare(message.relative_uri().to_string()) == 0) {
            returnFrontendFile(message);
            return ;
        }
        dirIt.next();
    }
    //frontend check ends here

    //check for specific urls
    // URL: /get-data/museum-list/
    if (paths[0].compare("get-data") == 0) {
        if(paths[1].compare("museum-list") == 0){
            returnMuseumList(message);
            return;
        }
        // URL: /get-data/museum-list/id
        else if(paths[1].compare("museum") == 0  && paths.size() == 3){
            ucout << "museum\n";
            std::string musId = paths[2];
            returnMuseumById(message,std::stoi(musId));
        }
        // URL: /get-data/user/id
        else if(paths[1].compare("user") == 0 && paths.size() == 3){
            ucout << "user\n";
            std::string usrId = paths[2];
            returnUserById(message,std::stoi(usrId));
        }
    }
    else {
        ucout << "wildcard caught\n";
        returnFrontendFile(message);
        return;
    }
    message.reply(status_codes::NotFound,U("Check the GET url and try again"));
    return;

};

template < class T >
void Handler<T>::returnFrontendFile(http_request message){
    std::string mime = "text/html";

    std::string base_path = (std::string(CODE_BASE_DIRECTORY) + "frontend/");
    bool getIndex = false;
    if(message.relative_uri().to_string().find(".html") != std::string::npos){
        mime = "text/html";
    } else if(message.relative_uri().to_string().find(".js") != std::string::npos){
        mime = "text/javascript";
    } else if(message.relative_uri().to_string().find(".css") != std::string::npos){
        mime = "text/css";
    } else {
        getIndex = true;
    }
    std::string path;
    if (getIndex) {
        path = base_path + "/index.html";
    } else {
        path = base_path + message.relative_uri().to_string();
    }

    concurrency::streams::fstream::open_istream(U(path),std::ios::in)
            .then([=](concurrency::streams::istream is)
    {
        message.reply(status_codes::OK, is, U(mime))
                .then([](pplx::task<void> t)
        {
            try{
                t.get();
            } catch(LafException &e){
                ucout << e.what() << "\n";
            }
        });
    })
            .then([=](pplx::task<void> t)
    {
        try {
            t.get();
        } catch (...) {
            message.reply(status_codes::InternalError);
        }
    });
    return;
}

template < class T >
void Handler<T>::returnMuseumList(http_request message){

    message.reply(status_codes::OK,T::getMuseumListJSON())
            .then([=](pplx::task<void> t){
        try{
            t.get();
        }catch(LafException &e){
            ucout << e.what() << "\n";
            message.reply(status_codes::InternalError, Util::getFailureJsonStr(e.what()));
        }
    });
    return;
};

template < class T >
void Handler<T>::returnMuseumById(http_request message,int musId){

    message.reply(status_codes::OK,T::getMuseumInfoJSON(musId))
            .then([=](pplx::task<void> t){
        try {
            t.get();
        } catch (LafException &e) {
            ucout << e.what() << "\n";
            message.reply(status_codes::InternalError, Util::getFailureJsonStr(e.what()));
        }
    });
    return;
}

template < class T >
void Handler<T>::returnUserById(http_request message,int usrId){
    message.reply(status_codes::OK,T::getUserInfoJSON(usrId))
            .then([=](pplx::task<void> t){
        try {
            t.get();
        } catch (LafException &e) {
            ucout << e.what() << "\n";
            message.reply(status_codes::InternalError, Util::getFailureJsonStr(e.what()));
        }
    });
    return;
}

//
// A POST request
//
template < class T >
void Handler<T>::handle_post(http_request message)
{
    //ucout <<  message.to_string() << std::endl;
    // URL: /get-data/museum-list
    ucout << "relative uri POST " << message.relative_uri().to_string() << "\n";
    if(message.relative_uri().to_string().compare("/get-data/museum-list") == 0){
        addMuseum(message);
        return ;
    }
    // URL: /get-data/user
    else if(message.relative_uri().to_string().compare("/get-data/user") == 0){
        addUser(message);
        return ;
    }
    else {
        message.extract_string(false).then([](utility::string_t s){
            ucout << s << std::endl;
        });
        message.reply(status_codes::NotFound,U("check the POST url and try again"));
        return ;
    }
};

template < class T >
void Handler<T>::addMuseum(http_request message){
    message.extract_string(false).then([=](utility::string_t s){
        try {
            //ucout << s << std::endl;
            Museum *m = Util::parseMuseumJsonStr(s);
            std::string username = m->getUsername();
            std::string password = m->getUserPass();
            //std::cout << password << '\n';

            std::string dataPass = T::getPasswordHash(username);
            //ucout << dataPass << '\n';
            //ucout << username << '\n';
            bool t =false;
            if (password.compare(dataPass) == 0) {
                ucout << "password Correct\n";
                t = T::saveMuseumToDB(*m);
                if(t) {
                    ucout << "success\n";
                    message.reply(status_codes::OK, Util::getSuccessJsonStr("Museum created"));
                    delete m;
                    return;
                } else {
                    ucout << "error\n";
                    message.reply(status_codes::Conflict,Util::getFailureJsonStr("Museum creation failed"));
                    delete m;
                    return;
                }
            } else {
                ucout << "login info incorrect\n";
                message.reply(status_codes::Unauthorized, Util::getFailureJsonStr("password incorrect"));
                delete m;
                return;
            }

        } catch(std::exception &e){
            message.reply(status_codes::InternalError, Util::getFailureJsonStr(e.what()));
            ucout << e.what() << "\n";
            return;
        }
    });
};


template < class T>
void Handler<T>::addUser(http_request message){
    message.extract_string(false).then([=](utility::string_t s){
        try {
            ucout << s << std::endl;
            User *u = Util::parseUserJsonStr(s);
            if(T::saveUserToDB(*u)){
                ucout << "success add user\n";
                message.reply(status_codes::OK, Util::getSuccessJsonStr("user registered"));
            } else{
                ucout << "failed to add user\n";
                message.reply(status_codes::Conflict, Util::getFailureJsonStr("user could not be registered"));
            }

        } catch (std::exception &e) {
            ucout << e.what() << "\n";
            message.reply(status_codes::InternalError, Util::getFailureJsonStr(e.what()));
        }
    });
    return;
};










//
// A DELETE request
//
template < class T >
void Handler<T>::handle_delete(http_request message)
{
    ucout <<  message.to_string() << std::endl;

    std::string rep = U("WRITE YOUR DELETE OPERATION");
    message.reply(status_codes::OK,rep);
    return;
};


//
// A PUT request
//
template < class T >
void Handler<T>::handle_put(http_request message)
{
    ucout << "PUT " << message.relative_uri().to_string() << "\n";
    auto paths = web::http::uri::split_path(web::http::uri::decode(message.relative_uri().path()));
    if(paths[0].compare("get-data") == 0 && paths[1].compare("login") == 0 && paths.size() == 3){
        checkLogin(message,paths[2]);
        return;
    }
    if(paths[0].compare("get-data") == 0 && paths[1].compare("user") == 0 && paths.size() == 3){
        getUserProfile(message,paths[2]);
        return;
    }
    message.reply(status_codes::NotFound,U("Check the url again"));
    return;
};

template < class T >
void Handler<T>::checkLogin(http_request message,std::string username){
    message.extract_string(false).then([=](utility::string_t s){
        try{
            std::string password = Util::parsePassword(s);
            std::string dataPass = T::getPasswordHash(username);
            if(password.compare(dataPass) == 0) {
                ucout << "login successful\n";
                message.reply(status_codes::OK, Util::getSuccessJsonStr("login successful"));
            } else{
                ucout << "login failed\n";
                message.reply(status_codes::Unauthorized, Util::getFailureJsonStr("login failed"));
            }
        } catch(LafException &e){
            ucout << e.what() << "\n";
            message.reply(status_codes::InternalError, Util::getFailureJsonStr(e.what()));

        }
    });
    return;
};

template < class T >
void Handler<T>::getUserProfile(http_request message,std::string username){
    message.extract_string(false).then([=](utility::string_t s){
        try{
            std::string password = Util::parsePassword(s);
            std::string dataPass = T::getPasswordHash(username);
            if(password.compare(dataPass) == 0){
                ucout << "authorized\n";
                User u = T::getUserObject(username);
                std::string userJSON = u.getJSON();
                ucout << userJSON << 'n';
                message.reply(status_codes::OK, userJSON);
            } else{
                ucout << "unauthorized\n";
                message.reply(status_codes::Unauthorized, Util::getFailureJsonStr("login failed"));
            }
        } catch(ModelException &e){
            ucout << e.what() << "\n";
            message.reply(status_codes::InternalError, Util::getFailureJsonStr(e.what()));
        }
    });
    return;
}
