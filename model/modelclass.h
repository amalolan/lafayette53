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
#include <string>
#include <sstream>
#include <iostream>
#include "edit.h"
#include "ModelException.h"

class ModelClass
{
public:

    virtual ~ModelClass();
    ModelClass& operator=(const ModelClass&) = delete;
    virtual bool open();
    virtual bool close();
    virtual bool status();
    virtual void createTables();

    virtual void saveEditToDB(Edit<Collection>);
    virtual void updateEditInDB(Edit<Collection>);
    virtual void saveEditToDB(Edit<Artifact>);
    virtual void updateEditInDB(Edit<Artifact>);
    virtual void saveEditToDB(Edit<Museum>);
    virtual void updateEditInDB(Edit<Museum>);


    virtual std::vector<Edit<Museum>> getMuseumEdits(int userID);
    virtual std::vector<Edit<Collection>> getCollectionEdits(int userID);
    virtual std::vector<Edit<Artifact>> getArtifactEdits(int userID);

    virtual std::vector<Edit<Museum>> getMuseumActions(int museumID);
    virtual std::vector<Edit<Collection>> getCollectionActions(int museumID);
    virtual std::vector<Edit<Artifact>> getArtifactActions(int museumID);

    virtual Artifact getArtifact(int artifactID);
    virtual void saveArtifactToDB(Artifact &);
    virtual void updateArtifactInDB(Artifact &);
    virtual void removeArtifactInDB(Artifact &);
    virtual std::vector<Artifact> getArtifactsByMuseum(int museumID);

    virtual std::vector<Collection> getCollectionsByArtifact(int artifactID);
    virtual std::vector<Artifact> getArtifactsByCollection(int collectionID);
    virtual void addArtifactCollection(const Collection & collection, const Artifact & artifact);
    virtual void addArtifactCollection(const Artifact & artifact, const Collection & collection);

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

    virtual std::vector<Museum> getMuseumByCurator(int userID);

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
    virtual void artifactCheck(const Artifact &);
    virtual void artifactPreCheck(const Artifact & artifact);
    virtual void collectionCheck(const Collection &);
    virtual void museumCheck(const Museum &);
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
