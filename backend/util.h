#ifndef UTIL_H
#define UTIL_H
#include "../model/modelclass.h"
#include "../model/LafException.h"
#include <QJsonObject>
#include <iostream>
class util
{
public:
    util();
    static Museum* parseMuseumJSON(std::string json){
        QString s = QString::fromStdString(json);

        QJsonDocument jsDoc = QJsonDocument::fromJson(s.toUtf8());

        QJsonObject jsObj = jsDoc.object();

        QJsonObject musObj = jsObj["museum"].toObject();


        std::string name = musObj["name"].toString().toStdString();
        std::string intro = musObj["introduction"].toString().toStdString();
        std::string description = musObj["description"].toString().toStdString();
        std::string id = musObj["id"].toString().toStdString();
        QJsonObject userObj = jsObj["user"].toObject();

        std::string username = userObj["username"].toString().toStdString();
        std::string password = userObj["password"].toString().toStdString();
        std::string email = userObj["email"].toString().toStdString();
        //std::string id = ModelClass::
        //TODO add userID by getting user from the database.
        User u(username,email,password,-1);
        return new Museum(name,description,u);
    }

    static User* parseUserJSON(std::string json){
        QString s = QString::fromStdString(json);

        QJsonDocument jsDoc = QJsonDocument::fromJson(s.toUtf8());

        QJsonObject jsobj = jsDoc.object();

        std::string username = jsobj["username"].toString().toStdString();
        std::string email = jsobj["email"].toString().toStdString();
        std::string password = jsobj["password"].toString().toStdString();
        //TODO fix constructor to add password
        return new User(username,email,password);
    }

    static bool checkLogin(User& u){
        try{
            std::string s = ModelClass::getPasswordHash(u.getName());
            //TODO check if passwords matches and return true
        } catch(LafException &e){

            std::cout << "user could not be found" << std::endl;
            return false;
        }
    }
    /**
     * @brief successJSON generates success message for the server
     * @return json object
     */
    static std::string successJSON(std::string message){
        QJsonObject jsObj;
        jsObj["success"] = true;
        jsObj["message"] = QString::fromStdString(message);

        QJsonDocument doc;
        doc.setObject(jsObj);

        return doc.toJson().toStdString();
    }

    /**
     * @brief failureJSON generates failure message with given message
     * @param message message for the failure
     * @return json object
     */
    static std::string failureJSON(std::string message){
        QJsonObject jsObj;
        jsObj["success"] = false;
        jsObj["message"] = QString::fromStdString(message);

        QJsonDocument doc;
        doc.setObject(jsObj);

        return doc.toJson().toStdString();
    }
    static std::string parsePassword(std::string json){
        QString s = QString::fromStdString(json);

        QJsonDocument jsDoc = QJsonDocument::fromJson(s.toUtf8());

        QJsonObject jsobj = jsDoc.object();

        return jsobj["password"].toString().toStdString();
    }

};

#endif // UTILITY_H
