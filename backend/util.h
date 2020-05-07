#ifndef UTIL_H
#define UTIL_H
#include <vector>
#include <iostream>
#include "../nlohmann/json.hpp"

#include "../model/ModelException.h"
#include "../model/modelclass.h"
#include <cpprest/http_client.h>
#include <cpprest/http_listener.h>
using json = nlohmann::json;

/**
 * @brief The BackendException class Exceptions on the backend for which the message has to be displayed on the front end use this class.
 * For example, if the user has invalid login credentials but performs POST requests requiring logins,
 * this exception is raised.
 */
class BackendException : public std::exception
{
    std::string _msg;
public:
    /**
     * @brief BackendException Constructor
     * @param msg Default message: "Error on Backend"
     */
    BackendException(const std::string msg = "Error on Backend") : _msg(msg){}

    /**
     * @brief what Describes the error
     * @return The description of the Login Error.
     */
    virtual const char * what () const noexcept override
    {
        return _msg.c_str();
    }
};

/**
 * @brief The Util class A class containing helper functions for Handler
 */
class Util
{
private:
    template<typename T>
    /**
     * @brief ptr Converts a reference/pointer to a pointer. Helper function for Util::getObjectWithKeys().
     * This is need to ensure both pointers and objects can be passed in the template of Util::getObjectWithKeys();
     * @param obj the reference
     * @return a pointer of the reference
     */
    static T* ptr(T & obj) { return &obj; }

    template<typename T>
    /**
     * @brief @see Util::ptr<T>(T& obj)
     * @param obj the pointer
     * @return the pointer itself
     */
    static T* ptr(T * obj) { return obj; } //obj is already pointer, return it!
public:

    /**
     * @brief validateJSON Performs key validation on the outermost JSON object. Raises json::other_error if failed.
     * Ensures each of the keys in the vector of keys is present in the JSON object.
     * Does not check for the presence of keys in nested JSON objects.
     * the jsonObj is allowed to contain more keys than in the keys vector.
     * @param jsonObj The JSON object to validate
     * @param keys The vector of keys to look for in the object.
     */
    static void validateJSON(json &jsonObj, std::vector<std::string> keys) {
        // If any keys are missing, raise an error.
        for  (auto key : keys) {
            if  (! jsonObj.contains(key))
                throw json::other_error::create(1, "JSON Schema not valid");
        }
    }

    /**
     * @brief checkLogin determines if the user in userJSON has the required login credentials.
     * If not, raises LoginException.
     * @param userJSON object schema:
     *  {
     *   "username": string,
     *   "password": string
     * }
     * @return The User object logged in.
     */
    static User checkLogin(json userJSON, ModelClass *model) {
        // First validate.
        Util::validateJSON(userJSON, {"username", "password"});
        std::string username  = userJSON["username"];
        // Get the user object from the database.
        User user = model->getUserObject(username);
        std::string password = user.getPassword();
        // Ensure the password matches.
        if (userJSON["password"] != password) {
            throw BackendException("Login failed. Please check username or password and try again.");
        }
        return user;
    }


    template<typename T>
    /**
     * @brief getObjectWithKeys Converts a C++ object containing a toJSON() method, to its JSON representation.
     * The JSON representation will have all keys specified in the keys vector. It will be validated.
     * Uses Util::ptr() to allow T to be a pointer or a standard object.
     * @param t The C++ object with a toJSON() implementation.
     * @param keys The list of keys required in the final JSON representation. Validation will be done.
     * @return the validated JSON object containing only keys specified in the keys vector
     */
    static json getObjectWithKeys(T t, std::vector<std::string> keys) {
        json tJSON = Util::ptr(t)->toJSON();
        Util::validateJSON(tJSON, keys);
        json object = json::object();
        // Get the required keys from the object's toJSON method.
        for (std::string key : keys) {
            object[key] = tJSON[key];
        }
        return object;
    }

    template<typename T>
    /**
     * @brief arrayFromVector Converts a vector of  objects with toJSON() implementation to a JSON array.
     * The JSON representation of each object will have all keys specified in the keys vector. They will be validated.
     * @param list The vector of C++ objects with toJSON() implementations.
     * @param keys keys The list of keys required in the final JSON representation. Validation will be done.
     * @return the validated JSON array with objects containing only keys specified in the keys vector.
     */
    static json arrayFromVector(std::vector<T> list, std::vector<std::string> keys) {
        json array = json::array();
        // For each object in the vector, get the JSON using the required keys.
        for (T t: list) {
            array.push_back(Util::getObjectWithKeys<T>(t, keys));
        }
        return array;
    }

    /**
     * @brief successJSON generates success message as a JSON object to send to the front end.
     * @param message success message
     * @return json message object
     * <code>
     * { <br>
     * success: bool true, <br>
     * message: string (message) <br>
     * }
     * </code>
     */
    static std::string getSuccessJsonStr(std::string message){
        json obj = {
            {"success", true},
            {"message", message}
        };
        return obj.dump();
    }

    /**
     * @brief failureJSON generates failure message as a JSON object to send to the front end.
     * @param message failure message
     * @return json message object
     * <code>
     * { <br>
     * success: bool false, <br>
     * message: string (message) <br>
     * }
     * </code>
     */
    static std::string getFailureJsonStr(std::string message){
        json obj = {
            {"success", false},
            {"message", message}
        };
        return obj.dump();
    }

    /**
     * @brief getArtifactEdit Helper function that turns an Edit<Artifact> object into a JSON representation
     * for the front end.
     * @param edit the edit object
     * @return the JSON representation <br>
     * { <br>
     *    id: int, <br>
     *    time: string, <br>
     *    type: string ["Addition", "Edit", or"Deletion"], <br>
     *    category: string "artifact", <br>
     *    approvalStatus: string ["Approved", "Denied", "Under Review"], <br>
     *    reviewer: { <br>
     *        id: int <br>
     *    } <br>
     *    artifact: { <br>
     *        artifact: { <br>
     *            id: int <br>
     *            name: string <br>
     *            description: string <br>
     *            introduction: string <br>
     *            image: string <br>
     *         }, <br>
     *        collectionList: [{id: int, name: string} ... ], <br>
     *        museum: { <br>
     *             id: int, <br>
     *             name: string <br>
     *         } <br>
     *     } <br>
     * } <br>
     */
    static json getArtifactEditJSON(Edit<Artifact> edit) {
        // Get the Edit's JSON.
        json output = Util::getObjectWithKeys<Edit<Artifact>>(edit, {"id", "type", "category", "approvalStatus", "time"});
        // Attach the artifact info.
        output["artifact"]["artifact"] =  Util::getObjectWithKeys<Artifact>(edit.getObject(),
                                                {"id", "name", "description", "introduction", "image"});
        // Attach the museum info.
        Museum m = edit.getObject().getMuseum();
        output["artifact"]["museum"] = Util::getObjectWithKeys<Museum>(m, {"id", "name"});
        // Attach the collection list.
        output["artifact"]["collectionList"] = Util::arrayFromVector<Collection>(edit.getCollectionList(), {"id","name"});
        // Attach the reviewer's username.
        output["reviewer"] = Util::getObjectWithKeys<User>(m.getUser(), {"username"});
        return output;
    }

    /**
     * @brief getCollectionEdit Helper function that turns an Edit<Collection> object into a JSON representation
     * for the front end.
     * @param edit the edit object
     * @return the JSON representation <br>
     * { <br>
     *    id: int, <br>
     *    time: string, <br>
     *    type: string ["Addition", "Edit", or"Deletion"], <br>
     *    category: string "collection", <br>
     *    approvalStatus: string ["Approved", "Rejected", "Under Review"], <br>
     *    reviewer: { <br>
     *        username: string <br>
     *    } <br>
     *    collection: { <br>
     *        collection: { <br>
     *            id: int <br>
     *            name: string <br>
     *            description: string <br>
     *            introduction: string <br>
     *            image: string <br>
     *         }, <br>
     *        museum: { <br>
     *             id: int, <br>
     *             name: string <br>
     *         } <br>
     *     } <br>
     * } <br>
     */
    static json getCollectionEditJSON(Edit<Collection> edit) {
        // Get the Edit's JSON.
        json output = Util::getObjectWithKeys<Edit<Collection>>(edit, {"id", "type", "category", "approvalStatus", "time"});
        // Attach the collection info.
        output["collection"]["collection"] =  Util::getObjectWithKeys<Collection>(edit.getObject(),
                                                {"id", "name", "description", "introduction", "image"});
        // Attach the museum info.
        Museum m = edit.getObject().getMuseum();
        output["collection"]["museum"] = Util::getObjectWithKeys<Museum>(m, {"id", "name"});
        // Attach the reviewer's username
        output["reviewer"] = Util::getObjectWithKeys<User>(m.getUser(), {"username"});
        return output;
    }


    /**
     * @brief sendEmail Sends an email.
     * @param to The email to send to.
     * @param subject The subject of the email.
     * @param body THe body of the email.
     */
    static void sendEmail(std::string to, std::string subject, std::string body)
    {
        json email = {
            {"to", to},
            {"subject", subject},
            {"body", body}
        };
        // Create an http_client to send an HTTP request.
        web::http::client::http_client client("https://prod-111.westus.logic.azure.com:443"
                                              "/workflows/b1743f93b23745e3a13a7a314c8ad913"
                                              "/triggers/manual/paths/invoke?api-version=2"
                                              "016-06-01&sp=%2Ftriggers%2Fmanual%2Frun&sv="
                                              "1.0&sig=guSYBmZIX7yxSkLH9d7fkeIaff7-GhQsjVVyzLVK3_U");
        std::string message = email.dump();
        // Using a POST request, send the email to that url.
        web::http::http_request request(web::http::methods::POST);
        request.headers().add("Content-Type", "application/json");
        request.set_body(message);
        pplx::task<void> task = client.request(request).then([](web::http::http_response resp){
                resp.extract_string(false).then([](utility::string_t s)
        {
                std::cout << s << '\n';
        });

        });
        try{
            task.get();
        } catch(std::exception &e){
            std::cout << e.what() << '\n';
        }
    }


};

#endif // UTILITY_H
