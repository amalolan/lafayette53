#ifndef COLLECTION_H
#define COLLECTION_H
#include <string>
#include "museum.h"

/**
 * @brief Represents a Collection object
 */
class Collection
{
public:
    Collection(Museum museum = Museum()):museum(museum)
    {
        this->id = -1;
    }

    /**
     * @brief Ccnstructor for Collection object
     * @param name: Name of object
     * @param desc: Description of object
     * @param museum: Museum object of object
     *
     */
    Collection(std::string name, std::string description, Museum museum) : museum(museum)
    {
        this->name = name;
        this->desc = description;
        this->id = -1;
    }

    /**
     * @brief Ccnstructor for Collection object
     * @param name: Name of object
     * @param description: Description of object
     * @param intro: Introduction of object
     * @param museum: Museum object of object
     *
     */
    Collection(std::string name, std::string description, std::string intro, Museum museum) : museum(museum)
    {
        this->name = name;
        this->desc = description;
        this->intro = intro;
        this->id = -1;
    }

    /**
     * @brief Ccnstructor for Collection object
     * @param name: Name of object
     * @param description: Description of object
     * @param intro: Introduction of object
     * @param museum: Museum object of object
     * @param photo: Image link of object
     *
     */
    Collection(std::string name, std::string description, std::string intro, std::string photo, Museum museum) : museum(museum)
    {
        this->name = name;
        this->desc = description;
        this->intro = intro;
        this->photo = photo;
        this->id = -1;
    }

    /**
     * @brief Ccnstructor for Collection object
     * @param name: Name of object
     * @param description: Description of object
     * @param intro: Introduction of object
     * @param museum: Museum object of object
     * @param photo: Image link of object
     * @param collectionID: ID of object
     *
     */
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

    /**
     * @return Name of object
     *
     */
    std::string getName() const
    {
        return this->name;
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
     * @return Description of object
     *
     */
    std::string getDescription() const
    {
        return this->desc;
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
     * @return Museum object of object
     *
     */
    Museum getMuseum() const
    {
        return this->museum;
    }

    /**
     * @return ID of object
     *
     */
    int getID() const
    {
        return this->id;
    }

    /**
     * @return json object of an object json = {"name": , "description": , "introduction": , "image": , "id": , "museum": }
     *
     */
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

    /**
     * @brief Sets name of object
     * @param newName: Name of object
     */
    void setName(std::string newName)
    {
        this->name = newName;
    }

    /**
     * @brief Sets description of object
     * @param newDesc: Description of object
     */
    void setDescription(std::string newDesc)
    {
        this->desc = newDesc;
    }

    /**
     * @brief Sets intoduction of object
     * @param intro: Indroduction of object
     */
    void setIntro(std::string intro)
    {
        this->intro = intro;
    }

    /**
     * @brief Sets image link of object
     * @param link: Image link of object
     */
    void setPhoto(std::string link)
    {
        this->photo = link;
    }

    /**
     * @brief Sets id of object
     * @param newID: ID of object
     */
    void setID(int newID)
    {
        this->id = newID;
    }

    /**
     * @brief Sets museum of object
     * @param newMuseum: Museum of object
     */
    void setMuseum(Museum newMuseum)
    {
        this->museum = newMuseum;
    }

    /**
     * @return true if object is stored in database
     */
    bool indb() const
    {
        return this->id > -1;
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
