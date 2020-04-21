#ifndef USER_H
#define USER_H
#include <string>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>
#include <iostream>
#include <../nlohmann/json.hpp>

using json = nlohmann::json;
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

    User(const User &user)
    {
        this->userID = user.getUserID();
        this->username = user.getName();
        this->email = user.getEmail();
        this->password = user.getPassword();

    }

    User& operator = (const User &user)
    {
        this->userID = user.getUserID();
        this->username = user.getName();
        this->email = user.getEmail();
        this->password = user.getPassword();
        return *this;
    }

    ~User()
    {

    }

    std::string getEmail() const
    {
        return this->email;
    }

    std::string getName() const
    {
        return this->username;
    }

    std::string getPassword() const
    {
        return this->password;
    }

    int getUserID() const
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

    void setPassword(std::string newPassword){
        this->password = newPassword;
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

    /*
     * json = { username:
     *          email:
     *          userID:
     *          password:
     *        }
     *
     */
    json getJson(){
        json output;
        output["username"] = this->username;
        output["email"] = this->email;
        output["userID"] = this->userID;
        output["password"] = this->password;
        return output;
    }

private:
    int userID;
    std::string username;
    std::string email;
    std::string password;

};

#endif // USER_H
