#ifndef MUSEUM_H
#define MUSEUM_H
#include <string>
#include "user.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

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

    std::string getJSON()
    {
        QJsonObject properties;
        properties["name"] = QString::fromStdString(this->name);
        properties["introduction"] = QString::fromStdString("This is "+this->name);
        properties["description"] = QString::fromStdString(this->description);
        properties["museumID"] = this->museumID;
        properties["userID"] = this->user.getUserID();
        QJsonDocument doc;
        doc.setObject(properties);
        return doc.toJson().toStdString();
    }
private:
    std::string name;
    std::string description;
    int museumID;
    User user;
};

#endif // MUSEUM_H
