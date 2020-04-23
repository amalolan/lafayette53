#ifndef MODELCLASS_H
#define MODELCLASS_H
#include "museum.h"
#include "user.h"
#include "collection.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ModelException.h"
#include <QtSql>
#include <QDebug>
#include <QFileInfo>

class ModelClass
{
public:
    ModelClass(std::string);
    virtual ~ModelClass();
    virtual bool open();
    virtual bool close();
    virtual bool status();

    virtual json getCollectionInfoJSON(int collectionID);
    virtual json getCollectionListByMuseumID(int museumID);
    virtual void saveCollectionToDB(Collection & collection);
    virtual void updateCollectionInDB(Collection & collection);
    virtual void removeCollectionInDB(Collection & collection);

    virtual std::string getMuseumListJSON();
    virtual std::string getMuseumInfoJSON(int museumID);
    virtual Museum getMuseumObject(std::string museumName);
    virtual json getMuseumInfoJson(int museumID);
    virtual void saveMuseumToDB(Museum & museum);
    virtual void removeMuseumFromDB(Museum & museum);
    virtual void updateMuseumInDB(Museum & museum);

    virtual User getUserObject(std::string username);
    virtual User getUserObject(int userID);
    virtual std::string getUserInfoJSON(int userID);
    virtual json getUserInfoJson(std::string username);
    virtual std::string getPasswordHash(std::string username);
    virtual void saveUserToDB(User & user);
    virtual void removeUserFromDB(User & user);
    virtual void updateUserInDB(User & user);

protected:
    QSqlDatabase db;
    QSqlQuery query;
};

#endif // MODELCLASS_H
