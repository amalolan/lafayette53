#ifndef ARTIFACT_H
#define ARTIFACT_H
#include "museum.h"
class Artifact
{
public:
    Artifact(Museum museum = Museum()):museum(museum)
    {
        this->id = -1;
    }

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

    Artifact(const Artifact &) = default;
    Artifact& operator = (const Artifact&) = default;
    Artifact& operator=(Artifact&&) = default;
    Artifact(Artifact&&) = default;
    virtual ~Artifact()
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
