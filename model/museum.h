#ifndef MUSEUM_H
#define MUSEUM_H
#include <string>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

class Museum
{
public:
    Museum(std::string name, std::string description, int userID);
    Museum(std::string name, std::string description, int userID, int museumID);
    ~Museum();

    void setName(std::string name);
    void setDescription(std::string desc);
    void setUserID(int id);
    void setMuseumID(int id);

    std::string getName();
    std::string getDescription();
    int getUserID();
    int getMuseumID();
    bool indb();

    std::string getJSON();

    static Museum* fromJson(std::string jsonString);

private:
    Museum();
    std::string name;
    std::string description;
    int museumID;
    int userID;
};

#endif // MUSEUM_H
