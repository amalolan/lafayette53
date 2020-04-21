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
        message.reply(status_codes::Unauthorized, Util<T>::getFailureJsonStr(error /* std::string(m.what())*/));
        ucout << m.what() << "\n";
    }
    catch(std::exception &e)
    {
        message.reply(status_codes::InternalError, Util<T>::getFailureJsonStr(error /*+ std::string(e.what())*/));
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
            returnMuseumAndCollectionsById(message,std::stoi(museumID));
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
        this->handle_error(message, t, "Museum list could not be found.");
    });
    return;
};

template < class T >
void Handler<T>::returnMuseumById(http_request message,int museumID){
    message.reply(status_codes::OK,T::getMuseumInfoJSON(museumID))
            .then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "Museum could not be found.");
    });
    return;
}

template <class T>
void Handler<T>::returnMuseumAndCollectionsById(http_request message, int museumID) {
    message.reply(status_codes::OK,T::getMuseumAndCollectionInfoJSON(museumID).dump())
            .then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "Museum and collections could not be found.");
    });
    return;
}

template < class T >
void Handler<T>::returnCollectionById(web::http::http_request message, int collectionID) {
    //TODO change ModelClassExt to T
    json obj = ModelClassExt::getCollectionInfoJSON(2);
    message.reply(status_codes::OK,ModelClassExt::getCollectionInfoJSON(collectionID).dump())
            .then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "Collection could not be found.");
    });
    return;
}

template < class T >
void Handler<T>::returnUserById(http_request message,int userID){
    message.reply(status_codes::OK,T::getUserInfoJSON(userID))
            .then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "User could not be found.");
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

    // URL: /request/login
    if (message.relative_uri().to_string()  == "/request/login"){
        validateLogin(message);
    }
    // URL: /request/add-museum
    else if(message.relative_uri().to_string() ==  "/request/add-museum"){
        addMuseum(message);
    }
    // URL: /request/add-collection
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
        message.reply(status_codes::NotFound,Util<T>::getFailureJsonStr("Check the url and try again."));
    }
    return;

};

template < class T >
void Handler<T>::validateLogin(http_request message){
    message.extract_string(false).then([=](utility::string_t s){

        bool loginCheck = Util<T>::checkLogin(s);

        if(loginCheck) {
            ucout << "login successful\n";
            message.reply(status_codes::OK, Util<T>::getSuccessJsonStr("Login successful."));
        } else{
            ucout << "login failed\n";
            message.reply(status_codes::Unauthorized,
                          Util<T>::getFailureJsonStr("Login failed. Check username/password and try again."));
        }
    }).then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "Login failed.");
    });;
    return;
};

template < class T >
void Handler<T>::addMuseum(http_request message){
    message.extract_string(false).then([=](utility::string_t s){
        Museum *m = Util<T>::parseMuseumJsonStr(s);

        json musObj = json::parse(s);
        std::string userObj = musObj["user"];
        bool loginCheck = Util<T>::checkLogin(userObj);

        if (loginCheck) {
            ucout << "password Correct\n";
            bool t = T::saveMuseumToDB(*m);
            if(t) {
                ucout << "success\n";
                delete m;
                return message.reply(status_codes::OK, Util<T>::getSuccessJsonStr("Museum created."));
            } else {
                ucout << "error\n";
                delete m;
                return message.reply(status_codes::Conflict,Util<T>::getFailureJsonStr("Error creating museum."));
            }
        } else {
            ucout << "login info incorrect\n";
            delete m;
            return message.reply(status_codes::Unauthorized, Util<T>::getFailureJsonStr("Login error."));
        }
    }).then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "Error adding museum.");
    });;
};

/*
{
"museum": {"id": 1 },
"collection": {"description": "museum Description", "title": "museum title", "introduction": "intro"},
"user": {"username": "testUser", "password": "test"}
}

*/
template < class T >
void Handler<T>::addCollection(web::http::http_request message) {
    message.extract_string(false).then([=](utility::string_t s){
        //parses user and museum objects from stirng.
        json colObj = json::parse(s);
        json userObj = colObj["user"];


        int musId = colObj["museum"]["id"];

        //gets museum and user objects.
        //TODO change ModelClassExt to T
        json musObj = ModelClassExt::getMuseumInfoJson(musId);
        //TODO change ModelClassExt to T
        json user = ModelClassExt::getUserInfoJson(userObj["username"]);

        bool loginCheck = false;
        if(userObj["password"] == user["password"]){
            loginCheck = true;
        }

        //std::string userjson = userObj;
        //TODO bool loginCheck = Util<T>::checkLogin(userjson);
        bool owner = false;

        if(user["id"] == musObj["userID"]){
            owner = true;
        } else{
            owner = false;
        }

        if(loginCheck){
            if(owner){
                ucout << "saved to database\n";
            } else{
                //TODO not owner add to request thing.
            }
        } else{
            message.reply(status_codes::Unauthorized,U("Login failed. Login and try again."));
        }

    }).then([=](pplx::task<void> t){
        this->handle_error(message, t, "Error adding collection.");
    });
}

template < class T>
void Handler<T>::addUser(http_request message){
    message.extract_string(false).then([=](utility::string_t s){
        ucout << "User " << s;
        User *u = Util<T>::parseUserJsonStr(s);
        if(T::saveUserToDB(*u)){
            ucout << "success add user\n";
            return message.reply(status_codes::OK, Util<T>::getSuccessJsonStr("User registered."));
        } else {
            ucout << "failed to add user\n";
            return message.reply(status_codes::Conflict,
                                 Util<T>::getFailureJsonStr("User could not be registered check username/email."));
        }
    }).then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "Error registering user check username/email.");
    });
    return;
};

template < class T >
void Handler<T>::getUserProfile(http_request message){
    message.extract_string(false).then([=](utility::string_t s){
        bool loginCheck = Util<T>::checkLogin(s);

        json userObj = json::parse(s);
        std::string username = userObj["username"];

        if(loginCheck){
            ucout << "Authorized.\n";
            User u = T::getUserObject(username);
            std::string userJSON = u.getJSON();
            message.reply(status_codes::OK, userJSON);
        } else{
            ucout << "unauthorized\n";
            message.reply(status_codes::Unauthorized, Util<T>::getFailureJsonStr("Login failed."));
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
    //TODO removed after frontend sends POST request
    if (message.relative_uri().to_string() == "/request/user-profile") {
            getUserProfile(message);
            return;
        }
    message.reply(status_codes::NotFound,Util<T>::getFailureJsonStr("No PUT methods implemented."));
    return;
};

//
// A DELETE request
//
template < class T >
void Handler<T>::handle_delete(http_request message)
{
    ucout <<  message.to_string() << std::endl;
    message.reply(status_codes::NotFound, Util<T>::getFailureJsonStr("No DELETE methods implemented"));
    return;
};

//
// Helpers that use <T>
//


