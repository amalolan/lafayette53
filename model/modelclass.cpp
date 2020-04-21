#include "modelclass.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ModelException.h"

QSqlDatabase ModelClass:: db = QSqlDatabase::addDatabase("QSQLITE", "Connection");
QSqlQuery ModelClass::query(db);
void ModelClass::initdb(){
    /*
     * Builds are different on different on Linux and Mac OS.
     * Known support for Linux flavors: Ubuntu and Fedora.
     */

    std::string linuxPath = "../../lafayette53/database/db.db";
    QString qLinuxPath = QString::fromStdString(linuxPath);
    std::string macPath = "../../../../../lafayette53/database/db.db";
    QString qMacPath = QString::fromStdString(macPath);

    db.setDatabaseName(qLinuxPath);
    for (int i = 3; i > 0 && !db.open(); i--) {
            qDebug("Try %d opening database failed.", i - 3);
            qDebug("Error occurred opening the database.");
            qDebug("Reason: %s.", qPrintable(db.lastError().text()));
            qDebug("Trying again. Tries left : %d.", i);
            db.setDatabaseName(qLinuxPath);
        }

    if(!db.open()){
        qDebug("Model class build failed for Linux setup.");
        qDebug("Checking if system is a Mac OS X build.");
        db.setDatabaseName(qMacPath);
        for (int i = 3; i > 0 && !db.open(); i--) {
                qDebug("Try %d opening database failed", i - 3);
                qDebug("Error occurred opening the database.");
                qDebug("Reason: %s.", qPrintable(db.lastError().text()));
                qDebug("Trying again. Tries left : %d.", i);
                db.setDatabaseName(qMacPath);
            }
    }

    if(!db.open()){
        qDebug("Model class build failed for both builds.");
        throw ModelException("Database failed to open.");
    }else{
        qDebug("Model class build successful.");
    }
}

bool ModelClass::open(){
    if(!db.isOpen()){
        db.open();
    }
    return db.isOpen();
}

bool ModelClass::close(){
    if(db.isOpen()){
        db.close();
    }
    return !db.isOpen();
}

bool ModelClass::status(){
    return db.isOpen();
}

std::string ModelClass::getMuseumListJSON(){
    query.exec("SELECT museumID, userID, name, description FROM museum;");
    QJsonArray array;
    while(query.next()){
        QJsonObject object;
        object["name"] = query.value(2).toString();
        object["introduction"] = "This is "+ query.value(2).toString();
        object["description"] = query.value(3).toString();
        object["id"] = query.value(0).toString().toInt();
        object["userID"] = query.value(1).toString().toInt();
        array.append(object);
    }
    QJsonDocument doc;
    doc.setArray(array);
    query.finish();
    return doc.toJson().toStdString();
}

std::string ModelClass::getUserInfoJSON(int userID){
    QString id(QString::fromStdString(std::to_string(userID)));
    bool execute = query.exec("SELECT username, email FROM public WHERE userID = "+id+";");

    if (!execute)
    {
        throw ModelException("UserID "+std::to_string(userID)+" not in database");
    }
    query.next();
    QJsonObject object;
    object["username"] = query.value(0).toString();
    object["email"] = query.value(1).toString();
    QJsonDocument doc;
    doc.setObject(object);
    return doc.toJson().toStdString();
}

std::string ModelClass::getMuseumInfoJSON(int museumID){
    QString id(QString::fromStdString(std::to_string(museumID)));
    bool execute = query.exec("SELECT name, description, userID, museumID FROM museum WHERE museumID = "+id+";");

    if (!execute)
    {
        throw ModelException("MuseumID "+std::to_string(museumID)+" not in database");
    }
    query.next();
    qDebug() << query.lastError();
    QJsonObject object;
    object["name"] = query.value(0).toString();
    object["introduction"] = "This is "+ query.value(0).toString();
    object["description"] = query.value(1).toString();
    object["id"] = query.value(3).toString().toInt();
    object["userID"] = query.value(2).toString().toInt();
    QJsonDocument doc;
    doc.setObject(object);
    query.finish();
    return doc.toJson().toStdString();
}

bool ModelClass::saveMuseumToDB(Museum & museum){
    if (museum.indb())
    {
        throw ModelException("Museum object already stored in database");
    }
    int seed = std::chrono::steady_clock::now().time_since_epoch().count();
    std::default_random_engine eng(seed);
    int nextMuseumIndex = eng();
    QString name = QString::fromStdString(museum.getName());
    QString museumID(QString::fromStdString(std::to_string(nextMuseumIndex)));
    QString userID(QString::fromStdString(std::to_string(museum.getUser().getUserID())));
    QString desc = QString::fromStdString(museum.getDescription());
    query.prepare("INSERT INTO museum(museumID, userID, name, description)"
                  " VALUES ("+museumID+", "+userID+", '"+name+"', '"+desc+"')");

    if(!query.exec())
    {
        std::string err = query.lastError().databaseText().toStdString()+". Cause: "+query.lastError().driverText().toStdString();
        throw ModelException("Saving museum failed. Reason: "+err);
    }
    museum.setMuseumID(nextMuseumIndex);
    query.finish();
    return true;
}

bool ModelClass::removeMuseumFromDB(Museum & museum){
    if (!museum.indb())
    {
        throw ModelException("Museum object not stored in database");
    }
    QString id(QString::fromStdString(std::to_string(museum.getMuseumID())));
    query.exec("DELETE FROM museum WHERE museumID = "+id+";");

    bool done = query.numRowsAffected() == 1;
    if (done)
    {
       museum.setMuseumID(-1);
    }
    query.finish();
    return done;
}

bool ModelClass::updateMuseumInDB(Museum & museum){
    if (!museum.indb())
    {
        throw ModelException("Museum object not stored in database");
    }
    QString name = QString::fromStdString(museum.getName());
    QString museumID = QString::fromStdString(std::to_string(museum.getMuseumID()));
    QString description = QString::fromStdString(museum.getDescription());
    bool done = query.exec("UPDATE museum SET name = '"+name+"', description = '"+description+"' WHERE museumID = "+museumID+";");
    if(!done)
    {
        std::string err = query.lastError().databaseText().toStdString()+". Cause: "+query.lastError().driverText().toStdString();
        throw ModelException("Museum object failed to update in database. Reason: "+err);
    }
    query.finish();
    return done;
}

std::string ModelClass::getPasswordHash(std::string username){
    QString name = QString::fromStdString(username);
    query.prepare("SELECT password FROM public where username GLOB '"+name+"';");
    if(!query.exec())
    {
        throw ModelException("Username does not exist in database");
    }
    query.next();
    std::string output = query.value(0).toString().toStdString();
    query.finish();
    return output;
}

User ModelClass::getUserObject(std::string username){
    QString name = QString::fromStdString(username);
    query.prepare("SELECT username, email, password, userID FROM public WHERE username GLOB '"+name+"';");
    if(!query.exec())
    {
        throw ModelException("Username does not exist in database");
    }
    query.next();
    std::string uname = query.value(0).toString().toStdString();
    std::string email = query.value(1).toString().toStdString();
    std::string password = query.value(2).toString().toStdString();
    int userID = query.value(3).toInt();
    return User(uname, email, password, userID);
}

User ModelClass::getUserObject(int userID){
    QString id(QString::fromStdString(std::to_string(userID)));
    bool exec = query.exec("SELECT username, email, password FROM public WHERE userID = "+id+";");
    if(!exec)
    {
        throw ModelException("UserID does not exist in database");
    }
    query.next();
    std::string uname = query.value(0).toString().toStdString();
    std::string email = query.value(1).toString().toStdString();
    std::string password = query.value(2).toString().toStdString();
    query.finish();
    return User(uname, email, password, userID);
}

Museum ModelClass::getMuseumObject(std::string museumName){
    QString name = QString::fromStdString(museumName);
    query.prepare("SELECT museumID, userID, name, description FROM museum where name GLOB '"+name+"';");
    if(!query.exec())
    {
        throw ModelException("Museum name does not exist in database");
    }
    query.next();
    int museumID = query.value(0).toInt();
    int userID = query.value(1).toInt();
    std::string _museumName = query.value(2).toString().toStdString();
    std::string description = query.value(3).toString().toStdString();
    query.finish();
    User user(ModelClass::getUserObject(userID));
    return Museum(_museumName, description, user, museumID);
}

bool ModelClass::saveUserToDB(User & user){
    if (user.indb())
    {
        throw ModelException("User object already in database");
    }
    int seed = std::chrono::steady_clock::now().time_since_epoch().count();
    std::default_random_engine eng(seed);
    int nextUserIndex = eng();
    QString name = QString::fromStdString(user.getName());
    QString userID(QString::fromStdString(std::to_string(nextUserIndex)));
    QString email = QString::fromStdString(user.getEmail());
    QString password = QString::fromStdString(user.getPassword());
    QString queryText("INSERT INTO public(userID,username,email,password) VALUES ("+userID+",'"+name+"','"+email+"','"+password+"');");
    bool done = query.exec(queryText);
    if (!done)
    {
        std::string err = query.lastError().databaseText().toStdString()+". Cause: "+query.lastError().driverText().toStdString();
        throw ModelException("Saving user failed. Reason: "+err);
    }
    user.setUserID(nextUserIndex);
    query.finish();
    return true;
}

bool ModelClass::removeUserFromDB(User & user){
    if (!user.indb())
    {
        throw ModelException("User object does not exist in database");
    }
    QString id(QString::fromStdString(std::to_string(user.getUserID())));
    query.exec("DELETE FROM public WHERE userID = "+id+";");
    bool done = query.numRowsAffected() == 1;
    if (done)
    {
       user.setUserID(-1);
    }
    else
    {
        throw ModelException("User object does not exist in database");
    }
    query.finish();
    return done;
}

bool ModelClass::updateUserInDB(User & user){
    if (!user.indb())
    {
        throw ModelException("User object does not exist in database");
    }
    QString email = QString::fromStdString(user.getEmail());
    QString password = QString::fromStdString(user.getPassword());
    QString id = QString::fromStdString(std::to_string(user.getUserID()));
    QString queryText("UPDATE public SET email = '"+email+"', password = '"+password+"' WHERE userID = "+id+";");
    bool done = query.exec(queryText);
    if (!user.indb())
    {
        std::string err = query.lastError().databaseText().toStdString()+". Cause: "+query.lastError().driverText().toStdString();
        throw ModelException("User object does not exist in database. Reason: "+err);
    }
    query.finish();
    return done;
}
