#ifndef UTIL_H
#define UTIL_H
#include <vector>
#include <iostream>
#include "../nlohmann/json.hpp"

#include "../model/ModelException.h"
#include "modelclassext.h"

using json = nlohmann::json;

/**
 * @brief The LoginException class If the user has invalid login credentials but performs POST requests requiring log ins,
 * this exception is raised.
 */
class LoginException : public std::exception
{

public:
    /**
     * @brief what Describes the error
     * @return The description of the Login Error.
     */
    virtual const char * what () const noexcept override
    {
        return "Login failed. Please check username or password and try again.";
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
        Util::validateJSON(userJSON, {"username", "password"});
        std::string username  = userJSON["username"];
        User user = model->getUserObject(username);
        std::string password = user.getPassword();
        if (userJSON["password"] != password) {
            throw LoginException();
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
        for (T t: list) {
            array.push_back(Util::getObjectWithKeys<T>(t, keys));
        }
        return array;
    }

    /**
     * @brief successJSON generates success message as a JSON object to send to the front end.
     * @param message success message
     * @return json message object
     * {
     *  "success": bool true,
     *  "message": string
     * }
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
     * {
     *  "success": bool false,
     *  "message": string
     * }
     */
    static std::string getFailureJsonStr(std::string message){
        json obj = {
            {"success", false},
            {"message", message}
        };
        return obj.dump();
    }

};

#endif // UTILITY_H
