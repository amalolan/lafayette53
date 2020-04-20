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
void Handler<T>::handle_error(http_request message, pplx::task<void>& t, std::string error)
{
    try
    {
        t.get();
    }
    catch(ModelException &m) {
        message.reply(status_codes::Unauthorized, Util::getFailureJsonStr(error + std::string(m.what())));
        ucout << m.what() << "\n";
    }
    catch(std::exception &e)
    {
        message.reply(status_codes::InternalError, Util::getFailureJsonStr(error + std::string(e.what())));
        ucout << e.what() << "\n";
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

    // URL: / or /home
    //check for frontend files.
    QDirIterator dirIt((std::string(CODE_BASE_DIRECTORY)+"frontend/").c_str(), QDirIterator::NoIteratorFlags);
    // || (paths[0] == "home" && paths.size() == 1)
    if(message.relative_uri() ==  "/" ) {
        returnFrontendFile(message);
        return;
    }
    while(dirIt.hasNext()){
        std::string s = "/" + dirIt.fileName().toStdString();

        if(message.relative_uri().to_string() == s) {
            returnFrontendFile(message);
            return ;
        }
        dirIt.next();
    }
    //frontend check ends here
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
        returnWildCard(message);
//        message.reply(status_codes::NotFound,Util::getFailureJsonStr("Check the GET url and try again"));
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
                .then([=] (pplx::task<void> t) {
            this->handle_error(message, t, "Front end Files Error: ");
        });
    }).then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "Front end Files Error: ");
    });
    return;
}

template < class T >
void Handler<T>::returnMuseumList(http_request message){

    message.reply(status_codes::OK,T::getMuseumListJSON())
            .then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "Return Museum List Error: ");
    });
    return;
};

template < class T >
void Handler<T>::returnWildCard(http_request message)
{
    std::string mime = "text/html";

    std::string base_path = (std::string(CODE_BASE_DIRECTORY) + "frontend/");

    std::string path = base_path + "index.html";

    concurrency::streams::fstream::open_istream(U(path),std::ios::in)
            .then([=](concurrency::streams::istream is)
    {
        message.reply(status_codes::OK, is, U(mime))
                .then([=] (pplx::task<void> t) {
            this->handle_error(message, t, "Front end Files Error: ");
        });
    }).then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "Front end Files Error: ");
    });
    return;
};

template < class T >
void Handler<T>::returnMuseumById(http_request message,int musId){
    message.reply(status_codes::OK,T::getMuseumInfoJSON(musId))
            .then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "Return Museum by ID Error: ");
    });;
    return;
}

template < class T >
void Handler<T>::returnUserById(http_request message,int usrId){
    message.reply(status_codes::OK,T::getUserInfoJSON(usrId))
            .then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "Return User by ID ERROR: ");
    });;
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
        message.reply(status_codes::NotFound,Util::getFailureJsonStr("check the POST url and try again"));
    }
    return;

};

template < class T >
void Handler<T>::addMuseum(http_request message){
    message.extract_string(false).then([=](utility::string_t s){
        Museum *m = Util::parseMuseumJsonStr(s);
        std::string username = m->getUsername();
        std::string password = m->getUserPass();
        std::string dataPass = T::getPasswordHash(username);
        bool t =false;
        if (password == dataPass) {
            ucout << "password Correct\n";
            t = T::saveMuseumToDB(*m);
            if(t) {
                ucout << "success\n";
                delete m;
                return message.reply(status_codes::OK, Util::getSuccessJsonStr("Museum created"));
            } else {
                ucout << "error\n";
                delete m;
                return message.reply(status_codes::Conflict,Util::getFailureJsonStr("Museum creation failed"));
            }
        } else {
            ucout << "login info incorrect\n";
            delete m;
            return message.reply(status_codes::Unauthorized, Util::getFailureJsonStr("login error"));
        }
    }).then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "Add Museum Error: ");
    });;
};


template < class T>
void Handler<T>::registerUser(http_request message){
    message.extract_string(false).then([=](utility::string_t s){
        ucout << "User " << s;
        User *u = Util::parseUserJsonStr(s);
        if(T::saveUserToDB(*u)){
            ucout << "success add user\n";
            return message.reply(status_codes::OK, Util::getSuccessJsonStr("user registered"));
        } else {
            ucout << "failed to add user\n";
            return message.reply(status_codes::Conflict, Util::getFailureJsonStr("user could not be registered"));
        }
    }).then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "Register User Error: ");
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

    if(paths[0] == "get-data" && paths[1] == "login" && paths.size() == 3){
        validateLogin(message,paths[2]);
    }
    else if(paths[0] == "get-data" && paths[1] == "user" && paths.size() == 3){
        getUserProfile(message,paths[2]);
        return;
    }
    else {
        message.reply(status_codes::NotFound,Util::getFailureJsonStr("Check the PUT url again"));
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
    }).then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "Validate Login Error: ");
    });;
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
    }).then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "Get User Profile Error: ");
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
    message.reply(status_codes::NotFound, Util::getFailureJsonStr("No DELETE methods implemented"));
    return;
};

//
// Helpers that use <T>
//


