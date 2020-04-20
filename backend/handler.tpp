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
    // URL: /request/museum-list/
    if (paths[0] == "request") {
        if(paths[1] == "museum-list"){
            returnMuseumList(message);
        }
        // URL: /request/museum/[id]
        else if(paths[1] == "museum" && paths.size() == 3){
            ucout << "museum\n";
            std::string museumID = paths[2];
            returnMuseumById(message,std::stoi(museumID));
        }
        // URL: /request/collection/[collectionID]
        else if(paths[1] == "collection" && paths.size() == 3) {
            ucout<< "collection\n";
            std::string collectionID = paths[2];
            returnCollectionById(message, std::stoi(collectionID));
        }
        // URL: /request/user/[id]
        else if(paths[1] == "user" && paths.size() == 3){
            ucout << "user\n";
            std::string userID = paths[2];
            returnUserById(message, std::stoi(userID));
        }
    }
    else {
        ucout << "Wildcard caught in GET URL \n";
        returnWildCard(message);
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
            .then([=](concurrency::streams::istream is) {
        return message.reply(status_codes::OK, is, U(mime));
    }).then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "Front end Files Error: ");
    });
    return;
}

template < class T >
void Handler<T>::returnWildCard(http_request message)
{
    std::string mime = "text/html";

    std::string base_path = (std::string(CODE_BASE_DIRECTORY) + "frontend/");

    std::string path = base_path + "index.html";

    concurrency::streams::fstream::open_istream(U(path),std::ios::in)
            .then([=](concurrency::streams::istream is) {
        return message.reply(status_codes::OK, is, U(mime));
    }).then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "Front end Files Error: ");
    });

    return;
};

template < class T >
void Handler<T>::returnMuseumList(http_request message){

    message.reply(status_codes::OK,T::getMuseumListJSON())
            .then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "Return Museum List Error: ");
    });
    return;
};

template < class T >
void Handler<T>::returnMuseumById(http_request message,int museumID){
    message.reply(status_codes::OK,T::getMuseumInfoJSON(museumID))
            .then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "Return Museum by ID Error: ");
    });;
    return;
}

template < class T >
void Handler<T>::returnCollectionById(web::http::http_request message, int collectionID) {
    message.reply(status_codes::OK,T::getCollectionInfoJSON(collectionID))
            .then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "Return Collection by ID Error: ");
    });
    return;
}

template < class T >
void Handler<T>::returnUserById(http_request message,int userID){
    message.reply(status_codes::OK,T::getUserInfoJSON(userID))
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
    ucout << "relative uri POST " << message.relative_uri().to_string() << "\n";

    // URL: /login
    if (message.relative_uri().to_string()  == "/request/login"){
        validateLogin(message);
    }
    // URL: /request/add-museum
    else if(message.relative_uri().to_string() ==  "/request/add-museum"){
        addMuseum(message);
    }
    // URL: /request/collection
    else if(message.relative_uri().to_string() == "/request/add-collection") {
        addCollection(message);
    }
    // URL: /request/user
    else if(message.relative_uri().to_string() == "/request/register"){
        addUser(message);
    }
    // URL: /request/user-profile
    else if (message.relative_uri().to_string() == "/request/user-profile") {

        getUserProfile(message);
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
void Handler<T>::validateLogin(http_request message){
    message.extract_string(false).then([=](utility::string_t s){
        json obj = json::parse(s);
        std::string username  = obj["username"];
        std::string password = obj["password"];
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

template < class T >
void Handler<T>::addCollection(web::http::http_request message) {

}

template < class T>
void Handler<T>::addUser(http_request message){
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

template < class T >
void Handler<T>::getUserProfile(http_request message){
    message.extract_string(false).then([=](utility::string_t s){
        json obj = json::parse(s);
        std::string username  = obj["username"];
        std::string password = obj["password"];
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
// A PUT request
//
template < class T >
void Handler<T>::handle_put(http_request message)
{
    ucout << "PUT " << message.relative_uri().to_string() << "\n";

    message.reply(status_codes::NotFound,Util::getFailureJsonStr("No PUT methods implemented."));
    return;
};

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


