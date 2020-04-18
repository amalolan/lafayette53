#ifndef USER_H
#define USER_H
#include <string>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>
#include <iostream>
class User
{
public:
    User(std::string username, std::string email, std::string password, int id)
    {
        this->userID = id;
        this->username = username;
        this->email = email;
        this->password = password;
    }

    User(std::string username, std::string email, std::string password)
    {
        this->userID = -1;
        this->username = username;
        this->email = email;
        this->password = password;
    }

    ~User()
    {

    }

    std::string getEmail()
    {
        return this->email;
    }

    std::string getName()
    {
        return this->username;
    }

    std::string getPassword(){
        return this->password;
    }

    int getUserID()
    {
        return this->userID;
    }

    void setEmail(std::string newEmail)
    {
        this->email = newEmail;
    }

    void setUserID(int newID)
    {
        this->userID = newID;
    }

    bool indb()
    {
        return this->userID > -1;
    }

    std::string getJSON()
    {
        QJsonObject properties;
        properties["username"] = QString::fromStdString(this->username);
        properties["email"] = QString::fromStdString(this->email);
        properties["userID"] = this->userID;
        properties["password"] = QString::fromStdString(this->password);
        QJsonDocument doc;
        doc.setObject(properties);
        return doc.toJson().toStdString();
    }

private:
    int userID;
    std::string username;
    std::string email;
    std::string password;

};

#endif // USER_H
