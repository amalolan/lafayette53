#include "handler.h"

void Handler::handle_error(http_request message, pplx::task<void>& t, std::string error)
{
    try {
        t.get();
    }
    catch(LoginException &l) {

        message.reply(status_codes::Unauthorized, error + "\n" +  Util::getFailureJsonStr(l.what()));
        ucout << l.what() << "\n";
    }

    catch(ModelException &m) {
        message.reply(status_codes::Conflict, Util::getFailureJsonStr(error /* + std::string(m.what())*/));
        ucout << m.what() << "\n";
    }
    catch(json::exception &j) {
        message.reply(status_codes::InternalError, Util::getFailureJsonStr(j.what()));
        ucout << j.what() << '\n';
    }

    catch(std::exception &e)
    {
        message.reply(status_codes::InternalError, Util::getFailureJsonStr(error/* + std::string(e.what())*/));
        ucout << e.what() << "\n";
    }
}


//
// Get Request
//

void Handler::handle_get(http_request message)
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
            .then([=](concurrency::streams::istream is) {
        return message.reply(status_codes::OK, is, U(mime));
    }).then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "Front end Files Error.");
    });
    return;
}


void Handler::returnWildCard(http_request message)
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


void Handler::returnMuseumList(http_request message){

    message.reply(status_codes::OK,this->model->getMuseumListJSON())
            .then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "Museum list could not be found.");
    });
    return;
};


void Handler::returnMuseumById(http_request message,int museumID){
    json outputData = {
        {"museum", this->model->getMuseumInfoJson(museumID)},
        {"collectionList", this->model->getCollectionListByMuseumID(museumID)}
    };
    ucout << outputData.dump(3) << '\n';
    //ucout<<outputData.dump(4)<<std::endl;
    message.reply(status_codes::OK,outputData.dump(3))
            .then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "Museum could not be found.");
    });
    return;
}


void Handler::returnCollectionById(web::http::http_request message, int collectionID) {
    json collectionJSON = this->model->getCollectionInfoJSON(collectionID);
    message.reply(status_codes::OK, collectionJSON.dump(4))
            .then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "Collection could not be found.");
    });
    return;
}

// DEPRECATED
//
//void Handler::returnUserById(http_request message,int userID){
//    message.reply(status_codes::Gone,this->model->getUserInfoJSON(userID).dump(4))
//            .then([=] (pplx::task<void> t) {
//        this->handle_error(message, t, "User could not be found.");
//    });;
//    return;
//}

//
// A POST request
//

void Handler::handle_post(http_request message)
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
        message.reply(status_codes::NotFound,Util::getFailureJsonStr("Check the url and try again."));
    }
    return;

};


void Handler::validateLogin(http_request message){
    message.extract_string(false).then([=](utility::string_t s){
        //ucout  << s<<  std::endl;
        Util::checkLogin(json::parse(s),  this->model);
        ucout << "Login successful\n";
        return message.reply(status_codes::OK, Util::getSuccessJsonStr("Login successful."));
    }).then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "Login failed.");
    });
    return;
};


void Handler::addMuseum(http_request message){
    message.extract_string(false).then([=](utility::string_t s){
        json data = json::parse(s);
        Util::validateJSON(data, {"museum", "user"});
        Util::validateJSON(data["museum"], {"name", "description", "introduction"});
        Util::checkLogin(data["user"],  this->model);
        ucout << data.dump(3);
        User museumCreator =  this->model->getUserObject(std::string(data["user"]["username"]));
        std::unique_ptr<Museum> m (new Museum(data["museum"]["name"], data["museum"]["description"], museumCreator));
        this->model->saveMuseumToDB(*m);
        ucout << "Success saving museum to DB\n";
        return message.reply(status_codes::OK, Util::getSuccessJsonStr("Museum created."));

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

void Handler::addCollection(web::http::http_request message) {
    message.extract_string(false).then([=](utility::string_t s){

        //parses user and museum objects from stirng.
        json data = json::parse(s);
        ucout << data.dump(3) << '\n';
        // Validate JSON only works at a single level. This means we have to validate each
        // nested object individually.
        Util::validateJSON(data, {"museum", "collection", "user"});
        Util::validateJSON(data["museum"], {"id"});
        Util::validateJSON(data["collection"], {"description", "name", "introduction"});
        json userJSON = data["user"];
        Util::checkLogin(userJSON,  this->model);

        //gets museum and user objects.
        json museumJson = this->model->getMuseumInfoJson(data["museum"]["id"]);
        //ucout << museumJson.dump(3) << '\n';
        User user = this->model->getUserObject(std::string(userJSON["username"]));
        Museum museum(museumJson["name"], museumJson["description"],
                user, museumJson["id"]);

        //TODO: this migh be a problem after website starts returning int instead of string.
        int userID = museumJson["userID"];

        bool isCuratorOfMuseum = (user.getUserID()  == userID);

        if(isCuratorOfMuseum){

            // TODO: Send args to constructor once ready
            Collection *collection = new Collection(data["collection"]["name"],
                    data["collection"]["description"],
                    museum);
            this->model->saveCollectionToDB(*collection);
            ucout << "saved to database\n";
            return message.reply(status_codes::OK, Util::getSuccessJsonStr("Collection added successfully."));
         } else{
            ucout << "not the owner\n";
            return message.reply(status_codes::NotImplemented, Util::getSuccessJsonStr("You are not the owner of the museum!"));

              //TODO not owner add to request thing.
         }
        return message.reply(status_codes::NotImplemented, Util::getSuccessJsonStr("Collection Addition Not Implemented"));


    }).then([=](pplx::task<void> t){
        this->handle_error(message, t, "Error adding collection.");
    });
}

void Handler::addUser(http_request message){
    message.extract_string(false).then([=](utility::string_t s){
        json userJSON = json::parse(s);
        Util::validateJSON(userJSON, {"username", "email", "password"});
        User *user = new User(userJSON["username"], userJSON["email"], userJSON["password"]);
        this->model->saveUserToDB(*user);
        ucout << "success add user\n";
        return message.reply(status_codes::OK, Util::getSuccessJsonStr("User registered."));
    }).then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "User could not be registered. There might already exist an account with the same username/email.");
    });
    return;
};


void Handler::getUserProfile(http_request message){
    message.extract_string(false).then([=](utility::string_t s){
        json userJSON = json::parse(s);
        Util::checkLogin(userJSON,  this->model);
        std::string username = userJSON["username"];
        ucout << "Authorized.\n";
        User u = this->model->getUserObject(username);
        message.reply(status_codes::OK, u.getJSON());
    }).then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "Get User Profile Error.");
    });
    return;
}


//
// A PUT request
//

void Handler::handle_put(http_request message)
{
    ucout << "PUT " << message.relative_uri().to_string() << "\n";
    //TODO removed after frontend sends POST request
    if (message.relative_uri().to_string() == "/request/user-profile") {
        getUserProfile(message);
        return;
    }
    message.reply(status_codes::NotFound,Util::getFailureJsonStr("No PUT methods implemented."));
    return;
};

//
// A DELETE request
//

void Handler::handle_delete(http_request message)
{
    ucout <<  message.to_string() << std::endl;
    message.reply(status_codes::NotFound, Util::getFailureJsonStr("No DELETE methods implemented"));
    return;
};

//
// Helpers that use <T>
//


