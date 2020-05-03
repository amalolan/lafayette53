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
    Museum(User user = User()):user(user)
    {
        this->museumID = -1;
    }

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

    Museum(std::string name, std::string description, std::string intro, std::string photo, User user):user(user)
    {
        this->name = name;
        this->description = description;
        this->intro = intro;
        this->photo = photo;
        this->museumID = -1;
    }

    Museum(std::string name, std::string description, std::string intro, std::string photo, User user, int museumID):user(user)
    {
        this->name = name;
        this->description = description;
        this->intro = intro;
        this->photo = photo;
        this->museumID = museumID;
    }

    Museum(const Museum &) = default;
    Museum& operator = (const Museum&) = default;
    Museum& operator=(Museum&&) = default;
    Museum(Museum&&) = default;
    virtual ~Museum()
    {
    }

    friend std::ostream& operator<<(std::ostream &strm, const Museum &m) {
      return strm << "Museum(name : "<< m.name <<", "
                     "introduction: "<< m.intro<< ", "
                     "description: " << m.description << ", "
                     "id: " << m.museumID << ", "
                     "user: " << m.user << ", "
                     "photoURL: " << m.photo << ")";
    }

    void setName(std::string name)
    {
        this->name = name;
    }

    void setPhoto(std::string photo)
    {
        this->photo = photo;
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

    std::string getPhoto() const
    {
        return this->photo;
    }

    User getUser() const
    {
        return this->user;
    }

    int getMuseumID() const
    {
        return this->museumID;
    }

    bool indb() const
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

    json toJSON() const
    {
        json museum = {
            {"name", this->getName()},
            {"introduction", this->getIntro()},
            {"description", this->getDescription()},
            {"id", this->getMuseumID()},
            {"userID", this->user.getUserID()},
            {"image", this->getPhoto()}
        };
        return museum;
    }

    bool empty() const
    {
        return name == "";
    }

    std::string toString() const
    {
        return "Museum(name : "+ this->name + ", "
                        "introduction: " + this->intro + ", "
                        "description: " + this->description + ", "
                        "user: " + this->user.toString() + ", "
                        "photoURL: " + this->photo + ")";
    }

private:
    std::string name;
    std::string description;
    std::string intro;
    std::string photo;
    int museumID;
    User user;
};

inline bool operator==(const Museum& lhs, const Museum& rhs) {
        return ((lhs.getName() == rhs.getName())
                && (lhs.getDescription() == rhs.getDescription())
                && (lhs.getIntro() == rhs.getIntro())
                && (lhs.getPhoto() == rhs.getPhoto())
                && (lhs.getMuseumID() == rhs.getMuseumID())
                && (lhs.getUser() == rhs.getUser()));
}

inline bool operator!=(const Museum& lhs, const Museum& rhs) {
    return ((lhs.getName() != rhs.getName())
            || (lhs.getDescription() != rhs.getDescription())
            || (lhs.getIntro() != rhs.getIntro())
            || (lhs.getPhoto() != rhs.getPhoto())
            || (lhs.getMuseumID() != rhs.getMuseumID())
            || (lhs.getUser() != rhs.getUser()));
}

#endif // MUSEUM_H
