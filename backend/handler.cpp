#include "handler.h"

Handler::Handler()
{

}

Handler::Handler(utility::string_t url):m_listener(url)
{
    m_listener.support(methods::GET, std::bind(&Handler::handle_get, this, std::placeholders::_1));
    m_listener.support(methods::PUT, std::bind(&Handler::handle_put, this, std::placeholders::_1));
    m_listener.support(methods::POST, std::bind(&Handler::handle_post, this, std::placeholders::_1));
    m_listener.support(methods::DEL, std::bind(&Handler::handle_delete, this, std::placeholders::_1));

}

Handler::~Handler()
{

}

void Handler::handle_error(pplx::task<void>& t)
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
void Handler::handle_get(http_request message)
{
    auto paths = http::uri::split_path(http::uri::decode(message.relative_uri().path()));

    ucout << "relative uri GET " << message.relative_uri().to_string() << "\n";

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
    // /get-data/museum-list/
    if (paths[0].compare("get-data") == 0) {
        if(paths[1].compare("museum-list") == 0){
            returnMuseumList(message);
            return;
        }
        // /get-data/museum-list/id
        else if(paths[1].compare("museum") == 0  && paths.size() == 3){
            ucout << "museum\n";
            std::string musId = paths[2];
            returnMuseumById(message,std::stoi(musId));
        }
        // /get-data/user/id
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
    message.reply(status_codes::NotFound,U("Check the url and try again"));
    // message.reply(status_codes::NotFound,U("This url not found"));
    return;

};

void Handler::returnFrontendFile(http_request message){
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


void Handler::returnMuseumList(http_request message){

    message.reply(status_codes::OK,ModelClass::getMuseumListJSON())
            .then([=](pplx::task<void> t){
        try{
            t.get();
        }catch(LafException &e){
            ucout << e.what() << "\n";
            message.reply(status_codes::InternalError, e.what());
        }
    });
    return;
};

void Handler::returnMuseumById(http_request message,int musId){

    message.reply(status_codes::OK,ModelClass::getMuseumInfoJSON(musId))
            .then([=](pplx::task<void> t){
        try {
            t.get();
        } catch (LafException &e) {
            ucout << e.what() << "\n";
            message.reply(status_codes::InternalError, e.what());
        }
    });
    return;
}

void Handler::returnUserById(http_request message,int usrId){
    message.reply(status_codes::OK,ModelClass::getUserInfoJSON(usrId))
            .then([=](pplx::task<void> t){
            try {
                t.get();
            } catch (LafException &e) {
                ucout << e.what() << "\n";
                message.reply(status_codes::InternalError);
        }
    });
    return;
}

//
// A POST request
//
void Handler::handle_post(http_request message)
{
    //ucout <<  message.to_string() << std::endl;
    ucout << "relative uri POST " << message.relative_uri().to_string() << "\n";
    if(message.relative_uri().to_string().compare("/get-data/museum-list") == 0){
        addMuseum(message);
        return ;
    }
    if(message.relative_uri().to_string().compare("/get-data/user") == 0){
        addUser(message);
        return ;
    }
    message.extract_string(false).then([](utility::string_t s){
        ucout << s << std::endl;
    });
    message.reply(status_codes::NotFound,U("check the url and try again"));
    return ;
};

void Handler::addMuseum(http_request message){
    message.extract_string(false).then([=](utility::string_t s){
        try {
            //ucout << s << std::endl;
            Museum *m = util::parseMuseumJSON(s);
            std::string username = m->getUsername();
            std::string password = m->getUserPass();
            //std::cout << password << '\n';

            std::string dataPass = ModelClass::getPasswordHash(username);
            //ucout << dataPass << '\n';
            //ucout << username << '\n';
            bool t =false;
            if (password.compare(dataPass) == 0)
            {
                ucout << "password Correct\n";
                t = ModelClass::saveMuseumToDB(*m);
                if(t) {
                    ucout << "success\n";
                    std::string reply = util::successJSON("Museum created");
                    message.reply(status_codes::OK, U(reply));
                    delete m;
                    return;
                } else {
                    ucout << "error\n";
                    std::string reply = util::failureJSON("Museum creation failed");
                    message.reply(status_codes::Conflict,U(reply));
                    delete m;
                    return;
                }
            } else
            {
                ucout << "login info incorrect\n";
                std::string reply = util::failureJSON("password incorrect");
                message.reply(status_codes::Unauthorized, reply);
                delete m;
                return;
            }

        } catch(LafException &e){
            std::string reply  = util::failureJSON(e.what());
            message.reply(status_codes::InternalError, U(reply));
            ucout << e.what() << "\n";
            return;
        }

    });
    return;
};


void Handler::addUser(http_request message){
    message.extract_string(false).then([=](utility::string_t s){
        try {
            ucout << s << std::endl;
            User *u = util::parseUserJSON(s);
            if(ModelClass::saveUserToDB(*u)){
                ucout << "success add user\n";
                std::string reply = util::successJSON("user registered");
                message.reply(status_codes::OK, reply);
            } else{
                ucout << "failed to add user\n";
                std::string reply = util::failureJSON("user could not be registered");
                message.reply(status_codes::Conflict, reply);
            }

        } catch (LafException &e) {
            ucout << e.what() << "\n";
            std::string reply = util::failureJSON(e.what());
            message.reply(status_codes::InternalError, reply);
        }
    });
    return;
};










//
// A DELETE request
//
void Handler::handle_delete(http_request message)
{
    ucout <<  message.to_string() << std::endl;

    std::string rep = U("WRITE YOUR OWN DELETE OPERATION");
    message.reply(status_codes::OK,rep);
    return;
};


//
// A PUT request
//
void Handler::handle_put(http_request message)
{
     ucout << "PUT " << message.relative_uri().to_string() << "\n";
     auto paths = http::uri::split_path(http::uri::decode(message.relative_uri().path()));
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

void Handler::checkLogin(http_request message,std::string username){
    message.extract_string(false).then([=](utility::string_t s){
        try{
            std::string password = util::parsePassword(s);
            std::string dataPass = ModelClass::getPasswordHash(username);
            if(password.compare(dataPass) == 0) {
                ucout << "login successful\n";
                std::string reply = util::successJSON("login successful");
                message.reply(status_codes::OK, reply);
            } else{
                ucout << "login failed\n";
                std::string reply = util::failureJSON("login failed");
                message.reply(status_codes::Unauthorized, reply);
            }
        } catch(LafException &e){
            ucout << e.what() << "\n";
            std::string reply = util::failureJSON(e.what());
            message.reply(status_codes::InternalError, reply);
        }
    });
    return;
};

void Handler::getUserProfile(http_request message,std::string username){
    message.extract_string(false).then([=](utility::string_t s){
        try{
            std::string password = util::parsePassword(s);
            std::string dataPass = ModelClass::getPasswordHash(username);
            if(password.compare(dataPass) == 0){
                ucout << "authorized\n";
                User u = ModelClass::getUserObject(username);
                std::string userJSON = u.getJSON();
                ucout << userJSON << 'n';
                message.reply(status_codes::OK, userJSON);
            } else{
                ucout << "unauthorized\n";
                std::string reply = util::failureJSON("login failed");
                message.reply(status_codes::Unauthorized, reply);
            }
        } catch(ModelException &e){
            ucout << e.what() << "\n";
            std::string reply = util::failureJSON(e.what());
            message.reply(status_codes::InternalError, reply);
        }
    });
    return;
}
























