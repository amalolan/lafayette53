#ifndef ARTIFACT_H
#define ARTIFACT_H
#include "museum.h"

/**
 * @brief Represents an Artifact object
 */
class Artifact
{
public:
    Artifact(Museum museum = Museum()):museum(museum)
    {
        this->id = -1;
    }


    /**
     * @brief Ccnstructor for Artifact object
     * @param name: Name of object
     * @param desc: Description of object
     * @param intro: Introduction of object
     * @param museum: Museum object of object
     *
     */
    Artifact(std::string name, std::string desc, std::string intro, Museum museum):museum(museum)
    {
        this->name = name;
        this->desc = desc;
        this->intro = intro;
        this->id = -1;
    }

    /**
     * @brief Ccnstructor for Artifact object
     * @param name: Name of object
     * @param desc: Description of object
     * @param intro: Introduction of object
     * @param museum: Museum object of object
     * @param photo: Image link of object
     * @param id: ID of object
     *
     */
    Artifact(std::string name, std::string desc, std::string intro, std::string photo, Museum museum, int id):museum(museum)
    {
        this->name = name;
        this->desc = desc;
        this->photo = photo;
        this->intro = intro;
        this->id = id;
    }

    /**
     * @brief Ccnstructor for Artifact object
     * @param name: Name of object
     * @param desc: Description of object
     * @param intro: Introduction of object
     * @param museum: Museum object of object
     * @param photo: Image link of object
     *
     */
    Artifact(std::string name, std::string desc, std::string intro, std::string photo, Museum museum):museum(museum)
    {
        this->name = name;
        this->desc = desc;
        this->intro = intro;
        this->photo = photo;
        this->id = -1;
    }

    Artifact(const Artifact &) = default;
    Artifact& operator = (const Artifact&) = default;
    Artifact& operator=(Artifact&&) = default;
    Artifact(Artifact&&) = default;
    virtual ~Artifact()
    {
    }

    friend std::ostream& operator<<(std::ostream &strm, const Artifact &m) {
      return strm << "Artifact(name : "<< m.name <<", "
                     "introduction: "<< m.intro<< ", "
                     "description: " << m.desc << ", "
                     "id: " << m.id << ", "
                     "museum: " << m.museum << ", "
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
     * @return Description of object
     *
     */
    std::string getDescription() const
    {
        return this->desc;
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
            {"introduction", this->getIntro()},
            {"id", this->getID()},
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
     * @brief Sets image link of object
     * @param link: Image link of object
     */
    void setPhoto(std::string link)
    {
        this->photo = link;
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
        return "Artifact(name : "+ this->name + ", "
                        "id: " + std::to_string(this->id) + ", "
                        "introduction: " + this->intro + ", "
                        "description: " + this->desc + ", "
                        "museum: " + this->museum.toString() + ", "
                        "photoURL: " + this->photo + ")";
    }

private:
    int id;
    std::string name;
    std::string desc;
    std::string photo;
    std::string intro;
    Museum museum;
};

inline bool operator==(const Artifact& lhs, const Artifact& rhs) {
        return ((lhs.getName() == rhs.getName())
                && (lhs.getDescription() == rhs.getDescription())
                && (lhs.getIntro() == rhs.getIntro())
                && (lhs.getPhoto() == rhs.getPhoto())
                && (lhs.getID() == rhs.getID())
                && (lhs.getMuseum() == rhs.getMuseum()));
}

inline bool operator!=(const Artifact& lhs, const Artifact& rhs) {
    return ((lhs.getName() != rhs.getName())
            || (lhs.getDescription() != rhs.getDescription())
            || (lhs.getIntro() != rhs.getIntro())
            || (lhs.getPhoto() != rhs.getPhoto())
            || (lhs.getID() != rhs.getID())
            || (lhs.getMuseum() != rhs.getMuseum()));
}

#endif // ARTIFACT_H
