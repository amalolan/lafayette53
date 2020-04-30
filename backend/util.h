#ifndef UTIL_H
#define UTIL_H
#include "../model/ModelException.h"
#include "modelclassext.h"
#include "../nlohmann/json.hpp"
#include <vector>
#include <iostream>

#ifdef NDEBUG
#define assert(condition, message) 0
#else
#define assertMessage(condition, message)\
    (!(condition)) ?\
    (std::cerr << "Assertion failed: (" << #condition << "), "\
    << "function " << __FUNCTION__\
    << ", file " << __FILE__\
    << ", line " << __LINE__ << "."\
    << std::endl << message << std::endl, abort(), 0) : 1
#endif

using json = nlohmann::json;

class LoginException : public std::exception
{
public:
    virtual const char * what () const noexcept override
    {
        return "Login failed. Please check username or password and try again.";
    }
};


class Util
{
public:
    Util();

    static void validateJSON(json &jsonObj, std::vector<std::string> keys) {
        for  (auto key : keys) {
            if  (! jsonObj.contains(key))
                throw json::other_error::create(1, "JSON Schema not valid");
        }
    }

    /**
     * @brief checkLogin give {username="",password=""} object to check if the login is successful.
     * @param user json object
     *  {
     *   "username": string,
     *   "password": string
     * }
     * TODO
     */
    static User checkLogin(json userJSON, ModelClass *model) {
        Util::validateJSON(userJSON, {"username", "password"});
        std::string username  = userJSON["username"];
        User user = model->getUserObject(username);
        if (userJSON["password"] != user.getPassword()) {
            throw LoginException();
        }
        return user;
    }


    template<typename T>
    static json getObjectWithKeys(T t, std::vector<std::string> keys) {
        json tJSON = t.toJSON();
        std::cout<<"Museum object "<<tJSON.dump(3)<<std::endl;
        Util::validateJSON(tJSON, keys);
        json object;
        for (std::string key : keys) {
            object[key] = tJSON[key];
        }
        return object;
    }

    template<typename T>
    static json arrayFromVector(std::vector<T> list, std::vector<std::string> keys) {
        json array = json::array();
        for (T t: list) {
            array.push_back(Util::getObjectWithKeys<T>(t, keys));
        }
        return array;
    }

    /**
     * @brief successJSON generates success message as a json to send to the front end.
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
     * @brief failureJSON generates failure message as a json to send to the front end.
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
