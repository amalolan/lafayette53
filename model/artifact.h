#ifndef ARTIFACT_H
#define ARTIFACT_H
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
};

#endif // ARTIFACT_H
