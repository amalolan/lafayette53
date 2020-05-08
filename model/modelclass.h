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

/**
 * @brief This class represents the object that manages the database connection resource (Model) of the MVC design system
 */
class ModelClass
{
public:

    virtual ~ModelClass();
    ModelClass& operator=(const ModelClass&) = delete;

    /**
     * @brief Opens the connection to the database
     */
    virtual bool open();

    /**
     * @brief Closes the connection to the database
     */
    virtual bool close();

    /**
     * @return true if the database is opened
     */
    virtual bool status();

    /**
     * @brief Creates all tables needed for the opration of the ModelClass
     */
    virtual void createTables();

    /**
     * @brief Saves an Edit<Collection> object to the database
     */
    virtual void saveEditToDB(Edit<Collection>&);

    /**
     * @brief Updates an Edit<Collection> object status to the database and implements the change if approved
     */
    virtual void updateEditInDB(Edit<Collection>&);

    /**
     * @brief Removes an Edit<Collection> object to the database
     */
    virtual void removeEditInDB(Edit<Collection>&);

    /**
     * @brief Saves an Edit<Artifact> object to the database
     */
    virtual void saveEditToDB(Edit<Artifact>&);

    /**
     * @brief Updates an Edit<Artifact> object status to the database and implements the change if approved
     *
     */
    virtual void updateEditInDB(Edit<Artifact>&);

    /**
     * @brief Removes an Edit<Artifact> object to the database
     *
     */
    virtual void removeEditInDB(Edit<Artifact>&);

    /**
     * @brief Saves an Edit<Museum> object to the database
     */
    virtual void saveEditToDB(Edit<Museum>&);

    /**
     * @brief Updates an Edit<Museum> object status to the database and implements the change if approved
     *
     */
    virtual void updateEditInDB(Edit<Museum>&);

    /**
     * @brief Removes an Edit<Museum> object to the database
     *
     */
    virtual void removeEditInDB(Edit<Museum>&);

    /**
     * @return All Edit<Museum> objects for a specific user in descending time
     */
    virtual std::vector<Edit<Museum>> getMuseumEdits(int userID);

    /**
     * @return All Edit<Collection> object for a specific user in descending time
     */
    virtual std::vector<Edit<Collection>> getCollectionEdits(int userID);

    /**
     * @return All Edit<Artifact> object for a specific user in descending time
     */
    virtual std::vector<Edit<Artifact>> getArtifactEdits(int userID);

    /**
     * @return All Edit<Museum> objects which have not been reviewed yet for a specific museum in descending time
     */
    virtual std::vector<Edit<Museum>> getMuseumActions(int museumID);


    /**
     * @brief All Edit<Collection> objects which have not been reviewed yet for a specific museum in descending time
     */
    virtual std::vector<Edit<Collection>> getCollectionActions(int museumID);


    /**
     * @brief All Edit<Artifact> objects which have not been reviewed yet for a specific museum in descending time
     */
    virtual std::vector<Edit<Artifact>> getArtifactActions(int museumID);


    /**
     * @return an Edit<Museum> of a specific editID
     */
    virtual Edit<Museum> getEditMuseumObject(int editID);

    /**
     * @return an Edit<Collection> of a specific editID
     */
    virtual Edit<Collection> getEditCollectionObject(int editID);

    /**
     * @return an Edit<Artifact> of a specific editID
     */
    virtual Edit<Artifact> getEditArtifactObject(int editID);

    /**
     * @return an Artifact object of a specific id
     */
    virtual Artifact getArtifact(int artifactID);

    /**
     * @brief Saves an Artifact object to the database
     */
    virtual void saveArtifactToDB(Artifact &);

    /**
     * @brief Updates an Artifact object to the database
     */
    virtual void updateArtifactInDB(Artifact &);

    /**
     * @brief Removes an Artifact object to the database
     */
    virtual void removeArtifactInDB(Artifact &);

    /**
     * @return all artifact objects of a specific museum
     */
    virtual std::vector<Artifact> getArtifactsByMuseum(int museumID);

    /**
     * @return all collection objects of a specific museum
     */
    virtual std::vector<Collection> getCollectionsByArtifact(int artifactID);

    /**
     * @return all collection objects of a specific collection
     */
    virtual std::vector<Artifact> getArtifactsByCollection(int collectionID);

    /**
     * @brief Adds an artifact to a collection
     */
    virtual void addArtifactCollection(const Collection & collection, const Artifact & artifact);

    /**
     * @brief Adds an artifact to a collection
     */
    virtual void addArtifactCollection(const Artifact & artifact, const Collection & collection);

    /**
     * @brief Removes an artifact from a collection
     */
    virtual void removeArtifactCollection(const Collection & collection, const Artifact & artifact);

    /**
     * @brief Removes an artifact from a collection
     */
    virtual void removeArtifactCollection(const Artifact & artifact, const Collection & collection);

    /**
     * @brief Removes an artifact from all collections
     */
    virtual void removeArtifactCollection(const Artifact & artifact);

    /**
     * @brief Removes all artifacts from a collection
     */
    virtual void removeArtifactCollection(const Collection & collection);

    /**
     * @brief Returns a Collection object of a specific id
     */
    virtual Collection getCollectionObject(int collectionID);

    /**
     * @brief Returns all collection objects of a specific museum
     */
    virtual std::vector<Collection> getCollectionListByMuseumID(int museumID);

    /**
     * @brief Saves a Collection object to the database
     */
    virtual void saveCollectionToDB(Collection & collection);

    /**
     * @brief Updates a Collection object to the database
     */
    virtual void updateCollectionInDB(Collection & collection);

    /**
     * @brief Removes a Collection object to the database
     */
    virtual void removeCollectionInDB(Collection & collection);

    /**
     * @return all Museum objects in the database
     */
    virtual std::vector<Museum> getMuseumList();

    /**
     * @return a Museum object of a specific name
     */
    virtual Museum getMuseumObject(std::string museumName);

    /**
     * @return a Museum object of a specific id
     */
    virtual Museum getMuseumObject(int museumID);

    /**
     * @brief Saves a Museum object to the database
     */
    virtual void saveMuseumToDB(Museum & museum);

    /**
     * @brief Removes a Museum object to the database
     */
    virtual void removeMuseumFromDB(Museum & museum);

    /**
     * @brief Updates a Museum object to the database
     */
    virtual void updateMuseumInDB(Museum & museum);

    /**
     * @return all Museum objects of a specific curatorID
     */
    virtual std::vector<Museum> getMuseumByCurator(int userID);

    /**
     * @return a User object of a specific username
     */
    virtual User getUserObject(std::string username);

    /**
     * @return a User object of a specific id
     */
    virtual User getUserObject(int userID);

    /**
     * @brief Saves a User object to the database
     */
    virtual void saveUserToDB(User & user);

    /**
     * @brief Removes a User object to the database
     */
    virtual void removeUserFromDB(User & user);

    /**
     * @brief Updates a User object to the database
     */
    virtual void updateUserInDB(User & user);

    /**
     * @return true if the User object is the head curator
     */
    virtual bool checkHeadCurator(User & user);

    /**
     * @param kind: ModelClass::test for connection to the testing database. ModelClass::pro for the production database
     * @return Returns the pointer of the kind of database connection
     *
     */
    static ModelClass* getInstance(bool kind);

    /**
     * @brief Sets up the file path of the database files
     */
    static void initdb(std::string);

    /**
     * @brief bool value for ModelCLass::test
     */
    static const bool test;

    /**
     * @brief bool value for ModelClass::pro
     */
    static const bool pro;

protected:
    ModelClass(std::string);
    ModelClass(std::string, bool);
    ModelClass(const ModelClass&) = delete;

    /**
     * @brief Checks if an Artifact object is in the database
     */
    virtual void artifactCheck(const Artifact &);

    /**
     * @brief Checks if an Artifact object is a correct instance before saving to database. That is, Artifact object has a legit Museum object
     */
    virtual void artifactPreCheck(const Artifact & artifact);

    /**
     * @brief Checks if a Collection object is in the database
     */
    virtual void collectionCheck(const Collection &);

    /**
     * @brief Checks if a Museum object is in the database
     */
    virtual void museumCheck(const Museum &);

    /**
     * @brief Checks if a Collection object is a correct instance before saving to database. That is, Collection object has a legit Museum object
     */
    virtual void preCollectionCheck(const Collection &);

    /**
     * @brief db: Database connection object to database
     */
    QSqlDatabase db;

    /**
     * @brief query: QSqlQuery object to execute query to the database
     */
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
