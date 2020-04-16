#ifndef MODELCLASS_H
#define MODELCLASS_H
#include "museum.h"
#include "user.h"
#include <QtSql>
#include <QDebug>
#include <QFileInfo>

class ModelClass
{
public:
    static void initdb();
    static bool open();
    static bool close();
    static bool status();
    static std::string getMuseumListJSON();
    static std::string getUserInfoJSON(int userID);
    static std::string getMuseumInfoJSON(int museumID);
    static bool saveMuseumToDB(Museum & museum);
    static bool removeMuseumFromDB(Museum & museum);
    static bool updateMuseumInDB(Museum & museum);
    static std::string getPasswordHash(std::string username);
    static bool saveUserToDB(User & user);
    static bool removeUserFromDB(User & user);
    static bool updateUserInDB(User & user);

private:
    static QSqlDatabase db;
    static QSqlQuery query;
};

#endif // MODELCLASS_H
