#ifndef COLLECTION_H
#define COLLECTION_H
#include <string>
#include "museum.h"

class Collection
{
public:
    Collection(std::string name, std::string description, Museum museum) : museum(museum)
    {
        this->name = name;
        this->desc = description;
        this->id = -1;
    }

    Collection(std::string name, std::string description, Museum museum, int collectionID) : museum(museum)
    {
        this->name = name;
        this->desc = description;
        this->id = collectionID;
    }

    Collection(const Collection &collection):museum(collection.getMuseum())
    {
        this->name = collection.getName();
        this->desc = collection.getDescription();
        this->id = collection.getID();
    }

    Collection& operator = (const Collection &collection)
    {
        this->name = collection.getName();
        this->desc = collection.getDescription();
        this->id = collection.getID();
        this->museum = collection.getMuseum();
        return *this;
    }

    ~Collection(){

    }

    std::string getName() const
    {
        return this->name;
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
            {"introduction", "This is " + this->getName()},
            {"image", ""}
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

private:
    std::string name;
    std::string desc;
    int id;
    Museum museum;
};

#endif // COLLECTION_H
