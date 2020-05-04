#ifndef EDIT_H
#define EDIT_H
#include "collection.h"
#include "artifact.h"

template <class T>
class Edit
{
public:
    Edit(Collection object, int kind, User user):user(user), object(object){
        this-> kind = kind;
        this->status = pending;
        this->id = -1;
    }

    Edit(Museum object, int kind, User user):user(user), object(object){
        this-> kind = kind;
        this->status = pending;
        this->id = -1;
    }

    Edit(Collection object, int kind, User user, int status, int id): user(user), object(object){
        this-> kind = kind;
        this->status = status;
        this->id = id;
    }

    Edit(Museum object, int kind, User user, int status, int id): user(user), object(object){
        this-> kind = kind;
        this->status = status;
        this->id = id;
    }

    Edit(Artifact object, int kind, User user, std::vector<Collection> list):user(user), object(object){
        this-> kind = kind;
        this->status = pending;
        this->collection = list;
        this->id = -1;
    }

    Edit(Artifact object, int kind, User user, std::vector<Collection> list, int status, int id):user(user), object(object){
        this-> kind = kind;
        this->status = status;
        this->collection = list;
        this->id = id;
    }

    Edit(const Edit &) = default;
    Edit& operator = (const Edit&) = default;
    Edit& operator=(Edit&&) = default;
    Edit(Edit&&) = default;
    virtual ~Edit()
    {
    }

    friend std::ostream& operator<<(std::ostream &strm, const Edit<T> &m) {
      return strm << "Edit(object : "<< m.object <<", "
                     "kind: "<< m.kind<< ", "
                     "status: " << m.status << ", "
                     "user: " << m.user << ", "
                     "id: " << m.id << ")";
    }

    void setObject(T obj)
    {
        this->object = obj;
    }

    T getObject() const
    {
        return object;
    }

    User getEditor() const
    {
        return user;
    }

    int getKind() const
    {
        return kind;
    }

    int getStatus() const
    {
        return status;
    }

    int getID() const
    {
        return this->id;
    }

    void setID(int id){
        this->id = id;
    }

    bool indb() const
    {
        return this->id > -1;
    }

    std::vector<Collection> getCollectionList() const
    {
        return collection;
    }

    void approveEdit()
    {
        this->status = approve;
    }

    void rejectEdit()
    {
        this->status = reject;
    }

    bool compareList(const std::vector<Collection> list) const
    {
        for (Collection col : list)
        {
            if (std::find(collection.begin(), collection.end(), col)==collection.end())
            {
                return false;
            }
        }
        return true;
    }

    std::string toString() const
    {
        return "Edit(object : " + this->object.toString()+", "
                "kind: " + std::to_string(this->kind) + ", "
                "status: " + std::to_string(this->status) + ", "
                "user: " + this->user.toString() + ", "
                "id: " + std::to_string(this->id) + ")";
    }

    json toJSON() const
    {
        json output;

        //artifact/collection
        if (std::is_same<T, Artifact>::value)
        {
            output["artifact"] = this->object.toJSON();
            output["category"] = "artifact";
        }
        if (std::is_same<T, Collection>::value)
        {
            output["collection"] = this->object.toJSON();
            output["category"] = "collection";
        }
        //id
        output["id"] = this->id;
        //status
        if (status == 1)
        {
            output["approvalStatus"] = "Approved";
        } else if (status == 0)
        {
            output["approvalStatus"] = "Under review";
        } else if (status == -1)
        {
            output["approvalStatus"] = "Rejected";
        }
        //kind
        if (kind == 1)
        {
            output["type"] = "Addition";
        } else if (kind == 0)
        {
            output["type"] = "Edit";
        } else if (kind == -1)
        {
            output["type"] = "Deletion";
        }

        return output;
    }

    const static int add = 1;
    const static int del = -1;
    const static int edit = 0;
    const static int approve = 1;
    const static int reject = -1;
    const static int pending = 0;

private:
    User user;
    T object;
    int status;
    int kind;
    int id;
    std::vector<Collection> collection;
};
template <class T>
const int Edit<T>::add;
template <class T>
const int Edit<T>::del;
template <class T>
const int Edit<T>::edit;
template <class T>
const int Edit<T>::approve;
template <class T>
const int Edit<T>::reject;
template <class T>
const int Edit<T>::pending;

template <class T>
inline bool operator==(const Edit<T>& lhs, const Edit<T>& rhs) {
        return ((lhs.getObject() == rhs.getObject())
                && (lhs.getID() == rhs.getID())
                && (lhs.getKind() == rhs.getKind())
                && (lhs.getStatus() == rhs.getStatus())
                && (lhs.getEditor() == rhs.getEditor())
                && (lhs.compareList(rhs.getCollectionList())));
}

template <class T>
inline bool operator!=(const Edit<T>& lhs, const Edit<T>& rhs) {
    return ((lhs.getObject() != rhs.getObject())
            || (lhs.getID() != rhs.getID())
            || (lhs.getKind() != rhs.getKind())
            || (lhs.getStatus() != rhs.getStatus())
            || (lhs.getEditor() != rhs.getEditor())
            || !(lhs.compareList(rhs.getCollectionList())));
}
#endif // EDIT_H
