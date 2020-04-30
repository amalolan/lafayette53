#include "handler.h"

void Handler::handle_error(http_request message, pplx::task<void>& t, std::string error)
{
    try {
        t.get();
    }
    catch(LoginException &l) {
        ucout << l.what() << "\n";
        message.reply(status_codes::Unauthorized, error + "\n" +  Util::getFailureJsonStr(l.what()));
    }
    catch(ModelException &m) {
        ucout << m.what() << "\n";
        message.reply(status_codes::Conflict, Util::getFailureJsonStr(error /* + std::string(m.what())*/));
    }
    catch(json::exception &j) {
        ucout << "Internal Error"<<j.what() << '\n';
        message.reply(status_codes::InternalError, Util::getFailureJsonStr(j.what()));
    }
    catch(std::exception &e)
    {
        ucout << e.what() << "\n";
        message.reply(status_codes::InternalError, Util::getFailureJsonStr("Caught error " +  std::string(e.what())));
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
    QDirIterator dirIt((std::string(this->codeBaseDirectory)+"/frontend/").c_str(), QDirIterator::NoIteratorFlags);
    // || (paths[0] == "home" && paths.size() == 1)
    if(message.relative_uri() ==  "/" )
    {
        returnFrontendFile(message);
        return;
    }
    //dirIt.hasNext() can not be written in while statement
    //causes errors for the last file in the directory.
    while(true)
    {
        std::string s = "/" + dirIt.fileName().toStdString();
        if(message.relative_uri().to_string() == s)
        {
            returnFrontendFile(message);
            return ;
        }
        if(!dirIt.hasNext()) break;
        dirIt.next();
    }
    //frontend check ends here

    //check for specific urls
    // URL: /request/museum-list/
    if (paths.size() >= 2 && paths[0] == "request")
    {
        if(paths[1] == "museum-list" && paths.size() == 2)
        {
            returnMuseumList(message);
            return;
        }
        else if (paths.size() == 3) {
            int id;
            try {
                id  =  std::stoi(paths[2]);
            } catch(std::exception &e) {
                message.reply(status_codes::InternalError,
                                Util::getFailureJsonStr("ID could not be converted to an integer."));
                return;
            }
            // URL: /request/museum/[id]
            if(paths[1] == "museum" && paths.size() == 3)
            {
                returnMuseumByID(message, id);
                return;
            }
            // URL: /request/collection/[collectionID]
            else if(paths[1] == "collection" && paths.size() == 3)
            {
                std::string collectionID = paths[2];
                returnCollectionByID(message, id);
                return;
            }
            // URL: /request/artifact/[artifactID]
            else if(paths[1] == "artifact" && paths.size() == 3)
            {
                std::string artifactID = paths[2];
                returnArtifactByID(message, id);
                return;
            }
        }
    }
    // URL: /???
    ucout << "Wildcard caught in GET URL \n";
    returnWildCard(message);
    return;

};


void Handler::returnFrontendFile(http_request message){
    std::string mime = "text/html";

    std::string base_path = (std::string(this->codeBaseDirectory) + "frontend");
    bool getIndex = false;
    if(message.relative_uri().to_string().find(".html") != std::string::npos)
    {
        mime = "text/html";
    } else if(message.relative_uri().to_string().find(".js") != std::string::npos)
    {
        mime = "text/javascript";
    } else if(message.relative_uri().to_string().find(".css") != std::string::npos)
    {
        mime = "text/css";
    } else
    {
        getIndex = true;
    }
    std::string path;
    if (getIndex)
    {
        path = base_path + "/index.html";
    } else
    {
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

    std::string path = std::string(this->codeBaseDirectory) + "frontend/index.html";

    concurrency::streams::fstream::open_istream(U(path),std::ios::in)
            .then([=](concurrency::streams::istream is) {
        return message.reply(status_codes::OK, is, U(mime));
    }).then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "Front end Files Error.");
    });

    return;
};


void Handler::returnMuseumList(http_request message){
    message.extract_string(false).then([=](utility::string_t s){
        ucout  << s <<  std::endl;
        json output = Util::arrayFromVector<Museum>(this->model->getMuseumList(),
        {"id", "name", "description", "introduction", "userID", "image"});
        //    json museumList = this->model->getMuseumListJSON();
        message.reply(status_codes::OK, output.dump(4));
    }).then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "Museum list could not be found");
    });
    return;
};


void Handler::returnMuseumByID(http_request message,int museumID){
    message.extract_string(false).then([=](utility::string_t s){
        ucout  << s <<  std::endl;
        json output;
        output["museum"] = Util::getObjectWithKeys<Museum>(this->model->getMuseumObject(museumID),
        {"id", "name", "description", "introduction", "userID", "image"});
        output["collectionList"] = Util::arrayFromVector<Collection>(this->model->getCollectionListByMuseumID(museumID),
        {"id", "name", "description", "introduction", "image"});
//        ucout << output.dump(3) << '\n';
        message.reply(status_codes::OK, output.dump(4));
    }).then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "Museum could not be found");
    });
    return;
}


void Handler::returnCollectionByID(http_request message, int collectionID) {
    message.extract_string(false).then([=](utility::string_t s){
        ucout  << s <<  std::endl;
        Collection col = this->model->getCollectionObject(collectionID);

        json collectionJSON = Util::getObjectWithKeys<Collection>(col,
        {"id", "name", "description", "introduction", "image"});

        json museumJSON = Util::getObjectWithKeys<Museum>(col.getMuseum(),{"id","name"});
        //TODO: outputData["artifactList"] =
        json output = {
            {"collection", collectionJSON},
            {"museum", museumJSON}
        };
        output["artifactList"] = Util::arrayFromVector<Artifact>(this->model->getArtifactsByCollection(collectionID),
        {"id", "name", "description", "introduction", "image"});

        message.reply(status_codes::OK, output.dump(4));
    }).then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "Collection could not be found");
    });
    return;
}

void Handler::returnArtifactByID(http_request message, int artifactID) {
    message.extract_string(false).then([=](utility::string_t s){
        ucout  << s <<  std::endl;
        Artifact artifact = this->model->getArtifact(artifactID);
        json artifactJSON = Util::getObjectWithKeys<Artifact>(artifact,
        {"id", "name", "description", "introduction", "image"});

        std::vector<Collection> collections = this->model->getCollectionsByArtifact(artifactID);
        json collectionListJSON = Util::arrayFromVector<Collection>(collections,{"id","name"});

        json museumJSON = Util::getObjectWithKeys<Museum>(artifact.getMuseum(), {"id"});
        json output = {
            {"museum", {{"id",artifact.getMuseum().getMuseumID()}}},
            {"artifact", artifactJSON},
            {"collectionList", collectionListJSON}
        };
        message.reply(status_codes::OK, output.dump(4));
    }).then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "Artifact could not be found");
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
    auto paths = web::http::uri::split_path(web::http::uri::decode(message.relative_uri().path()));
    // URL: /request
    if (paths.size() == 2  && paths[0] == "request")
    {
        // URL: /request/register
        if (paths[1] == "register")
        {
            addUser(message);
            return;
        }
        // URL: /request/login
        else if (paths[1] == "login")
        {
            validateLogin(message);
            return;
        }
        // URL: /request/add-museum
        else if(paths[1] == "add-museum")
        {
            addMuseum(message);
            return;
        }
        // URL: /request/add-collection
        else if(paths[1] == "add-collection")
        {
            addCollection(message);
            return;
        }
        // URL: /request/user-profile
        else if (paths[1] == "user-profile")
        {
            getUserProfile(message);
            return;
        }
        // URL: /request/add-artifact
        else if (paths[1] == "add-artifact" && paths.size() == 2)
        {
            addArtifact(message);
            return;
        }
    }

    message.extract_string(false).then([](utility::string_t s){
        ucout << s << std::endl;
    });
    message.reply(status_codes::NotFound,Util::getFailureJsonStr("Check the url and try again."));

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

        User museumCurator =  this->model->getUserObject(std::string(data["user"]["username"]));
        std::unique_ptr<Museum> m (new Museum(data["museum"]["name"], data["museum"]["description"],
                data["museum"]["introduction"], data["museum"]["image"], museumCurator));

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
        User user = Util::checkLogin(userJSON, this->model);

        //gets museum and user objects.
        Museum museum = this->model->getMuseumObject((int) data["museum"]["id"]);
        //ucout << museumJson.dump(3) << '\n';

        int curatorID = museum.getUser().getUserID();

        bool isCuratorOfMuseum = (user.getUserID()  == curatorID);

        if(isCuratorOfMuseum){
            Collection *collection = new Collection
                    (
                    data["collection"]["name"],
                    data["collection"]["description"],
                    data["collection"]["introduction"],
                    data["collection"]["image"],
                    museum
                    );

            this->model->saveCollectionToDB(*collection);
            ucout << "saved to database\n";
            delete collection;
            return message.reply(status_codes::OK, Util::getSuccessJsonStr("Collection added successfully."));
        } else{
            ucout << "not the owner\n";
            return message.reply(status_codes::NotImplemented, Util::getFailureJsonStr("You are not the owner of the museum!"));

            //TODO not owner add to request thing.
        }
        //return message.reply(status_codes::NotImplemented, Util::getSuccessJsonStr("Collection Addition Not Implemented"));


    }).then([=](pplx::task<void> t){
        this->handle_error(message, t, "Error adding collection.");
    });
};

void Handler::editArtifact(http_request message){
    message.extract_string(false).then([=](utility::string_t s){
        //general data.
        json data = json::parse(s);
        ucout << data.dump(3) << std::endl;
        Util::validateJSON(data, {"artifact", "collection", "museum", "user"});
        json artifactJSON = data["artifact"];
        //museum
        Util::validateJSON(data["museum"], {"id"});
        Museum m = this->model->getMuseumObject((int)data["museum"]["id"]);
        //editor
        Util::validateJSON(data["user"], {"username", "password"});
        Util::checkLogin(data["user"], this->model);
        User editor = this->model->getUserObject((std::string)data["user"]["username"]);
        //artifact
        Util::validateJSON(artifactJSON, {"name","description", "introduction", "collectionList"});
        Artifact artifact(artifactJSON["name"], artifactJSON["description"],
                          artifactJSON["introduction"], m);
        artifact.setID(artifactJSON["id"]);
        //collctions
        std::vector<Collection> collections;
        json collectionList = artifactJSON["collectionList"];
        for(auto item : collectionList.items())
        {
            json c = item.value();
            Util::validateJSON(c, {"id","name"});
            Collection col(c["name"], "", "", "", m, c["id"]);
            collections.push_back(col);
        }

        Edit<Artifact> edit(artifact, Edit<Artifact>::edit, editor, collections);

        bool isCurator = (m.getUser().getUserID() == editor.getUserID());

        //TODO initialize edit object
        if(isCurator){
            //TODO change collection stuff.
            this->model->updateArtifactInDB(artifact);
            ucout << "edit done.\n";
            return message.reply(status_codes::OK, Util::getSuccessJsonStr("Artifact in your museum updated!"));
        } else{
            this->model->saveEditToDB(edit);
            ucout << "edit added to review list.\n";
            return message.reply(status_codes::OK, Util::getSuccessJsonStr("Edis Successfully added to "
                                                                           "the review list."));
        }

        return message.reply(status_codes::NotImplemented, Util::getFailureJsonStr("Edit unsuccesful "
                                                                                   "try again."));

    }).then([=](pplx::task<void> t){
        this->handle_error(message,t,"Edit unsuccessful.");
    });
}



void Handler::addArtifact(http_request message){
    message.extract_string(false).then([=](utility::string_t s){

        json data = json::parse(s);
        ucout << data.dump(4) << std::endl;
        //validate JSON
        Util::validateJSON(data, {"collection", "artifact", "user", "museum"});
        Util::validateJSON(data["artifact"], {"name", "description", "image", "collectionList","introduction"});
        json collectionList = data["collection"];
        //retrieve data from database. Check login info.
        User u = Util::checkLogin(data["user"], this->model);
        Museum m = this->model->getMuseumObject((int)data["museum"]["id"]);

        //check if artifact has at least one collection associated.
        if(data["artifact"]["collectionList"].size() == 0) {
            return message.reply(status_codes::Conflict, Util::getFailureJsonStr("Artifact has to be associated with"
                                                                                 " at least one collection."));
        }

        bool isCuratorOfMuseum = (m.getUser().getUserID() == u.getUserID());
        if(isCuratorOfMuseum)
        {
            Artifact* artifact  =  new Artifact(data["artifact"]["name"], data["artifact"]["description"],
                    data["artifact"]["introduction"], data["artifact"]["image"], m);
            this->model->saveArtifactToDB(*artifact);
            for(auto& item : collectionList.items())
            {
                Collection col = this->model->getCollectionObject(item.value());
                this->model->addArtifactCollection(*artifact,col);
            }
            ucout << "Artifact Saved to database.";
            delete artifact;
            return message.reply(status_codes::OK, Util::getSuccessJsonStr("Artifact saved to database."));
        } else {
            //todo add to edit list.
            ucout << "not authorized\n";
            return message.reply(status_codes::NotImplemented, Util::getFailureJsonStr("You re not authorized to add artifact."));
        }

    }).then([=](pplx::task<void> t){
        this->handle_error(message, t, "Artifact addition incomplete.");
    });

}
void Handler::addUser(http_request message){
    message.extract_string(false).then([=](utility::string_t s){
        json userJSON = json::parse(s);
        Util::validateJSON(userJSON, {"username", "email", "password"});
        User *user = new User(userJSON["username"], userJSON["email"], userJSON["password"]);
        this->model->saveUserToDB(*user);
        delete user;
        ucout << "success add user\n";
        return message.reply(status_codes::OK, Util::getSuccessJsonStr("User registered."));
    }).then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "User could not be registered. There might already exist an account with the same username/email.");
    });
    return;
};


void Handler::getUserProfile(http_request message){
    message.extract_string(false).then([=](utility::string_t s){
        json data = json::parse(s);
        Util::validateJSON(data, {"username", "password"});
        Util::checkLogin(data,  this->model);
        ucout << "Authorized.\n";
        std::string username = data["username"];

        User u = this->model->getUserObject(username);

        json userJSON = Util::getObjectWithKeys<User>(u, {"username", "email", "id"});

        //TODO user,museumList,actionList,editList

        json output;
        output["user"] = userJSON;

        //TODO json museumList = Util::arrayFromVector<Museum>(this->model->getMuseumByCurator(u.getUserID()),
        //{"id","name","introduction","description","userID","image"}

        message.reply(status_codes::OK, Util::getObjectWithKeys<User>(u,
        {"username", "email", "id"}).dump(3));

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


