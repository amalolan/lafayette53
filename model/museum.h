#ifndef MUSEUM_H
#define MUSEUM_H
#include <string>
#include "user.h"
#include <../nlohmann/json.hpp>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <iostream>

/**
 * @brief Represents a Museum object
 */
using json = nlohmann::json;
class Museum
{
public:

    Museum(User user = User()):user(user)
    {
        this->museumID = -1;
    }

    /**
     * @brief Ccnstructor for Museum object
     * @param name: Name of object
     * @param description: Description of object
     * @param user: User object of object
     *
     */
    Museum(std::string name, std::string description, User user):user(user)
    {
        this->name = name;
        this->description = description;
        this->museumID = -1;
    }

    /**
     * @brief Ccnstructor for Museum object
     * @param name: Name of object
     * @param description: Description of object
     * @param intro: Introduction of object
     * @param user: User object of object
     *
     */
    Museum(std::string name, std::string description, std::string intro, User user):user(user)
    {
        this->name = name;
        this->description = description;
        this->intro = intro;
        this->museumID = -1;
    }

    /**
     * @brief Ccnstructor for Museum object
     * @param name: Name of object
     * @param description: Description of object
     * @param intro: Introduction of object
     * @param photo: Image link of object
     * @param user: User object of object
     *
     */
    Museum(std::string name, std::string description, std::string intro, std::string photo, User user):user(user)
    {
        this->name = name;
        this->description = description;
        this->intro = intro;
        this->photo = photo;
        this->museumID = -1;
    }

    /**
     * @brief Ccnstructor for Museum object
     * @param name: Name of object
     * @param description: Description of object
     * @param intro: Introduction of object
     * @param photo: Image link of object
     * @param user: User object of object
     * @param museumID: ID of object
     *
     */
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

    /**
     * @brief Sets name of object
     * @param name: Name of object
     */
    void setName(std::string name)
    {
        this->name = name;
    }

    /**
     * @brief Sets image link of object
     * @param photo: Image link of object
     */
    void setPhoto(std::string photo)
    {
        this->photo = photo;
    }

    /**
     * @brief Sets description of object
     * @param desc: Description of object
     */
    void setDescription(std::string desc)
    {
        this->description = desc;
    }

    /**
     * @brief Sets introduction of object
     * @param intro: Introduction of object
     */
    void setIntro(std::string intro)
    {
        this->intro = intro;
    }

    /**
     * @brief Sets ID of object
     * @param id: ID of object
     */
    void setMuseumID(int id)
    {
        this->museumID = id;
    }

    /**
     * @brief Sets User object of object
     * @param user: User of object
     */
    void setUser(User user)
    {
        this->user = user;
    }

    /**
     * @return Name of object
     *
     */
    std::string getName() const
    {
        return this->name;
    }


    std::string getDescription() const
    {
        return this->description;
    }

    /**
     * @return Introduction of object
     *
     */
    std::string getIntro() const
    {
        return this->intro;
    }

    /**
     * @return Image link of object
     *
     */
    std::string getPhoto() const
    {
        return this->photo;
    }

    /**
     * @return User object of object
     *
     */
    User getUser() const
    {
        return this->user;
    }

    /**
     * @return ID of object
     *
     */
    int getMuseumID() const
    {
        return this->museumID;
    }

    /**
     * @return true if object is stored in database
     */
    bool indb() const
    {
        return this->museumID > -1;
    }

    /**
     * @return username of Museum's User
     */
    std::string getUsername()
    {
        return this->user.getName();
    }

    /**
     * @return Hashed password of Museum's User
     */
    std::string getUserPass()
    {
        return this->user.getPassword();
    }

    /**
     * @return json object of an object json = {"name": , "description": , "introduction": , "image": , "id": , "userID": }
     *
     */
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


    /**
     * @return true if object has no name
     */
    bool empty() const
    {
        return name == "";
    }

    /**
     * @return String representation of object
     */
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
