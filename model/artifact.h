#ifndef ARTIFACT_H
#define ARTIFACT_H
<<<<<<< HEAD
#include <vector>
#include <string>
#include <../nlohmann/json.hpp>

using json = nlohmann::json;
class Artifact
{
public:
    Artifact(std::string name, std::string desc = "", std::string intro ="", int id = -1)
    {
        this->name = name;
        this->description = desc;
        this->introduction = intro;
        this->id = id;
    };

    json toJSON()
    {
        json output = {
            {"name", this->getName()},
            {"description", this->getDescription()},
            {"id", this->getID()},
            {"introduction", "This is " + this->getName()},
            {"image", ""}
        };
        return output;
    }

    int getID()
    {
        return this->id;
    }

    void setID(int value)
    {
        this->id = value;
    };
    std::string getDescription()
    {
        return this->description;
    };

    void setDescription(std::string value)
    {
        this->description = value;
    };

    std::string getIntroduction()
    {
        return this->introduction;
    };

    void setIntroduction(std::string value)
    {
        this->introduction = value;
    };

    std::string getName()
    {
        return this->name;
    };

    void setName(std::string value)
    {
        this->name = value;
    };

    std::string getImage()
    {
        return this->image;
    };

    void setImage(std::string value)
    {
        this->image = value;
    };

    void setMuseumID(int value)
    {
        this->museumID = value;
    };

    int getMuseumID()
    {
        return this->museumID;
    };
private:
    int id;
    int museumID;
    std::string description;
    std::string introduction;
    std::string name;
    std::string image;
=======
#include "museum.h"
class Artifact
{
public:
    Artifact(std::string name, std::string desc, std::string intro, Museum museum):museum(museum)
    {
        this->name = name;
        this->desc = desc;
        this->intro = intro;
        this->id = -1;
    }

    Artifact(std::string name, std::string desc, std::string intro, std::string photo, Museum museum, int id):museum(museum)
    {
        this->name = name;
        this->desc = desc;
        this->photo = photo;
        this->intro = intro;
        this->id = id;
    }

    Artifact(std::string name, std::string desc, std::string intro, std::string photo, Museum museum):museum(museum)
    {
        this->name = name;
        this->desc = desc;
        this->intro = intro;
        this->photo = photo;
        this->id = -1;
    }

    ~Artifact()
    {

    }

    std::string getName() const
    {
        return this->name;
    }

    std::string getIntro() const
    {
        return this->intro;
    }

    std::string getPhoto() const
    {
        return this->photo;
    }

    std::string getDescription() const
    {
        return this->desc;
    }

    Museum getMuseum() const
    {
        return this->museum;
    }

    int getID() const
    {
        return this->id;
    }

    json toJSON()
    {
        json output = {
            {"name", this->getName()},
            {"description", this->getDescription()},
            {"introduction", this->getIntro()},
            {"id", this->getID()},
            {"image", this->photo},
            {"museum", this->museum.toJSON()}
        };
        return output;
    }

    void setName(std::string newName)
    {
        this->name = newName;
    }

    void setDescription(std::string newDesc)
    {
        this->desc = newDesc;
    }

    void setPhoto(std::string link)
    {
        this->photo = link;
    }

    void setIntro(std::string intro)
    {
        this->intro = intro;
    }

    void setID(int newID)
    {
        this->id = newID;
    }

    void setMuseum(Museum newMuseum)
    {
        this->museum = newMuseum;
    }

    bool indb(){
        return this->id > -1;
    }

    bool empty(){
        return name == "";
    }

private:
    int id;
    std::string name;
    std::string desc;
    std::string photo;
    std::string intro;
    Museum museum;
>>>>>>> testing
};

#endif // ARTIFACT_H
