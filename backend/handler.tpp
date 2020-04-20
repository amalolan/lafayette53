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
    try {
        //check for specific urls
        // URL: /get-data/museum-list/
        if (paths[0] == "get-data") {
            if(paths[1] == "museum-list"){
                returnMuseumList(message);
                return;
            }
            // URL: /get-data/museum-list/id
            else if(paths[1] == "museum" && paths.size() == 3){
                ucout << "museum\n";
                std::string musId = paths[2];
                returnMuseumById(message,std::stoi(musId));
            }
            // URL: /get-data/user/id
            else if(paths[1] == "user" && paths.size() == 3){
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
    } catch (std::exception &e) {
        message.reply(status_codes::InternalError, Util::getFailureJsonStr("GET ERROR " + std::string(e.what())));
        ucout << e.what() << "\n";
    }

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
            } catch(std::exception &e){
                ucout << e.what() << "\n";
            }
        });
    })
            .then([=](pplx::task<void> t)
    {
        try {
            t.get();
        } catch (std::exception &e) {
            message.reply(status_codes::InternalError, Util::getFailureJsonStr("FRONTEND FILES ERROR: " + std::string(e.what())));
        }
    });
    return;
}

template < class T >
void Handler<T>::returnMuseumList(http_request message){

    message.reply(status_codes::OK,T::getMuseumListJSON())
            .then([=](pplx::task<void> t){

            t.get();

    });
    return;
};

template < class T >
void Handler<T>::returnMuseumById(http_request message,int musId){

    message.reply(status_codes::OK,T::getMuseumInfoJSON(musId))
            .then([=](pplx::task<void> t){
            t.get();
    });
    return;
}

template < class T >
void Handler<T>::returnUserById(http_request message,int usrId){
    message.reply(status_codes::OK,T::getUserInfoJSON(usrId))
            .then([=](pplx::task<void> t){
            t.get();
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
    try {
        if(message.relative_uri().to_string() ==  "/get-data/museum-list"){
            addMuseum(message);
        }
        // URL: /get-data/user
        else if(message.relative_uri().to_string() == "/get-data/user"){
            registerUser(message);
        }
        else {
            message.extract_string(false).then([](utility::string_t s){
                ucout << s << std::endl;
            });
            message.reply(status_codes::NotFound,U("check the POST url and try again"));
        }
    } catch(std::exception &e){
        message.reply(status_codes::InternalError, Util::getFailureJsonStr("POST ERROR " + std::string(e.what())));
        ucout << e.what() << "\n";
    }
    return;

};

template < class T >
void Handler<T>::addMuseum(http_request message){
    message.extract_string(false).then([=](utility::string_t s){
        //ucout << s << std::endl;
        Museum *m = Util::parseMuseumJsonStr(s);
        std::string username = m->getUsername();
        std::string password = m->getUserPass();
        //std::cout << password << '\n';

        std::string dataPass = T::getPasswordHash(username);
        //ucout << dataPass << '\n';
        //ucout << username << '\n';
        bool t =false;
        if (password == dataPass) {
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
        }
        return;

    });
};


template < class T>
void Handler<T>::registerUser(http_request message){
    message.extract_string(false).then([=](utility::string_t s){
        ucout << s << std::endl;
        User *u = Util::parseUserJsonStr(s);
        if(T::saveUserToDB(*u)){
            ucout << "success add user\n";
            message.reply(status_codes::OK, Util::getSuccessJsonStr("user registered"));
        } else{
            ucout << "failed to add user\n";
            message.reply(status_codes::Conflict, Util::getFailureJsonStr("user could not be registered"));
        }
    });
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
    try {
        if(paths[0] == "get-data" && paths[1] == "login" && paths.size() == 3){
            validateLogin(message,paths[2]);
        }
        else if(paths[0] == "get-data" && paths[1] == "user" && paths.size() == 3){
            getUserProfile(message,paths[2]);
            return;
        }
        else {
            message.reply(status_codes::NotFound,U("Check the url again"));
        }
    } catch (std::exception &e) {
        ucout << e.what() << "\n";
        message.reply(status_codes::InternalError, Util::getFailureJsonStr("PUT ERROR " + std::string(e.what())));
    }
    return;
};

template < class T >
void Handler<T>::validateLogin(http_request message,std::string username){
    message.extract_string(false).then([=](utility::string_t s){
        std::string password = Util::parsePassword(s);
        std::string dataPass = T::getPasswordHash(username);
        if(password == dataPass) {
            ucout << "login successful\n";
            message.reply(status_codes::OK, Util::getSuccessJsonStr("login successful"));
        } else{
            ucout << "login failed\n";
            message.reply(status_codes::Unauthorized, Util::getFailureJsonStr("login failed"));
        }

    });
    return;
};

template < class T >
void Handler<T>::getUserProfile(http_request message,std::string username){
    message.extract_string(false).then([=](utility::string_t s){
        std::string password = Util::parsePassword(s);
        std::string dataPass = T::getPasswordHash(username);
        if(password == dataPass){
            ucout << "authorized\n";
            User u = T::getUserObject(username);
            std::string userJSON = u.getJSON();
            ucout << userJSON << 'n';
            message.reply(status_codes::OK, userJSON);
        } else{
            ucout << "unauthorized\n";
            message.reply(status_codes::Unauthorized, Util::getFailureJsonStr("login failed"));
        }
    });
    return;
}

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
