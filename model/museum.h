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

    Museum(std::string name, std::string description, std::string intro, User user):user(user)
    {
        this->name = name;
        this->description = description;
        this->intro = intro;
        this->museumID = -1;
    }

    Museum(std::string name, std::string description, std::string intro, User user, int museumID):user(user)
    {
        this->name = name;
        this->description = description;
        this->intro = intro;
        this->museumID = museumID;
    }

    Museum(const Museum &museum):user(museum.getUser())
    {
        this->name = museum.getName();
        this->description = museum.getDescription();
        this->museumID = museum.getMuseumID();
        this->intro = museum.getIntro();
    }

    Museum& operator = (const Museum &museum)
    {
        this->user = museum.getUser();
        this->name = museum.getName();
        this->description = museum.getDescription();
        this->museumID = museum.getMuseumID();
        this->intro = museum.getIntro();
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

    void setIntro(std::string intro)
    {
        this->intro = intro;
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

    std::string getIntro() const
    {
        return this->intro;
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
            {"introduction", this->getIntro()},
            {"description", this->getDescription()},
            {"id", this->getMuseumID()},
            {"userID", this->user.getUserID()},
            {"image", ""}
        };
        return museum;
    }

    bool empty(){
        return name == "" && description == "" && intro == "";
    }
private:
    std::string name;
    std::string description;
    std::string intro;
    int museumID;
    User user;
};

#endif // MUSEUM_H
