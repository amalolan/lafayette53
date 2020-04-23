#include "modelclass.h"

ModelClass::ModelClass(std::string databasePath) {
    db = QSqlDatabase::addDatabase("QSQLITE", "Connection");
    db.setDatabaseName(QString::fromStdString(databasePath));
    if(!db.open()){
        qDebug("Model class build failed for linux and macos builds.");
        throw ModelException("Database failed to open.");
    }else{
        qDebug("Model class build successful.");
    }
    this->query = new QSqlQuery(db);
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

ModelClass::~ModelClass() {

}

bool ModelClass::status(){
    return db.isOpen();
}

json ModelClass::getCollectionInfoJSON(int collectionID){
    QString id(QString::fromStdString(std::to_string(collectionID)));
    this->query->exec("SELECT collectionID, museumID, name, description FROM collections WHERE collectionID = "+id+";");
    this->query->next();
    if (!this->query->isValid())
    {
        throw ModelException("No collection entity stored in database");
    }
    json output;
    json collection;
    collection["name"] = this->query->value(2).toString().toStdString();
    collection["description"] = this->query->value(3).toString().toStdString();
    collection["id"] = this->query->value(0).toInt();
    collection["intoduction"] = "This is "+this->query->value(2).toString().toStdString();
    output["collection"] = collection;
    output["museum"] = this->getMuseumInfoJson(this->query->value(1).toInt());
    this->query->finish();
    return output;
}

json ModelClass::getCollectionListByMuseumID(int museumID){
    QString id(QString::fromStdString(std::to_string(museumID)));
    this->query->exec("SELECT collectionID, museumID, name, description FROM collections WHERE museumID = "+id+";");
    this->query->next();
    if (!this->query->isValid())
    {
        //no exception should be thrown. just returned empty list.
        //throw ModelException("No collection entity stored in database");
        return json::array();
    }
    json list;
    do
    {
        json entity;
        entity["name"] = this->query->value(2).toString().toStdString();
        entity["description"] = this->query->value(3).toString().toStdString();
        entity["id"] = this->query->value(0).toInt();
        entity["intoduction"] = "This is "+this->query->value(2).toString().toStdString();
        list.emplace_back(entity);
    }while(this->query->next());
    this->query->finish();
//    json output;
//    output["collectionList"] = list;

    //we just need collection list not museum.
    //output["museum"] = ModelClass::getMuseumInfoJson(museumID);
    return list;
}

void ModelClass::saveCollectionToDB(Collection & collection){
    if(collection.indb())
    {
        throw ModelException("Collection already exists in database");
    }
    else if (!collection.getMuseum().indb())
    {
        throw ModelException("Museum object of collection does not exist in database");
    }
    else if (!collection.getMuseum().getUser().indb())
    {
        throw ModelException("User object of Museum object of collection does not exist in database");
    }
    else
    {
        try
        {
            Museum museum = ModelClass::getMuseumObject(collection.getMuseum().getName());
        } catch (ModelException e)
        {
            throw ModelException("Museum object of collection does not exist in database");
        }

        try
        {
            User user = ModelClass::getUserObject(collection.getMuseum().getUser().getName());
        } catch (ModelException e)
        {
            throw ModelException("User object of Museum object of collection does not exist in database");
        }
    }

    int seed = std::chrono::steady_clock::now().time_since_epoch().count();
    std::default_random_engine eng(seed);
    int nextCollectionIndex = eng();
    QString name = QString::fromStdString(collection.getName());
    QString museumID(QString::fromStdString(std::to_string(collection.getMuseum().getMuseumID())));
    QString id(QString::fromStdString(std::to_string(nextCollectionIndex)));
    QString desc = QString::fromStdString(collection.getDescription());
    this->query->prepare("INSERT INTO collections(museumID, collectionID, name, description)"
                  " VALUES ("+museumID+", "+id+", '"+name+"', '"+desc+"')");

    if(!this->query->exec())
    {
        std::string err = this->query->lastError().databaseText().toStdString()+". Cause: "+this->query->lastError().driverText().toStdString();
        throw ModelException("Saving collection failed. Reason: "+err);
    }
    collection.setID(nextCollectionIndex);
    this->query->finish();
}

void ModelClass::updateCollectionInDB(Collection &collection){
    if(!collection.indb())
        {
            throw ModelException("Collection not in database");
        }
        else if (!collection.getMuseum().indb())
        {
            throw ModelException("Museum object of collection does not exist in database");
        }
        else if (!collection.getMuseum().getUser().indb())
        {
            throw ModelException("User object of Museum object of collection does not exist in database");
        }
        else
        {
            try
            {
                Museum museum = this->getMuseumObject(collection.getMuseum().getName());
            } catch (ModelException e)
            {
                throw ModelException("Museum object of collection does not exist in database");
            }

            try
            {
                User user = this->getUserObject(collection.getMuseum().getUser().getName());
            } catch (ModelException e)
            {
                throw ModelException("User object of Museum object of collection does not exist in database");
            }
        }
    QString name = QString::fromStdString(collection.getName());
    QString id = QString::fromStdString(std::to_string(collection.getID()));
    QString description = QString::fromStdString(collection.getDescription());
    bool done = this->query->exec("UPDATE collections SET name = '"+name+"', description = '"+description+"' WHERE collectionID = "+id+";");
    if(!done)
    {
        std::string err = this->query->lastError().databaseText().toStdString()+". Cause: "+this->query->lastError().driverText().toStdString();
        throw ModelException("Collection object failed to update in database. Reason: "+err);
    }
    this->query->finish();
}

void ModelClass::removeCollectionInDB(Collection &collection){
    if(!collection.indb())
    {
        throw ModelException("Collection not in database");
    }
    else if (!collection.getMuseum().indb())
    {
        throw ModelException("Museum object of collection does not exist in database");
    }
    else if (!collection.getMuseum().getUser().indb())
    {
        throw ModelException("User object of Museum object of collection does not exist in database");
    }
    else
    {
        try
        {
            Museum museum = this->getMuseumObject(collection.getMuseum().getName());
        } catch (ModelException e)
        {
            throw ModelException("Museum object of collection does not exist in database");
        }

        try
        {
            User user = this->getUserObject(collection.getMuseum().getUser().getName());
        } catch (ModelException e)
        {
            throw ModelException("User object of Museum object of collection does not exist in database");
        }
    }

    QString id(QString::fromStdString(std::to_string(collection.getID())));
    this->query->exec("DELETE FROM collections WHERE collectionID = "+id+";");
    bool done = this->query->numRowsAffected() == 1;
    if (done)
    {
       collection.setID(-1);
    }
    else
    {
        throw ModelException("Collection object does not exist in database");
    }
    this->query->finish();
}

std::string ModelClass::getMuseumListJSON(){
    this->query->exec("SELECT museumID, userID, name, description FROM museum;");
    this->query->next();
    if (!this->query->isValid())
    {
        throw ModelException("No museum entity stored in database");
    }
    QJsonArray array;
    do{
        QJsonObject object;
        object["name"] = this->query->value(2).toString();
        object["introduction"] = "This is "+ this->query->value(2).toString();
        object["description"] = this->query->value(3).toString();
        object["id"] = this->query->value(0).toString().toInt();
        object["userID"] = this->query->value(1).toString().toInt();
        array.append(object);
    }while(this->query->next());
    QJsonDocument doc;
    doc.setArray(array);
    this->query->finish();
    return doc.toJson().toStdString();
}

std::string ModelClass::getMuseumInfoJSON(int museumID){
    QString id(QString::fromStdString(std::to_string(museumID)));
    this->query->exec("SELECT name, description, userID, museumID FROM museum WHERE museumID = "+id+";");
    this->query->next();
    if (!this->query->isValid())
    {
        throw ModelException("MuseumID does not exist in database");
    }
    QJsonObject object;
    object["name"] = this->query->value(0).toString();
    object["introduction"] = "This is "+ this->query->value(0).toString();
    object["description"] = this->query->value(1).toString();
    object["id"] = this->query->value(3).toString().toInt();
    object["userID"] = this->query->value(2).toString().toInt();
    QJsonDocument doc;
    doc.setObject(object);
    this->query->finish();
    return doc.toJson().toStdString();
}

json ModelClass::getMuseumInfoJson(int museumID){
    QString id(QString::fromStdString(std::to_string(museumID)));
    this->query->exec("SELECT name, description, userID, museumID FROM museum WHERE museumID = "+id+";");
    this->query->next();
    if (!this->query->isValid())
    {
        throw ModelException("MuseumID does not exist in database");
    }
    json output;
    output["name"] = this->query->value(0).toString().toStdString();
    output["introduction"] = "This is "+this->query->value(0).toString().toStdString();
    output["description"] = this->query->value(1).toString().toStdString();
    output["id"] = this->query->value(3).toString().toInt();
    output["userID"] = this->query->value(2).toString().toInt();
    return output;
}

Museum ModelClass::getMuseumObject(std::string museumName){
    QString name = QString::fromStdString(museumName);
    this->query->exec("SELECT museumID, userID, name, description FROM museum where name GLOB '"+name+"';");
    this->query->next();
    if (!this->query->isValid())
    {
        throw ModelException("Museum name does not exist in database");
    }
    int museumID = this->query->value(0).toInt();
    int userID = this->query->value(1).toInt();
    std::string _museumName = this->query->value(2).toString().toStdString();
    std::string description = this->query->value(3).toString().toStdString();
    this->query->finish();
    User user(this->getUserObject(userID));
    return Museum(_museumName, description, user, museumID);
}

void ModelClass::saveMuseumToDB(Museum & museum){
    if (museum.indb())
    {
        throw ModelException("Museum object already in database");
    }
    else if (!museum.getUser().indb())
    {
        throw ModelException("User object of Museum object does not exist in database");
    }
    else
    {
        try
        {
            User user = this->getUserObject(museum.getUser().getName());
        } catch (ModelException e)
        {
            throw ModelException("User object of Museum object does not exist in database");
        }
    }

    int seed = std::chrono::steady_clock::now().time_since_epoch().count();
    std::default_random_engine eng(seed);
    int nextMuseumIndex = eng();
    QString name = QString::fromStdString(museum.getName());
    QString museumID(QString::fromStdString(std::to_string(nextMuseumIndex)));
    QString userID(QString::fromStdString(std::to_string(museum.getUser().getUserID())));
    QString desc = QString::fromStdString(museum.getDescription());
    this->query->prepare("INSERT INTO museum(museumID, userID, name, description)"
                  " VALUES ("+museumID+", "+userID+", '"+name+"', '"+desc+"')");

    if(!this->query->exec())
    {
        std::string err = this->query->lastError().databaseText().toStdString()+". Cause: "+this->query->lastError().driverText().toStdString();
        throw ModelException("Saving museum failed. Reason: "+err);
    }
    museum.setMuseumID(nextMuseumIndex);
    this->query->finish();
}

void ModelClass::removeMuseumFromDB(Museum & museum){
    if (!museum.indb())
    {
        throw ModelException("Museum object not stored in database");
    }
    else if (!museum.getUser().indb())
    {
        throw ModelException("User object of Museum object does not exist in database");
    }
    else
    {
        try
        {
            User user = this->getUserObject(museum.getUser().getName());
        } catch (ModelException e)
        {
            throw ModelException("User object of Museum object does not exist in database");
        }
    }

    QString id(QString::fromStdString(std::to_string(museum.getMuseumID())));
    this->query->exec("DELETE FROM museum WHERE museumID = "+id+";");

    bool done = this->query->numRowsAffected() == 1;
    if (done)
    {
       museum.setMuseumID(-1);
    }
    else
    {
        throw ModelException("Museum object does not exist in database");
    }
    this->query->finish();
}

void ModelClass::updateMuseumInDB(Museum & museum){
    if (!museum.indb())
    {
        throw ModelException("Museum object not stored in database");
    }
    else if (!museum.getUser().indb())
    {
        throw ModelException("User object of Museum object does not exist in database");
    }
    else
    {
        try
        {
            User user = this->getUserObject(museum.getUser().getName());
        } catch (ModelException e)
        {
            throw ModelException("User object of Museum object does not exist in database");
        }
    }

    QString name = QString::fromStdString(museum.getName());
    QString museumID = QString::fromStdString(std::to_string(museum.getMuseumID()));
    QString description = QString::fromStdString(museum.getDescription());
    bool done = this->query->exec("UPDATE museum SET name = '"+name+"', description = '"+description+"' WHERE museumID = "+museumID+";");
    if(!done)
    {
        std::string err = this->query->lastError().databaseText().toStdString()+". Cause: "+this->query->lastError().driverText().toStdString();
        throw ModelException("Museum object failed to update in database. Reason: "+err);
    }
    this->query->finish();
}

std::string ModelClass::getUserInfoJSON(int userID){
    QString id(QString::fromStdString(std::to_string(userID)));
    this->query->exec("SELECT username, email FROM public WHERE userID = "+id+";");
    this->query->next();
    if (!this->query->isValid())
    {
        throw ModelException("UserID does not exist in database");
    }
    QJsonObject object;
    object["username"] = this->query->value(0).toString();
    object["email"] = this->query->value(1).toString();
    QJsonDocument doc;
    doc.setObject(object);
    return doc.toJson().toStdString();
}

json ModelClass::getUserInfoJson(std::string username){
    return this->getUserObject(username).getJson();
}

std::string ModelClass::getPasswordHash(std::string username){
    QString name = QString::fromStdString(username);
    this->query->exec("SELECT password FROM public where username GLOB '"+name+"';");
    this->query->next();
    if (!this->query->isValid())
    {
        throw ModelException("Username does not exist in database");
    }
    std::string output = this->query->value(0).toString().toStdString();
    this->query->finish();
    return output;
}

User ModelClass::getUserObject(std::string username){
    QString name = QString::fromStdString(username);
    this->query->exec("SELECT username, email, password, userID FROM public WHERE username GLOB '"+name+"';");
    this->query->next();
    if (!this->query->isValid())
    {
        throw ModelException("Username does not exist in database");
    }
    std::string uname = this->query->value(0).toString().toStdString();
    std::string email = this->query->value(1).toString().toStdString();
    std::string password = this->query->value(2).toString().toStdString();
    int userID = this->query->value(3).toInt();
    return User(uname, email, password, userID);
}

User ModelClass::getUserObject(int userID){
    QString id(QString::fromStdString(std::to_string(userID)));
    this->query->exec("SELECT username, email, password FROM public WHERE userID = "+id+";");
    this->query->next();
    if (!this->query->isValid())
    {
        throw ModelException("Username does not exist in database");
    }
    std::string uname = this->query->value(0).toString().toStdString();
    std::string email = this->query->value(1).toString().toStdString();
    std::string password = this->query->value(2).toString().toStdString();
    this->query->finish();
    return User(uname, email, password, userID);
}

void ModelClass::saveUserToDB(User & user){
    if (user.indb())
    {
        throw ModelException("User object already in database");
    }
    else if (user.empty())
    {
        throw ModelException("User object empty");
    }

    int seed = std::chrono::steady_clock::now().time_since_epoch().count();
    std::default_random_engine eng(seed);
    int nextUserIndex = eng();
    QString name = QString::fromStdString(user.getName());
    QString userID(QString::fromStdString(std::to_string(nextUserIndex)));
    QString email = QString::fromStdString(user.getEmail());
    QString password = QString::fromStdString(user.getPassword());
    QString queryText("INSERT INTO public(userID,username,email,password) VALUES ("+userID+",'"+name+"','"+email+"','"+password+"');");
    bool done = this->query->exec(queryText);
    if (!done)
    {
        std::string err = this->query->lastError().databaseText().toStdString()+". Cause: "+this->query->lastError().driverText().toStdString();
        throw ModelException("Museum object could not be saved. Reason: "+err);
    }
    user.setUserID(nextUserIndex);
    this->query->finish();
}

void ModelClass::removeUserFromDB(User & user){
    if (!user.indb())
    {
        throw ModelException("User object does not exist in database");
    }
    QString id(QString::fromStdString(std::to_string(user.getUserID())));
    this->query->exec("DELETE FROM public WHERE userID = "+id+";");
    bool done = this->query->numRowsAffected() == 1;
    if (done)
    {
       user.setUserID(-1);
    }
    else
    {
        throw ModelException("User object does not exist in database");
    }
    this->query->finish();
}

void ModelClass::updateUserInDB(User & user){
    if (!user.indb())
    {
        throw ModelException("User object does not exist in database");
    }
    else if (user.empty())
    {
        throw ModelException("User object empty");
    }
    QString email = QString::fromStdString(user.getEmail());
    QString password = QString::fromStdString(user.getPassword());
    QString id = QString::fromStdString(std::to_string(user.getUserID()));
    QString queryText("UPDATE public SET email = '"+email+"', password = '"+password+"' WHERE userID = "+id+";");
    bool done = this->query->exec(queryText);
    if (!done)
    {
        std::string err = this->query->lastError().databaseText().toStdString()+". Cause: "+this->query->lastError().driverText().toStdString();
        throw ModelException("User object could not be updated. Reason: "+err);
    }
    this->query->finish();
}
