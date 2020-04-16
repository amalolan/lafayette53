#ifndef UTIL_H
#define UTIL_H
#include <../model/modelclass.h>
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

        std::string user = jsObj["user"].toString().toStdString();
        std::string name = musObj["name"].toString().toStdString();
        std::string intro = musObj["introduction"].toString().toStdString();
        std::string description = musObj["description"].toString().toStdString();
        std::string id = musObj["id"].toString().toStdString();
        //TODO fix the id and add intro
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

};

#endif // UTILITY_H
