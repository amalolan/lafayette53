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
#include "artifact.h"
#include "ModelException.h"

class ModelClass
{
public:
    virtual ModelClass& operator=(const ModelClass&) = delete;

    virtual ~ModelClass();
    virtual bool open();
    virtual bool close();
    virtual bool status();
    virtual void createTables();

    virtual Artifact getArtifact(int artifactID);
    virtual void saveArtifactToDB(Artifact &);
    virtual void updateArtifactInDB(Artifact &);
    virtual void removeArtifactInDB(Artifact &);
    virtual std::vector<Artifact> getArtifactsByMuseum(int museumID);

    virtual std::vector<Collection> getCollectionsByArtifact(int artifactID);
    virtual std::vector<Artifact> getArtifactsByCollection(int collectionID);
    virtual void addArtifactCollection(Collection collection, Artifact artifact);
    virtual void addArtifactCollection(Artifact artifact, Collection collection);

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

    static ModelClass* getInstance(bool kind);
    static void initdb(std::string);
    static const bool test;
    static const bool pro;

protected:
    ModelClass(std::string);
    ModelClass(std::string, bool);
    ModelClass(const ModelClass&) = delete;
    virtual void artifactCheck(Artifact &);
    virtual void collectionCheck(Collection &);
    QSqlDatabase db;
    QSqlQuery query;

private:
    bool kind;
    static void setup();
    static bool instanceFlagTest;
    static bool instanceFlagPro;
    static std::string path;
    static ModelClass * singlePro;
    static ModelClass * singleTest;
};

#endif // MODELCLASS_H
