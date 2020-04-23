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
    try {
        t.get();
    }
    catch(LoginException &l) {

        message.reply(status_codes::Unauthorized, error + "\n" +  Util<T>::getFailureJsonStr(l.what()));
        ucout << l.what() << "\n";
    }

    catch(ModelException &m) {
        message.reply(status_codes::Conflict, Util<T>::getFailureJsonStr(error /* + std::string(m.what())*/));
        ucout << m.what() << "\n";
    }
    catch(json::exception &j) {
        message.reply(status_codes::InternalError, Util<T>::getFailureJsonStr(j.what()));
        ucout << j.what() << '\n';
    }

    catch(std::exception &e)
    {
        message.reply(status_codes::InternalError, Util<T>::getFailureJsonStr(error/* + std::string(e.what())*/));
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

    // URL: /
    //check for frontend files.
    QDirIterator dirIt((std::string(CODE_BASE_DIRECTORY)+"/frontend/").c_str(), QDirIterator::NoIteratorFlags);
    // || (paths[0] == "home" && paths.size() == 1)
    if(message.relative_uri() ==  "/" ) {
        returnFrontendFile(message);
        return;
    }
    //dirIt.hasNext() can not be written in while statement
    //causes errors for the last file in the directory.
    while(true){
        std::string s = "/" + dirIt.fileName().toStdString();
        if(message.relative_uri().to_string() == s) {
            returnFrontendFile(message);
            return ;
        }
        if(!dirIt.hasNext()) break;
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
            returnWildCard(message);
//            returnUserById(message, std::stoi(userID));
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
        this->handle_error(message, t, "Front end Files Error.");
    });
    return;
}

template < class T >
void Handler<T>::returnWildCard(http_request message)
{
    std::string mime = "text/html";

    std::string path = std::string(CODE_BASE_DIRECTORY) + "frontend/index.html";

    concurrency::streams::fstream::open_istream(U(path),std::ios::in)
            .then([=](concurrency::streams::istream is) {
        return message.reply(status_codes::OK, is, U(mime));
    }).then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "Front end Files Error.");
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
    json outputData = {
        {"museum", T::getMuseumInfoJSON(museumID)},
        {"collectionList", T::getCollectionListByMuseumID(museumID)}
    };
    ucout << outputData.dump(3) << '\n';
    //ucout<<outputData.dump(4)<<std::endl;
    message.reply(status_codes::OK,outputData.dump(3))
            .then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "Museum could not be found.");
    });
    return;
}

template < class T >
void Handler<T>::returnCollectionById(web::http::http_request message, int collectionID) {
    json collectionJSON = T::getCollectionInfoJSON(collectionID);
    message.reply(status_codes::OK, collectionJSON.dump(4))
            .then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "Collection could not be found.");
    });
    return;
}

// DEPRECATED
//template < class T >
//void Handler<T>::returnUserById(http_request message,int userID){
//    message.reply(status_codes::Gone,T::getUserInfoJSON(userID).dump(4))
//            .then([=] (pplx::task<void> t) {
//        this->handle_error(message, t, "User could not be found.");
//    });;
//    return;
//}

//
// A POST request
//
template < class T >
void Handler<T>::handle_post(http_request message)
{
    //ucout <<  message.to_string() << std::endl;
    ucout << "relative uri POST " << message.relative_uri().to_string() << "\n";
    auto paths = web::http::uri::split_path(web::http::uri::decode(message.relative_uri().path()));
    // URL: /request
    if (paths[0] == "request") {
        // URL: /request/user
        if (paths[1] == "register") {
            addUser(message);
        }
        // URL: /request/login
        else if (paths[1] == "login"){
            validateLogin(message);
        }
        // URL: /request/add-museum
        else if(paths[1] == "add-museum"){
            addMuseum(message);
        }
        // URL: /request/add-collection
        else if(paths[1] == "add-collection") {
            addCollection(message);
        }
        // URL: /request/user-profile
        else if (paths[1] == "user-profile") {
            getUserProfile(message);
        }
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
        //ucout  << s<<  std::endl;
        Util<T>::checkLogin(json::parse(s));
        ucout << "Login successful\n";
        return message.reply(status_codes::OK, Util<T>::getSuccessJsonStr("Login successful."));
    }).then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "Login failed.");
    });
    return;
};

template < class T >
void Handler<T>::addMuseum(http_request message){
    message.extract_string(false).then([=](utility::string_t s){
        json data = json::parse(s);
        Util<T>::validateJSON(data, {"museum", "user"});
        Util<T>::validateJSON(data["museum"], {"name", "description", "introduction"});
        Util<T>::checkLogin(data["user"]);

        User museumCreator =  T::getUserObject(std::string(data["user"]["username"]));
        std::unique_ptr<Museum> m (new Museum(data["museum"]["name"], data["museum"]["description"], museumCreator));
        T::saveMuseumToDB(*m);
        ucout << "Success saving museum to DB\n";
        return message.reply(status_codes::OK, Util<T>::getSuccessJsonStr("Museum created."));

    }).then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "Error creating museum.");
    });
};

/*
{
"museum": {"id": 1 },
"collection": {"description": "museum Description", "name": "museum title", "introduction": "intro"},
"user": {"username": "testUser", "password": "test"}
}

*/
template < class T >
void Handler<T>::addCollection(web::http::http_request message) {
    message.extract_string(false).then([=](utility::string_t s){

        //parses user and museum objects from stirng.
        json data = json::parse(s);
        //ucout << data.dump(3) << '\n';
        // Validate JSON only works at a single level. This means we have to validate each
        // nested object individually.
        Util<T>::validateJSON(data, {"museum", "collection", "user"});
        Util<T>::validateJSON(data["museum"], {"id"});
        Util<T>::validateJSON(data["collection"], {"description", "name", "introduction"});
        json userJSON = data["user"];
        Util<T>::checkLogin(userJSON);

        //gets museum and user objects.
        //TODO web is sending string right now that needs to be fixed.
        json museumJson = T::getMuseumInfoJson(646814068);
        ucout << museumJson.dump(3) << '\n';
        User user = T::getUserObject(std::string(userJSON["username"]));
        Museum museum(museumJson["name"], museumJson["description"],
                user, museumJson["id"]);

        //TODO: this migh be a problem after website starts returning int instead of string.
        int userID = std::stoi((std::string)museumJson["userID"]);

        bool isCuratorOfMuseum = (user.getUserID()  == userID);

        if(isCuratorOfMuseum){

            // TODO: Send args to constructor once ready
            Collection *collection = new Collection(data["collection"]["name"],
                    data["collection"]["description"],
                    museum);
            T::saveCollectionToDB(*collection);
            ucout << "saved to database\n";
            return message.reply(status_codes::OK, Util<T>::getSuccessJsonStr("Collection added successfully."));
         } else{
            ucout << "not the owner\n";
              //TODO not owner add to request thing.
         }
        return message.reply(status_codes::NotImplemented, Util<T>::getSuccessJsonStr("Collection Addition Not Implemented"));


    }).then([=](pplx::task<void> t){
        this->handle_error(message, t, "Error adding collection.");
    });
}

template < class T>
void Handler<T>::addUser(http_request message){
    message.extract_string(false).then([=](utility::string_t s){
        json userJSON = json::parse(s);
        Util<T>::validateJSON(userJSON, {"username", "email", "password"});
        User *user = new User(userJSON["username"], userJSON["email"], userJSON["password"]);
        T::saveUserToDB(*user);
        ucout << "success add user\n";
        return message.reply(status_codes::OK, Util<T>::getSuccessJsonStr("User registered."));
    }).then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "User could not be registered. There might already exist an account with the same username/email.");
    });
    return;
};

template < class T >
void Handler<T>::getUserProfile(http_request message){
    message.extract_string(false).then([=](utility::string_t s){
        json userJSON = json::parse(s);
        Util<T>::checkLogin(userJSON);
        std::string username = userJSON["username"];
        ucout << "Authorized.\n";
        User u = T::getUserObject(username);
        message.reply(status_codes::OK, u.getJSON());
    }).then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "Get User Profile Error.");
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


