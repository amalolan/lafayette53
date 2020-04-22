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
    std::string getJSON()
    {
        QJsonObject properties;
        properties["name"] = QString::fromStdString(this->name);
        properties["introduction"] = QString::fromStdString("This is "+this->name);
        properties["description"] = QString::fromStdString(this->description);
        properties["id"] = this->museumID;
        properties["userID"] = this->user.getUserID();
        QJsonDocument doc;
        doc.setObject(properties);
        return doc.toJson().toStdString();
    }


    /*
     * json = { name:
     *          introduction:
     *          description:
     *          id:
     *          userID:
     *        }
     */
    json getJson(){
        json output;
        output["name"] = this->name;
        output["introduction"] = "This is "+this->name;
        output["description"] = this->description;
        output["id"] = this->museumID;
        output["userID"] = this->user.getUserID();
        return output;
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
