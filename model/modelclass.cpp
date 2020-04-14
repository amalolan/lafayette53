#include "modelclass.h"
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
    qDebug() << query.lastError();
    QJsonArray array;
    while(query.next()){
        QJsonObject object;
        object["name"] = query.value(2).toString();
        object["introduction"] = "This is "+ query.value(2).toString();
        object["description"] = query.value(3).toString();
        object["museumID"] = query.value(0).toString().toInt();
        object["userID"] = query.value(1).toString().toInt();
        array.append(object);
    }
    QJsonDocument doc;
    doc.setArray(array);
    return doc.toJson().toStdString();
}

std::string ModelClass::getUserInfoJSON(int userID){
    QString id(QString::fromStdString(std::to_string(userID)));
    query.exec("SELECT username, email FROM public WHERE userID = "+id+";");
    qDebug() << query.lastError();
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
    query.exec("SELECT name, description, userID, museumID FROM museum WHERE museumID = "+id+";");
    query.next();
    qDebug() << query.lastError();
    QJsonObject object;
    object["name"] = query.value(0).toString();
    object["introduction"] = "This is "+ query.value(0).toString();
    object["description"] = query.value(1).toString();
    object["museumID"] = query.value(3).toString().toInt();
    object["userID"] = query.value(2).toString().toInt();
    QJsonDocument doc;
    doc.setObject(object);
    return doc.toJson().toStdString();
}

bool ModelClass::saveMuseumToDB(Museum & museum){
    if (museum.indb()) return false;
    query.exec("SELECT COUNT(*) FROM museum;");
    query.next();
    int nextMuseumIndex = query.value(0).toString().toInt();
    QString name = QString::fromStdString(museum.getName());
    QString museumID(nextMuseumIndex);
    QString userID(museum.getUserID());
    QString desc = QString::fromStdString(museum.getDescription());
    query.prepare("INSERT INTO museum(museumID, userID, name, description)"
                  " VALUES ("+museumID+", "+userID+", '"+name+"', '"+desc+"')");

    if(!query.exec()) return false;
    museum.setMuseumID(nextMuseumIndex);
    return true;
}

bool ModelClass::removeMuseumFromDB(Museum & museum){
    if (!museum.indb()) return false;
    QString id(museum.getMuseumID());
    query.exec("DELETE FROM museum WHERE museumID = "+id+";");
    museum.setMuseumID(-1);
    return true;
}
