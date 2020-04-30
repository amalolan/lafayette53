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

    Collection(std::string name, std::string description, std::string intro, Museum museum) : museum(museum)
    {
        this->name = name;
        this->desc = description;
        this->intro = intro;
        this->id = -1;
    }

    Collection(std::string name, std::string description, std::string intro, std::string photo, Museum museum) : museum(museum)
    {
        this->name = name;
        this->desc = description;
        this->intro = intro;
        this->photo = photo;
        this->id = -1;
    }

    Collection(std::string name, std::string description, std::string intro, std::string photo, Museum museum, int collectionID) : museum(museum)
    {
        this->name = name;
        this->desc = description;
        this->id = collectionID;
        this->photo = photo;
        this->intro = intro;
    }

    Collection(const Collection &collection):museum(collection.getMuseum())
    {
        this->name = collection.getName();
        this->desc = collection.getDescription();
        this->id = collection.getID();
        this->intro = collection.getIntro();
        this->photo = collection.getPhoto();
    }

    Collection& operator = (const Collection &collection)
    {
        this->name = collection.getName();
        this->desc = collection.getDescription();
        this->id = collection.getID();
        this->museum = collection.getMuseum();
        this->intro = collection.getIntro();
        this->photo = collection.getPhoto();
        return *this;
    }

    ~Collection(){

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

    std::string getIntro() const
    {
        return this->intro;
    }

    Museum getMuseum() const
    {
        return this->museum;
    }

    int getID() const
    {
        return this->id;
    }

    json toJSON() const
    {
        json output = {
            {"name", this->getName()},
            {"description", this->getDescription()},
            {"id", this->getID()},
            {"introduction", this->getIntro()},
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

    void setIntro(std::string intro)
    {
        this->intro = intro;
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

    bool indb() const
    {
        return this->id > -1;
    }

    bool empty() const
    {
        return name == "";
    }

private:
    std::string name;
    std::string desc;
    std::string intro;
    std::string photo;
    int id;
    Museum museum;
};

inline bool operator==(const Collection& lhs, const Collection& rhs) {
        return ((lhs.getName() == rhs.getName())
                && (lhs.getDescription() == rhs.getDescription())
                && (lhs.getIntro() == rhs.getIntro())
                && (lhs.getPhoto() == rhs.getPhoto())
                && (lhs.getID() == rhs.getID())
                && (lhs.getMuseum() == rhs.getMuseum()));
}

inline bool operator!=(const Collection& lhs, const Collection& rhs) {
    return ((lhs.getName() != rhs.getName())
            || (lhs.getDescription() != rhs.getDescription())
            || (lhs.getIntro() != rhs.getIntro())
            || (lhs.getPhoto() != rhs.getPhoto())
            || (lhs.getID() != rhs.getID())
            || (lhs.getMuseum() != rhs.getMuseum()));
}
#endif // COLLECTION_H
