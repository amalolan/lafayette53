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
        //TODO id is not necessary
        QJsonObject userObj = jsObj["user"].toObject();
        //TODO create user object and add it to the museum object
        std::string username = userObj["username"].toString().toStdString();
        std::string password = userObj["password"].toString().toStdString();

        return new Museum(name,description,1);
    }

    static User* parseUserJSON(std::string json){
        QString s = QString::fromStdString(json);

        QJsonDocument jsDoc = QJsonDocument::fromJson(s.toUtf8());

        QJsonObject jsobj = jsDoc.object();

        std::string username = jsobj["username"].toString().toStdString();
        std::string email = jsobj["email"].toString().toStdString();
        std::string password = jsobj["password"].toString().toStdString();
        //TODO fix constructor to add password
        return new User(username,email);
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
};

#endif // UTILITY_H
