#include "museum.h"

Museum::Museum(std::string name, std::string description, int userID)
{
    this->name = name;
    this->description = description;
    this->userID = userID;
}

Museum::Museum(std::string name, std::string description, int userID, int museumID)
{
    this->name = name;
    this->description = description;
    this->userID = userID;
    this->museumID = museumID;
}

Museum::~Museum()
{

}

void Museum::setName(std::string name)
{
    this->name = name;
}

void Museum::setDescription(std::string desc)
{
    this->description = desc;
}

void Museum::setUserID(int id)
{
    this->userID = id;
}

void Museum::setMuseumID(int id)
{
    this->museumID = id;
}

std::string Museum::getJSON()
{
    QJsonObject properties;
    properties["museumID"] = this->museumID;
    properties["userID"] = this->userID;
    properties["name"] = QString::fromStdString(this->name);
    properties["introduction"] = QString::fromStdString("This is "+this->name);
    properties["description"] = QString::fromStdString(this->description);
    QJsonDocument doc;
    doc.setObject(properties);
    return doc.toJson().toStdString();
}

Museum* Museum::fromJson(std::string jsonString)
{
    //This method currently does not check if JSON String is correct.
    //That is, it does not check if the string has all fields to create
    //a museum object. Checks will be introducted in a later implementation
    //if neccessary.

    QJsonDocument doc = QJsonDocument::fromJson(QByteArray::fromStdString(jsonString));

    if (doc.isNull() || !doc.isObject())
    {
        return nullptr;
    }

    QJsonObject object = doc.object();
    std::string name = object["name"].toString().toStdString();
    std::string desc = object["description"].toString().toStdString();
    int userID = object["userID"].toInt();
    QJsonValue museumID  = object["museumID"];

    if(museumID.isUndefined()){
        return new Museum(name, desc, userID);
    }
    return new Museum(name, desc, userID, museumID.toInt());
}
