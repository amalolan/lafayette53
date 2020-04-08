#include "modelclass.h"
QSqlDatabase ModelClass:: db = QSqlDatabase::addDatabase("QSQLITE", "Connection");
QSqlQuery ModelClass::query(db);
void ModelClass::initdb(){
    std::string path = "../../lafayette53/database/db.db";
    QString qpath = QString::fromStdString(path);
    db.setDatabaseName(qpath);
    if (!db.open()) {
            qDebug("Error occurred opening the database.");
            qDebug("%s.", qPrintable(db.lastError().text()));
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
        object["description"] = query.value(3).toString();
        object["id"] = query.value(0).toString();
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
    query.exec("SELECT name, description FROM museum WHERE museumID = "+id+";");
    query.next();
    qDebug() << query.lastError();
    QJsonObject object;
    object["name"] = query.value(0).toString();
    object["description"] = query.value(1).toString();
    QJsonDocument doc;
    doc.setObject(object);
    return doc.toJson().toStdString();
}
