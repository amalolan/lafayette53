#ifndef USER_H
#define USER_H
#include <string>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>
#include <iostream>
#include <../nlohmann/json.hpp>
/**
 * @brief Represents a User object
 */
using json = nlohmann::json;
class User
{
public:
    User()
    {
        this->userID = -1;
    }

    /**
     * @brief Ccnstructor for User object
     * @param username: Name of object
     * @param email: Email of object
     * @param password: Hashed password of object
     * @param id: ID of object
     *
     */
    User(std::string username, std::string email, std::string password, int id)
    {
        this->userID = id;
        this->username = username;
        this->email = email;
        this->password = password;
    }

    /**
     * @brief Ccnstructor for User object
     * @param username: Name of object
     * @param email: Email of object
     * @param password: Hashed password of object
     *
     */
    User(std::string username, std::string email, std::string password)
    {
        this->userID = -1;
        this->username = username;
        this->email = email;
        this->password = password;
    }

    User(const User &user) = default;
    User& operator = (const User&) = default;
    User& operator=(User&&) = default;
    User(User&&) = default;
    virtual ~User()
    {
    }

    friend std::ostream& operator<<(std::ostream &strm, const User &u) {
      return strm << "User(name : "<< u.username <<", "
                     "id: " << u.userID << ", "
                     "email: "<< u.email<< ")";
    }

    /**
     * @return Email of object
     */
    std::string getEmail() const
    {
        return this->email;
    }

    /**
     * @return Name of object
     */
    std::string getName() const
    {
        return this->username;
    }

    /**
     * @return Hashed password of object
     */
    std::string getPassword() const
    {
        return this->password;
    }

    /**
     * @return Email of object
     */
    int getUserID() const
    {
        return this->userID;
    }


    /**
     * @brief Sets email of object
     * @param newWEmail: Email of object
     */
    void setEmail(std::string newEmail)
    {
        this->email = newEmail;
    }

    /**
     * @brief Sets id of object
     * @param newID: ID of object
     */
    void setUserID(int newID)
    {
        this->userID = newID;
    }

    /**
     * @brief Sets password of object
     * @param newPassword: Hashed password of object
     */
    void setPassword(std::string newPassword){
        this->password = newPassword;
    }

    /**
     * @return true if object is stored in database
     */
    bool indb() const
    {
        return this->userID > -1;
    }


    /**
     * @return json object of an object json = {"username": , "email": , "password": , "id": , "userID": }
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

    /**
     * @return true if object has no name
     */
    bool empty(){
        return username == "" && email == "" && password == "";
    }

    /**
     * @return String represnetation of object
     */
    std::string toString() const
    {
        return "Public(name : "+ this->username + ", "
                       "email: " + this->email + ", "
                       "id: " + std::to_string(this->userID) + ")";
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
