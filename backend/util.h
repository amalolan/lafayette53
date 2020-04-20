#ifndef UTIL_H
#define UTIL_H
#include "../model/modelclass.h"
#include "backendexception.h"
#include "../nlohmann/json.hpp"
//#include <cpprest/http_client.h>
//#include <cpprest/filestream.h>
//#include <cpprest/json.h>
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
class Util
{
public:
    Util();

    /**
     * @brief parseMuseumJSON Parses a json string and returns a museum object.
     * @param jsonStr JSON String
     * {
     *   museum:  {
     *      "name": string,
     *      "introduction": string,
     *      "description": string,
     *      "id": string
     *    },
     *   user: {
     *       "username": string,
     *       "password": string
     *    }
     * }
     * @return Museum object
     */
    static Museum* parseMuseumJsonStr(std::string jsonStr){
        //TODO :add userID by getting user from the database. Should be  done in constructor of userID @Sena
        json obj = json::parse(jsonStr);
//        std::string keys[] = {"username", "password", "name", "description"};
//        for (auto key : keys) {
//            assertMessage (obj.contains(key), "Invalid Json Schema");
//        }
        User u(obj["user"]["username"], "", obj["user"]["password"]);
        return new Museum(obj["museum"]["name"], obj["museum"]["description"], u);
    }

    /**
     * @brief parseUserJSON
     * @param json User json object
     * {
     *   "username": string,
     *   "email": string,
     *   "password": string
     * }
     * @return
     */
    static User* parseUserJsonStr(std::string jsonStr){
            json obj = json::parse(jsonStr);
            std::string keys[] = {"username", "email", "password"};
            for (auto key : keys) {
//              if (key == "email" && registered) continue;
              if (! obj.contains(key))
                  throw BackendException("Invalid Json Schema");
            }
            return new User(obj["username"],obj["email"],obj["password"]);


    }

    /**
     * @brief checkLogin give {username="",password=""} object to check if the login is successful.
     * @param user json object
     *  {
     *   "username": string,
     *   "password": string
     * }
     * @return true/false if login is successful or not
     * TODO
     */
    static bool checkLogin(std::string userJsonStr){
        try{
            User *u = parseUserJsonStr(userJsonStr);
            std::cout << u->getName() << " " << u->getPassword() << '\n';
//            std::string dataPass = T::getPasswordHash(u->getName());
        } catch(std::exception &e){

            std::cout << "user could not be found" << std::endl;
            return false;
        }
        return false;
    }

    /**
     * @brief parsePassword
     * @param jsonStr
     * {
     *   "username": string,
     *   "email": string,
     *   "password": string
     * }
     * @return string
     */
    static std::string parsePassword(std::string jsonStr){
        json obj = json::parse(jsonStr);
        assertMessage(obj.contains("password"), "Invalid Json Schema");
        return obj["password"];
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
            {"success", "true"},
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
            {"success", "false"},
            {"message", message}
        };
        return obj.dump();
    }
};

#endif // UTILITY_H
