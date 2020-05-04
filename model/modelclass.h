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
#include <algorithm>
#include "edit.h"
#include "ModelException.h"

/*
 * This class represents the object that manages the database connection resource (Model) of the MVC design system
 */
class ModelClass
{
public:

    virtual ~ModelClass();
    ModelClass& operator=(const ModelClass&) = delete;

    /*
     * Opens the connection to the database
     */
    virtual bool open();

    /*
     * Closes the connection to the database
     */
    virtual bool close();

    /*
     * Returns true if the database is opened
     */
    virtual bool status();

    /*
     * Creates all tables needed for the opration of the ModelClass
     */
    virtual void createTables();

    /*
     * Saves an Edit<Collection> object to the database
     */
    virtual void saveEditToDB(Edit<Collection>&);

    /*
     * Updates an Edit<Collection> object status to the database
     * and implements the change if approved
     */
    virtual void updateEditInDB(Edit<Collection>&);

    /*
     * Removes an Edit<Collection> object to the database
     */
    virtual void removeEditInDB(Edit<Collection>&);

    /*
     * Saves an Edit<Artifact> object to the database
     */
    virtual void saveEditToDB(Edit<Artifact>&);

    /*
     * Updates an Edit<Artifact> object status to the database
     * and implements the change if approved
     */
    virtual void updateEditInDB(Edit<Artifact>&);

    /*
     * Removes an Edit<Artifact> object to the database
     */
    virtual void removeEditInDB(Edit<Artifact>&);

    /*
     * Saves an Edit<Museum> object to the database
     */
    virtual void saveEditToDB(Edit<Museum>&);

    /*
     * Updates an Edit<Museum> object status to the database
     * and implements the change if approved
     */
    virtual void updateEditInDB(Edit<Museum>&);

    /*
     * Removes an Edit<Museum> object to the database
     */
    virtual void removeEditInDB(Edit<Museum>&);

    /*
     * Returns all the Edit<Museum> object for a specific user in descending time
     */
    virtual std::vector<Edit<Museum>> getMuseumEdits(int userID);

    /*
     * Returns all the Edit<Collection> object for a specific user in descending time
     */
    virtual std::vector<Edit<Collection>> getCollectionEdits(int userID);

    /*
     * Returns all the Edit<Artifact> object for a specific user in descending time
     */
    virtual std::vector<Edit<Artifact>> getArtifactEdits(int userID);

    /*
     * Returns all the Edit<Museum> objects which have not been reviewed yet for a specific museum in descending time
     */
    virtual std::vector<Edit<Museum>> getMuseumActions(int museumID);


    /*
     * Returns all the Edit<Collection> objects which have not been reviewed yet for a specific museum in descending time
     */
    virtual std::vector<Edit<Collection>> getCollectionActions(int museumID);


    /*
     * Returns all the Edit<Artifact> objects which have not been reviewed yet for a specific museum in descending time
     */
    virtual std::vector<Edit<Artifact>> getArtifactActions(int museumID);


    /*
     * Returns an Edit<Museum> of a specific editID
     */
    virtual Edit<Museum> getEditMuseumObject(int editID);

    /*
     * Returns an Edit<Collection> of a specific editID
     */
    virtual Edit<Collection> getEditCollectionObject(int editID);

    /*
     * Returns an Edit<Artifact> of a specific editID
     */
    virtual Edit<Artifact> getEditArtifactObject(int editID);

    /*
     * Returns an Artifact object of a specific id
     */
    virtual Artifact getArtifact(int artifactID);

    /*
     * Saves an Artifact object to the database
     */
    virtual void saveArtifactToDB(Artifact &);

    /*
     * Updates an Artifact object to the database
     */
    virtual void updateArtifactInDB(Artifact &);

    /*
     * Removes an Artifact object to the database
     */
    virtual void removeArtifactInDB(Artifact &);

    /*
     * Returns all artifact objects of a specific museum
     */
    virtual std::vector<Artifact> getArtifactsByMuseum(int museumID);

    /*
     * Returns all collection objects of a specific museum
     */
    virtual std::vector<Collection> getCollectionsByArtifact(int artifactID);

    /*
     * Returns all collection objects of a specific collection
     */
    virtual std::vector<Artifact> getArtifactsByCollection(int collectionID);

    /*
     * Adds an artifact to a collection
     */
    virtual void addArtifactCollection(const Collection & collection, const Artifact & artifact);

    /*
     * Adds an artifact to a collection
     */
    virtual void addArtifactCollection(const Artifact & artifact, const Collection & collection);

    /*
     * Removes an artifact from a collection
     */
    virtual void removeArtifactCollection(const Collection & collection, const Artifact & artifact);

    /*
     * Removes an artifact from a collection
     */
    virtual void removeArtifactCollection(const Artifact & artifact, const Collection & collection);

    /*
     * Removes an artifact from all collections
     */
    virtual void removeArtifactCollection(const Artifact & artifact);

    /*
     * Removes all artifacts from a collection
     */
    virtual void removeArtifactCollection(const Collection & collection);

    /*
     * Returns a Collection object of a specific id
     */
    virtual Collection getCollectionObject(int collectionID);

    /*
     * Returns all collection objects of a specific museum
     */
    virtual std::vector<Collection> getCollectionListByMuseumID(int museumID);

    /*
     * Saves a Collection object to the database
     */
    virtual void saveCollectionToDB(Collection & collection);

    /*
     * Updates a Collection object to the database
     */
    virtual void updateCollectionInDB(Collection & collection);

    /*
     * Removes a Collection object to the database
     */
    virtual void removeCollectionInDB(Collection & collection);

    /*
     * Returns all Museum objects in the database
     */
    virtual std::vector<Museum> getMuseumList();

    /*
     * Returns a Museum object of a specific name
     */
    virtual Museum getMuseumObject(std::string museumName);

    /*
     * Returns a Museum object of a specific id
     */
    virtual Museum getMuseumObject(int museumID);

    /*
     * Saves a Museum object to the database
     */
    virtual void saveMuseumToDB(Museum & museum);

    /*
     * Removes a Museum object to the database
     */
    virtual void removeMuseumFromDB(Museum & museum);

    /*
     * Updates a Museum object to the database
     */
    virtual void updateMuseumInDB(Museum & museum);

    /*
     * Returns all Museum objects of a specific curatorID
     */
    virtual std::vector<Museum> getMuseumByCurator(int userID);

    /*
     * Returns a User object of a specific username
     */
    virtual User getUserObject(std::string username);

    /*
     * Returns a User object of a specific id
     */
    virtual User getUserObject(int userID);

    /*
     * Saves a User object to the database
     */
    virtual void saveUserToDB(User & user);

    /*
     * Removes a User object to the database
     */
    virtual void removeUserFromDB(User & user);

    /*
     * Updates a User object to the database
     */
    virtual void updateUserInDB(User & user);

    /*
     * Returns true if the User object is the head curator
     */
    virtual bool checkHeadCurator(User & user);

    /*
     * Returns the pointer of the database connection
     * ModelClass::test for connection to the testing database
     * ModelClass::pro for the production database
     */
    static ModelClass* getInstance(bool kind);

    /*
     * Sets up the file path of the database files
     */
    static void initdb(std::string);
    static const bool test;
    static const bool pro;

protected:
    ModelClass(std::string);
    ModelClass(std::string, bool);
    ModelClass(const ModelClass&) = delete;

    /*
     * Checks if an Artifact object is in the database
     */
    virtual void artifactCheck(const Artifact &);

    /*
     * Checks if an Artifact object is a correct instance before saving to database
     * i.e The Artifact object has a legit Museum object
     */
    virtual void artifactPreCheck(const Artifact & artifact);

    /*
     * Checks if a Collection object is in the database
     */
    virtual void collectionCheck(const Collection &);

    /*
     * Checks if a Museum object is in the database
     */
    virtual void museumCheck(const Museum &);

    /*
     * Checks if a Collection object is a correct instance before saving to database
     * i.e The Collection object has a legit Museum object
     */
    virtual void preCollectionCheck(const Collection &);
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
