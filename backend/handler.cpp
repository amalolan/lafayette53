#include "handler.h"

void Handler::handle_error(http_request message, pplx::task<void>& t, std::string error)
{
    try {
        t.get();
    }
    catch(BackendException &l) {
        ucout << l.what() << "\n";
        message.reply(status_codes::Unauthorized, Util::getFailureJsonStr(l.what()));
    }
    catch(ModelException &m) {
        ucout << m.what() << "\n";
        message.reply(status_codes::Conflict, Util::getFailureJsonStr(error /* + std::string(m.what())*/));
    }
    catch(json::exception &j) {
        ucout << "Internal Error "<<j.what() << '\n';
        message.reply(status_codes::InternalError, Util::getFailureJsonStr(error));
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
            // URL: /request/edit/[editID/
            else if(paths[1] == "edit" && paths.size() == 3)
            {
                std::string editID = paths[2];
                returnEditByID(message, stoi(editID));
                return;
            }
        }
    }
    // URL: /???
    ucout << "Wildcard caught in GET URL \n";
    returnWildCard(message);
}


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
}


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
}


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
}


void Handler::returnCollectionByID(http_request message, int collectionID) {
    message.extract_string(false).then([=](utility::string_t s){
        ucout  << s <<  std::endl;
        Collection col = this->model->getCollectionObject(collectionID);

        json collectionJSON = Util::getObjectWithKeys<Collection>(col,
        {"id", "name", "description", "introduction", "image"});

        json museumJSON = Util::getObjectWithKeys<Museum>(col.getMuseum(),{"id","name"});
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
            {"museum", museumJSON},
            {"artifact", artifactJSON},
            {"collectionList", collectionListJSON}
        };
        message.reply(status_codes::OK, output.dump(4));
    }).then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "Artifact could not be found");
    });
}

void Handler::returnEditByID(http_request message, int editID)
{
    message.extract_string(false).then([=](utility::string_t s){
        ucout<< s <<std::endl;
        try {
            return message.reply(status_codes::OK,
                                 Util::getArtifactEditJSON(this->model->getEditArtifactObject(editID)).dump(4));
        } catch (ModelException &e) {
            ucout << e.what() << std::endl;
        }
        try {
            return message.reply(status_codes::OK,
                                 Util::getCollectionEditJSON(this->model->getEditCollectionObject(editID)).dump(4));
        } catch (ModelException &e) {
            ucout << e.what() << std::endl;
        }
        throw ModelException("No edits found in DB.");
//         TODO museum edit
    }).then([=](pplx::task<void> t){
        handle_error(message, t, "Edit could not be returned");
    });

}

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
            addEditCollection(message, Edit<Collection>::add);
            return;
        }
        // URL: /request/edit-collection
        else if (paths[1] == "edit-collection") {
            addEditCollection(message, Edit<Collection>::edit);
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
            addEditArtifact(message, Edit<Artifact>::add);
            return;
        }
        // URL: /request/edit-artifact
        else if(paths[1] == "edit-artifact" && paths.size() == 2)
        {
            addEditArtifact(message, Edit<Artifact>::edit);
            return;
        }
        // URL: /requst/review-edit
        else if(paths[1] == "review-edit" && paths.size() == 2)
        {
            actOnEdit(message);
            return;
        }
        // URL: /request/reset-password
        else if(paths[1] == "reset-password" && paths.size() == 2)
        {
            changePassword(message);
            return;
        }
        //delete requests.
    } else if ( paths.size() == 3 && paths[0] == "request" )
    {
        int id;
        try {
            id  =  std::stoi(paths[2]);
        } catch(std::exception &e) {
            message.reply(status_codes::InternalError,
                          Util::getFailureJsonStr("ID could not be converted to an integer."));
            return;
        }
        //URL: request/delete-museum/[id]
        if ( paths[1] == "delete-museum" )
        { 
            deleteMuseum(message, id);
            return;
        }
        //URL: request/delete-artifact/[id]
        if ( paths[1] == "delete-artifact")
        {
            deleteArtifact(message, id);
            return;
        }
    }

    message.extract_string(false).then([](utility::string_t s){
        ucout << s << std::endl;
    });
    message.reply(status_codes::NotFound,Util::getFailureJsonStr("Check the url and try again."));
}

void Handler::validateLogin(http_request message){
    message.extract_string(false).then([=](utility::string_t s){
        ucout  << s<<  std::endl;
        Util::checkLogin(json::parse(s),  this->model);
        ucout << "Login successful\n";
        return message.reply(status_codes::OK, Util::getSuccessJsonStr("Login successful."));
    }).then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "Login failed.");
    });
}

void Handler::addUser(http_request message){
    message.extract_string(false).then([=](utility::string_t s){
        json userJSON = json::parse(s);
        Util::validateJSON(userJSON, {"username", "email", "password"});
        User *user = new User(userJSON["username"], userJSON["email"], userJSON["password"]);
        this->model->saveUserToDB(*user);
        delete user;
        std::string welcomeMessage = (std::string)(userJSON["username"]) + " it is a pleasure having you on our platform!"
                                                            "\nFor further information please contact thead curator!";
        Util::sendEmail(userJSON["email"], "Welcome to Museum Wikia!", welcomeMessage);
        ucout << "success add user\n";
        return message.reply(status_codes::OK, Util::getSuccessJsonStr("User registered."));
    }).then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "User could not be registered. "
                                       "There might already exist an "
                                       "account with the same username/email.");
    });
}

void Handler::addMuseum(http_request message){
    message.extract_string(false).then([=](utility::string_t s){
        json data = json::parse(s);
        Util::validateJSON(data, {"museum", "user"});
        Util::validateJSON(data["museum"], {"name", "description", "introduction", "image"});
        Util::checkLogin(data["user"], this->model);
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
}

void Handler::addEditCollection(web::http::http_request message, int kind) {
    message.extract_string(false).then([=](utility::string_t s){
        //parses user and museum objects from stirng.
        json data = json::parse(s);
        ucout << data.dump(3) << '\n';
        // Validate JSON only works at a single level. This means we have to validate each
        // nested object individually.
        Util::validateJSON(data, {"museum", "collection", "user"});
        Util::validateJSON(data["museum"], {"id"});
        Util::validateJSON(data["collection"], {"description", "name", "introduction", "image"});
        User user = Util::checkLogin(data["user"], this->model);
        //gets museum and user objects.
        Museum museum = this->model->getMuseumObject((int) data["museum"]["id"]);
        Collection collection(data["collection"]["name"], data["collection"]["description"],
                data["collection"]["introduction"], data["collection"]["image"], museum);

        if (kind == Edit<Collection>::edit) {
            Util::validateJSON(data["collection"], {"id"});
            this->model->getCollectionObject((int) data["collection"]["id"]);
            collection.setID(data["collection"]["id"]);
        }
        bool isCuratorOfMuseum = (user.getUserID()  == museum.getUser().getUserID());
        ucout<<"Is curator " << isCuratorOfMuseum << std::endl;

        std::string statusMessage;
        if(isCuratorOfMuseum){
            if (kind == Edit<Collection>::edit) {
                this->model->updateCollectionInDB(collection);
                ucout << "edit done.\n";
                statusMessage = "Collection has been successfully edited.";
            } else {
                this->model->saveCollectionToDB(collection);
                ucout << "saved to database\n";
                statusMessage =  "Collection added successfully.";
            }
        } else{
            ucout<<"Trying to create edit object for collection"<<std::endl;
            std::time_t time = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now());
            ucout<<std::ctime(&time)<<std::endl;
            Edit<Collection> edit(collection, kind, user);
            this->model->saveEditToDB(edit);
            ucout << "edit/add collection added to review list.\n";
            statusMessage = "Your collection edit/addition will be published after review.";
        }
        return message.reply(status_codes::OK, Util::getSuccessJsonStr(statusMessage));
    }).then([=](pplx::task<void> t){
        this->handle_error(message, t, "Task unsuccessful. Please ensure no other collection has"
                                       " the same name.");
    });
};

void Handler::addEditArtifact(http_request message, int kind) {
    message.extract_string(false).then([=](utility::string_t s){
        json data = json::parse(s);
        // Validation
        ucout << data.dump(3) << std::endl;
        Util::validateJSON(data, {"artifact", "collection", "museum", "user"});
        json artifactJSON = data["artifact"];
        Util::validateJSON(artifactJSON, {"name","description", "introduction", "image"});
        Util::validateJSON(data["museum"], {"id"});
        User user = Util::checkLogin(data["user"], this->model);
        Museum m = this->model->getMuseumObject((int)data["museum"]["id"]);
        json collectionList = data["collection"];
        if (collectionList.size() == 0) {
            return message.reply(status_codes::Conflict, Util::getFailureJsonStr("Artifact has to be associated with"
                                                                                 " at least one collection."));
        }
        std::vector<Collection> collections;
        for (auto item : collectionList.items())
        {
            collections.push_back(this->model->getCollectionObject(item.value()));
        }
        Artifact artifact(data["artifact"]["name"], data["artifact"]["description"],
                data["artifact"]["introduction"], data["artifact"]["image"], m);
        if (kind == Edit<Artifact>::edit) {
            Util::validateJSON(artifactJSON, {"id"});
            this->model->getArtifact((int) artifactJSON["id"]);
            artifact.setID(artifactJSON["id"]);
        }
        bool isCurator = (m.getUser().getUserID() == user.getUserID());
        std::string statusMessage;
        if (isCurator) {
            if (kind == Edit<Artifact>::edit) {
                //TODO change collection stuff.
                this->model->updateArtifactInDB(artifact);
                this->model->removeArtifactCollection(artifact);
                for(Collection col : collections)
                {
                    this->model->addArtifactCollection(artifact,col);
                }
                ucout << "edit done.\n";
                statusMessage = "Artifact has been successfully edited.";
            } else {
                this->model->saveArtifactToDB(artifact);
                for(Collection col : collections)
                {
                    this->model->addArtifactCollection(artifact,col);
                }
                ucout << "Artifact Saved to database.";
                statusMessage = "Artifact saved to database.";
            }
        } else {
            Edit<Artifact> edit(artifact, kind, user, collections);
            std::time_t time = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now());
            ucout<<std::ctime(&time)<<std::endl;
            this->model->saveEditToDB(edit);
            ucout << "edit artifact added to review list.\n";
            statusMessage = "Your artifact edit/addition will be published after review.";
        }
        return message.reply(status_codes::OK, Util::getSuccessJsonStr(statusMessage));
    }).then([=](pplx::task<void> t){
        this->handle_error(message, t, "Task unsuccessful. Please ensure no other artifact has"
                                       " the same name.");
    });

}

void Handler::getUserProfile(http_request message){
    message.extract_string(false).then([=](utility::string_t s){
        json data = json::parse(s);
        User user = Util::checkLogin(data,  this->model);
        ucout << "Authorized.\n";
        ucout << data.dump(3) << '\n';
        // All of the user's requested edits.
        json editList = json::array();
        for(auto edit : this->model->getArtifactEdits(user.getUserID())) {
            editList.push_back(Util::getArtifactEditJSON(edit));
        }
        for (auto edit: this->model->getCollectionEdits(user.getUserID())) {
            editList.push_back(Util::getCollectionEditJSON(edit));
        }
        std::vector<Museum> museums = this->model->getMuseumByCurator(user.getUserID());
        // All of other user's edits that are pending for the user's approval.
        json actionsList = json::array();
        for(Museum museum : museums) {
            ucout << museum.getMuseumID() << ' ';
            for (auto action : this->model->getArtifactActions(museum.getMuseumID())) {
                actionsList.push_back(Util::getArtifactEditJSON(action));
            }
            for (auto action: this->model->getCollectionActions(museum.getMuseumID())) {
                actionsList.push_back(Util::getCollectionEditJSON(action));
            }
        }

        // TODO Headcurator
        bool isHeadCurator = this->model->checkHeadCurator(user);

        json output = {
            {"user", Util::getObjectWithKeys<User>(user, {"username", "email", "id"})},
            {"museumList", Util::arrayFromVector(museums, {"id", "name"})},
            {"editsList", editList},
            {"actionsList", actionsList}
        };
        if (isHeadCurator) {
            output["headCuratorList"] = Util::arrayFromVector(this->model->getMuseumList(), {"id", "name"});
        }
        return message.reply(status_codes::OK, output.dump(3));
    }).then([=] (pplx::task<void> t) {
        this->handle_error(message, t, "Get User Profile Error.");
    });
    return;
}

template <typename T>
std::string Handler::reviewEdit(Edit<T> edit, bool approved, User user) {
    T object = edit.getObject();
    std::string email = edit.getEditor().getEmail();
    std::string emailMessage = "Your edit on " + object.getName() + " has been ";
    bool isCurator = (object.getMuseum().getUser().getUserID() == user.getUserID());
    if (! isCurator) {
        ucout << "permission denied!" << '\n';
        throw BackendException("Permission to act on edit denied!");
    }
    ucout << "permission granted!\n";
    if (approved) {
        edit.approveEdit();
        this->model->updateEditInDB(edit);
        emailMessage += "approved";
        Util::sendEmail(email, "Your Edit has been approved.", emailMessage);
        ucout << "edit approved and updated";
        return "Edit approved.";
    } else {
        edit.rejectEdit();
        this->model->updateEditInDB(edit);
        emailMessage += "denied";
        Util::sendEmail(email, "Your Edit has been denied.", emailMessage);
        ucout << "edit rejected and updated\n";
        return "Edit rejected.";
    }
}

void Handler::actOnEdit(http_request message) {
    message.extract_string(false).then([=](utility::string_t s){
        json data = json::parse(s);
        ucout << data.dump(3) << '\n';
        Util::validateJSON(data, {"user", "editId", "action", "category"});
        User user = Util::checkLogin(data["user"], this->model);
        std::string statusMessage;
        if (data["category"] == "artifact") {
            statusMessage = this->reviewEdit(this->model->getEditArtifactObject(data["editId"]),
                    data["action"], user);
        } else if (data["category"] == "collection") {
            statusMessage =  this->reviewEdit(this->model->getEditCollectionObject(data["editId"]),
                    data["action"], user);
        } else if (data["category"] == "museum"){
            return message.reply(status_codes::NotImplemented, Util::getFailureJsonStr("Editing museums has not been implemented"));
        } else {
            throw json::other_error::create(1, "category key has invalid value");
        }
        return message.reply(status_codes::OK, Util::getSuccessJsonStr(statusMessage));
    }).then([=](pplx::task<void> t){
        this->handle_error(message, t, "Review unsuccesful. The updated object's name may clash with another object. "
                                       "Try again, or try rejecting the edit.");
    });
}

void Handler::deleteMuseum(http_request message, int museumID)
{
    message.extract_string(false).then([=](utility::string_t s){
        ucout << s << std::endl;
        json data =json::parse(s);
        Util::validateJSON(data,{"username", "password"});
        User editor = Util::checkLogin(data,this->model);
        Museum museum = this->model->getMuseumObject(museumID);

        bool isCurator = (editor.getUserID() == museum.getUser().getUserID());
        bool isHeadCurator = this->model->checkHeadCurator(editor);
        ucout << "Is curator "<< isCurator << "\n" ;
        if ( isCurator || isHeadCurator)
        {
            this->model->removeMuseumFromDB(museum);
            ucout << "museum deleted.\n";
            return message.reply(status_codes::OK, Util::getSuccessJsonStr("Museum successfuly deleted."));
        }
        else
        {
            ucout << "not the curator/head curator \n";
            return message.reply(status_codes::Unauthorized, Util::getFailureJsonStr("You are not autorized"
                                                                                     " to remove this museum."));
        }
    }).then([=](pplx::task<void> t){
        this->handle_error(message, t, "Delete unsuccessful.");
    });
}

void Handler::deleteArtifact(http_request message, int artifactID)
{
    message.extract_string(false).then([=](utility::string_t s){
        json data = json::parse(s);
        User editor = Util::checkLogin(data, this->model);
        Artifact artifact = this->model->getArtifact(artifactID);

        bool isCurator = (editor.getUserID() == artifact.getMuseum().getUser().getUserID());

        if ( isCurator ) {
            this->model->removeArtifactInDB(artifact);
            ucout << "artifact deleted\n";
            return message.reply(status_codes::OK, Util::getSuccessJsonStr("Artifact deleted."));
        } else {
            std::vector<Collection> collections = this->model->getCollectionsByArtifact(artifactID);
            Edit<Artifact> edit(artifact, Edit<Artifact>::del, editor, collections);
            ucout << "artifact saved to DB\n";
            this->model->saveEditToDB(edit);
            return message.reply(status_codes::OK, Util::getSuccessJsonStr("Artifact will be deleted after review."));
        }
    }).then([=](pplx::task<void> t){
        handle_error(message, t, "Delete could not be processed.");
    });
}

void Handler::changePassword(http_request message)
{
    message.extract_string(false).then([=](utility::string_t s){
        json data = json::parse(s);
        Util::validateJSON(data, {"username"});
        ucout << data.dump(3) << '\n';
        User u = this->model->getUserObject((std::string)data["username"]);
        //random password generator
        int len = qrand() % 10 + 8;
        QString pass;
        pass.resize(len);
        for (int s = 0; s < len ; ++s)
        {
            pass[s] = QChar('A' + char(qrand() % ('Z' - 'A')));
        }
        QByteArray result = QCryptographicHash::hash(pass.toUtf8(), QCryptographicHash::Sha512);
        QString inputHash = QLatin1String(result.toHex());
        std::string sha = inputHash.toStdString();
        u.setPassword(sha);
        this->model->updateUserInDB(u);
        std::string body = u.getName() + " your password has been reset."
                                         " new pasword: " + pass.toStdString();
        Util::sendEmail(u.getEmail(),"Password Reset", body);
        return message.reply(status_codes::OK, Util::getSuccessJsonStr("Password sent to your email!"));
    }).then([=](pplx::task<void> t){
        handle_error(message, t, "Could not reset password.");
    });
}
//
// A PUT request
//

void Handler::handle_put(http_request message)
{
    ucout << "PUT " << message.relative_uri().to_string() << "\n";
    message.reply(status_codes::NotFound,Util::getFailureJsonStr("Check the url and try again."));
    return;
}

//
// A DELETE request
//

void Handler::handle_delete(http_request message)
{
    ucout <<"Delete " << message.relative_uri().to_string() << std::endl;

    message.reply(status_codes::NotFound,Util::getFailureJsonStr("Check the url and try again."));
    return;
}



