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
#include <vector>

class ModelClass
{
public:
    ModelClass(std::string);
    virtual ~ModelClass();
    virtual bool open();
    virtual bool close();
    virtual bool status();

    virtual Collection getCollectionObject(int collectionID);
    virtual std::vector<Collection> getCollectionListByMuseumID(int museumID);
    virtual void saveCollectionToDB(Collection & collection);
    virtual void updateCollectionInDB(Collection & collection);
    virtual void removeCollectionInDB(Collection & collection);

    virtual std::vector<Museum> getMuseumList();
    virtual json getMuseumListJSON();

    virtual Museum getMuseumObject(std::string museumName);
    virtual Museum getMuseumObject(int museumID);
    virtual void saveMuseumToDB(Museum & museum);
    virtual void removeMuseumFromDB(Museum & museum);
    virtual void updateMuseumInDB(Museum & museum);

    virtual User getUserObject(std::string username);
    virtual User getUserObject(int userID);
    virtual void saveUserToDB(User & user);
    virtual void removeUserFromDB(User & user);
    virtual void updateUserInDB(User & user);

protected:
    QSqlDatabase db;
    QSqlQuery query;
};

#endif // MODELCLASS_H
