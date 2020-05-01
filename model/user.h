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

    virtual ~User()
    {

    }

    friend std::ostream& operator<<(std::ostream &strm, const User &u) {
      return strm << "User(name : "<< u.username <<", "
                     "email: "<< u.email<< ")";
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

    bool indb() const
    {
        return this->userID > -1;
    }


    /*
     * json = { username:
     *          email:
     *          userID:
     *          password:
     *        }
     *
     */
    virtual json toJSON() const
    {
        json output  =  {
            {"username", this->getName()},
            {"email", this->getEmail()},
            {"id", this->getUserID()},
            {"passowrd", this->getPassword()}
        };
        return output;
    }

    bool empty(){
        return username == "" && email == "" && password == "";
    }
private:
    int userID;
    std::string username;
    std::string email;
    std::string password;

};


inline bool operator==(const User& lhs, const User& rhs) {
        return ((lhs.getName() == rhs.getName())
                && (lhs.getEmail() == rhs.getEmail())
                && (lhs.getPassword() == rhs.getPassword())
                && (lhs.getUserID() == rhs.getUserID()));
}

inline bool operator!=(const User& lhs, const User& rhs) {
        return ((lhs.getName() != rhs.getName())
                || (lhs.getEmail() != rhs.getEmail())
                || (lhs.getPassword() != rhs.getPassword())
                || (lhs.getUserID() != rhs.getUserID()));
}

#endif // USER_H
