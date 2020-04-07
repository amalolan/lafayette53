#include "modelclass.h"
QSqlDatabase ModelClass:: db = QSqlDatabase::addDatabase("QSQLITE",QSqlDatabase::defaultConnection);
QSqlQuery ModelClass::query(db);

void ModelClass::initdb(){
    db.setDatabaseName("db.db");
}

bool ModelClass::open(){
    db.open();
    return db.isOpen();
}

bool ModelClass::close(){
    db.close();
    return !db.isOpen();
}

bool ModelClass::status(){
    return db.isOpen();
}

std::string ModelClass::getMuseumListJSON(){
    query.prepare("SELECT museumID, userID, name, description, FROM museum;");
    query.exec();
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
    query.prepare("SELECT username, email, FROM public WHERE userID = "+id+";");
    query.exec();
    QJsonObject object;
    object["name"] = query.value(0).toString();
    object["email"] = query.value(1).toString();
    QJsonDocument doc;
    doc.setObject(object);
    return doc.toJson().toStdString();
}

std::string ModelClass::getMuseumInfoJSON(int museumID){
    QString id(QString::fromStdString(std::to_string(museumID)));
    query.prepare("SELECT name, description, FROM museum WHERE museumID = "+id+";");
    query.exec();
    QJsonObject object;
    object["name"] = query.value(0).toString();
    object["description"] = query.value(1).toString();
    QJsonDocument doc;
    doc.setObject(object);
    return doc.toJson().toStdString();
}
