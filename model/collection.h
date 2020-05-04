#ifndef COLLECTION_H
#define COLLECTION_H
#include <string>
#include "museum.h"

/*
 * Represents a Collection object
 */
class Collection
{
public:
    Collection(Museum museum = Museum()):museum(museum)
    {
        this->id = -1;
    }

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

    Collection(const Collection &) = default;
    Collection& operator = (const Collection&) = default;
    Collection& operator=(Collection&&) = default;
    Collection(Collection&&) = default;
    virtual ~Collection()
    {
    }

    friend std::ostream& operator<<(std::ostream &strm, const Collection &m) {
      return strm << "Collection(name : "<< m.name <<", "
                     "introduction: "<< m.intro<< ", "
                     "description: " << m.desc << ", "
                     "museum: " << m.museum << ", "
                     "id: " << m.id << ", "
                     "photoURL: " << m.photo << ")";
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

    std::string toString() const
    {
        return "Collection(name : "+ this->name + ", "
                        "id: " + std::to_string(this->id) + ", "
                        "introduction: " + this->intro + ", "
                        "description: " + this->desc + ", "
                        "museum: " + this->museum.toString() + ", "
                        "photoURL: " + this->photo + ")";
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
