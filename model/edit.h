#ifndef EDIT_H
#define EDIT_H
#include "collection.h"
#include "artifact.h"
/**
 * @brief Represents an Edit object
 *
 * Object contained in this class contains the edited form of the object.
 * Original object is retireved using edited object's ID.
 * User object contains the user who made the edit.
 */
template <class T>
class Edit
{
public:
    /**
     * @brief Constructor of Edit object
     * @param object: Object to be edited
     * @param kind: Kind of object
     * @param user: User of object
     */
    Edit(Collection object, int kind, User user):user(user), object(object){
        this-> kind = kind;
        this->status = pending;
        this->id = -1;
    }

    /**
     * @brief Constructor of Edit object
     * @param object: Object to be edited
     * @param kind: Kind of object
     * @param user: User of object
     */
    Edit(Museum object, int kind, User user):user(user), object(object){
        this-> kind = kind;
        this->status = pending;
        this->id = -1;
    }

    /**
     * @brief Constructor of Edit object
     * @param object: Object to be edited
     * @param kind: Kind of object
     * @param user: User of object
     * @param id: ID of object
     */
    Edit(Collection object, int kind, User user, int status, int id): user(user), object(object){
        this-> kind = kind;
        this->status = status;
        this->id = id;
    }

    /**
     * @brief Constructor of Edit object
     * @param object: Object to be edited
     * @param kind: Kind of object
     * @param user: User of object
     * @param id: ID of object
     */
    Edit(Museum object, int kind, User user, int status, int id): user(user), object(object){
        this-> kind = kind;
        this->status = status;
        this->id = id;
    }

    /**
     * @brief Constructor of Edit object
     * @param object: Object to be edited
     * @param kind: Kind of object
     * @param user: User of object
     * @param list: List of collections object to be edited belongs to after edit
     */
    Edit(Artifact object, int kind, User user, std::vector<Collection> list):user(user), object(object){
        this-> kind = kind;
        this->status = pending;
        this->collection = list;
        this->id = -1;
    }

    /**
     * @brief Constructor of Edit object
     * @param object: Object to be edited
     * @param kind: Kind of object
     * @param user: User of object
     * @param list: List of collections object to be edited belongs to after edit
     * @param id: ID of object
     */
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

    /**
     * @brief Sets object of edit object
     */
    void setObject(T obj)
    {
        this->object = obj;
    }

    /**
     * @return object of edit object
     */
    T getObject() const
    {
        return object;
    }

    /**
     * @return object of edit object
     */
    User getEditor() const
    {
        return user;
    }

    /**
     * @return kind of edit object. Either Edit<T>::add, Edit<T>::del, Edit<T>::edit
     */
    int getKind() const
    {
        return kind;
    }

    /**
     * @return status of edit object. Either Edit<T>::pending, Edit<T>::approve, Edit<T>::reject
     */
    int getStatus() const
    {
        return status;
    }

    /**
     * @return id of object
     */
    int getID() const
    {
        return this->id;
    }

    /**
     * @return time of object's creation
     */
    std::string getTime() const
    {
        return this->time;
    }

    /**
     * @brief Sets the id of the object
     * @param id: ID of object
     */
    void setID(int id){
        this->id = id;
    }

    /**
     * @brief Sets the time of creation of the object
     * @param t: time of object
     */
    void setTime(std::string t)
    {
        this->time = t;
    }

    /**
     * @return true if the edit object is stored in database
     */
    bool indb() const
    {
        return this->id > -1;
    }

    /**
     * @return the collection list the edited object will belong to after edit is implemented
     */
    std::vector<Collection> getCollectionList() const
    {
        return collection;
    }

    /**
     * @brief Approves an edit
     */
    void approveEdit()
    {
        this->status = approve;
    }

    /**
     * @brief Rejects an edit
     */
    void rejectEdit()
    {
        this->status = reject;
    }

    /**
     * @return True if a collection list contain the same collections as the object's collections
     */
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

    /**
     * @return String representation of object
     */
    std::string toString() const
    {
        return "Edit(object : " + this->object.toString()+", "
                "kind: " + std::to_string(this->kind) + ", "
                "status: " + std::to_string(this->status) + ", "
                "user: " + this->user.toString() + ", "
                "id: " + std::to_string(this->id) + ")";
    }

    /**
     * @return json object of an object json = {"time": , "artifact/collection": , "category": , "id": , "approvalStatus": , "type": }
     *
     */
    json toJSON() const
    {
        json output;
        output["time"] = this->time;
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

    /**
     * @brief int value of Edit<T>::add
     *
     */
    const static int add = 1;

    /**
     * @brief int value of Edit<T>::del
     *
     */
    const static int del = -1;

    /**
     * @brief int value of Edit<T>::edit
     *
     */
    const static int edit = 0;

    /**
     * @brief int value of Edit<T>::approve
     *
     */
    const static int approve = 1;

    /**
     * @brief int value of Edit<T>::reject
     *
     */
    const static int reject = -1;

    /**
     * @brief int value of Edit<T>::pending
     *
     */
    const static int pending = 0;

private:
    User user;
    T object;
    int status;
    int kind;
    int id;
    std::string time;
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
                && (lhs.compareList(rhs.getCollectionList()))
                && (rhs.compareList(lhs.getCollectionList()))
                && (lhs.getTime() == rhs.getTime()));
}

template <class T>
inline bool operator!=(const Edit<T>& lhs, const Edit<T>& rhs) {
    return ((lhs.getObject() != rhs.getObject())
            || (lhs.getID() != rhs.getID())
            || (lhs.getKind() != rhs.getKind())
            || (lhs.getStatus() != rhs.getStatus())
            || (lhs.getEditor() != rhs.getEditor())
            || !(lhs.compareList(rhs.getCollectionList()))
            || !(rhs.compareList(lhs.getCollectionList()))
            || (lhs.getTime() == rhs.getTime()));
}
#endif // EDIT_H
