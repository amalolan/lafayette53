#ifndef MUSEUM_H
#define MUSEUM_H
#include <string>
#include "user.h"
#include <../nlohmann/json.hpp>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <iostream>

using json = nlohmann::json;
class Museum
{
public:
    Museum(std::string name, std::string description, User user):user(user)
    {
        this->name = name;
        this->description = description;
        this->museumID = -1;
    }

    Museum(std::string name, std::string description, User user, int museumID):user(user)
    {
        this->name = name;
        this->description = description;
        this->museumID = museumID;
    }

    Museum(const Museum &museum):user(museum.getUser())
    {
        this->name = museum.getName();
        this->description = museum.getDescription();
        this->museumID = museum.getMuseumID();
    }

    Museum& operator = (const Museum &museum)
    {
        this->user = museum.getUser();
        this->name = museum.getName();
        this->description = museum.getDescription();
        this->museumID = museum.getMuseumID();
        return *this;
    }

    ~Museum()
    {

    }

    void setName(std::string name)
    {
        this->name = name;
    }

    void setDescription(std::string desc)
    {
        this->description = desc;
    }

    void setMuseumID(int id)
    {
        this->museumID = id;
    }

    void setUser(User user)
    {
        this->user = user;
    }

    std::string getName() const
    {
        return this->name;
    }

    std::string getDescription() const
    {
        return this->description;
    }

    User getUser() const
    {
        return this->user;
    }

    int getMuseumID() const
    {
        return this->museumID;
    }

    bool indb()
    {
        return this->museumID > -1;
    }

    std::string getUsername()
    {
        return this->user.getName();
    }

    std::string getUserPass()
    {
        return this->user.getPassword();
    }
    json toJSON()
    {
        json museum = {
            {"name", this->getName()},
            {"introduction", "This is " + this->getName()},
            {"description", this->getDescription()},
            {"id", this->getMuseumID()},
            {"userID", this->user.getUserID()},
            {"image", ""}
        };
        return museum;
    }

    bool empty(){
        return name == "" && description == "";
    }
private:
    std::string name;
    std::string description;
    int museumID;
    User user;
};

#endif // MUSEUM_H
