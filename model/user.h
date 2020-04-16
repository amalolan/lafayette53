#ifndef USER_H
#define USER_H
#include <string>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>

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

    static User* fromJson(std::string jsonString)
    {
        //This method currently does not check if JSON String is correct.
        //That is, it does not check if the string has all fields to create
        //a museum object. Checks will be introducted in a later implementation
        //if neccessary.

        QJsonDocument doc = QJsonDocument::fromJson(QByteArray::fromStdString(jsonString));

        if (doc.isNull() || !doc.isObject())
        {
            return nullptr;
        }

        QJsonObject object = doc.object();
        std::string name = object["username"].toString().toStdString();
        std::string email = object["email"].toString().toStdString();
        std::string password = object["password"].toString().toStdString();
        QJsonValue userID  = object["userID"];

        if(userID.isUndefined()){
            return new User(name, email, password);
        }
        return new User(name, email, password, userID.toInt());
    }

private:
    int userID;
    std::string username;
    std::string email;
    std::string password;

};

#endif // USER_H
