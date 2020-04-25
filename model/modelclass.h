#ifndef MODELCLASS_H
#define MODELCLASS_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <sqlite3.h>
#include <QtSql>
#include <QDebug>
#include <QFileInfo>
#include <vector>
#include "museum.h"
#include "user.h"
#include "collection.h"
#include "ModelException.h"

class ModelClass
{
public:
    virtual ModelClass& operator=(const ModelClass&) = delete;

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

    static ModelClass* getInstance(std::string);
    static void initdb(std::string, bool);

protected:
    ModelClass(std::string);
    ModelClass(const ModelClass&) = delete;
    QSqlDatabase db;
    QSqlQuery query;

private:
    static bool instanceFlag;
    static bool test;
    static bool pro;
    static std::string path;
    static ModelClass * single;
};

#endif // MODELCLASS_H
