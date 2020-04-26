#ifndef ARTIFACT_H
#define ARTIFACT_H
#include "museum.h"
class Artifact
{
public:
    Artifact(std::string name, std::string desc, std::string photo, Museum museum):museum(museum)
    {
        this->name = name;
        this->desc = desc;
        this->photo = photo;
        this->id = -1;
    }

    Artifact(std::string name, std::string desc, std::string photo, Museum museum, int id):museum(museum)
    {
        this->name = name;
        this->desc = desc;
        this->photo = photo;
        this->id = id;
    }

    Artifact(std::string name, std::string desc, Museum museum):museum(museum)
    {
        this->name = name;
        this->desc = desc;
        this->photo = "";
        this->id = -1;
    }

    ~Artifact()
    {

    }

    std::string getName() const
    {
        return this->name;
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
    Museum museum;
};

#endif // ARTIFACT_H
