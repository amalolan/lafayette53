#include "modelclass.h"
/**
 * @brief ModelClass::ModelClass Opens the database at databasePath, creates the query
 * @param databasePath
 */
ModelClass::ModelClass(std::string databasePath) {
    db = QSqlDatabase::addDatabase("QSQLITE", "Connection");
    db.setDatabaseName(QString::fromStdString(databasePath));
    if(!db.open()){
        qDebug("Model class build failed for linux and macos builds.");
        throw ModelException("Database failed to open.");
    }else{
        qDebug("Model class build successful.");
    }
    query = QSqlQuery(db);
}

/**
 * @brief ModelClass::~ModelClass Closes and removes the database, and destructs the db and query.
 */
ModelClass::~ModelClass() {
    this->db.close();
    this->query.clear();
    {
        this->db = QSqlDatabase::database();
        this->query = QSqlQuery(db);
    }
    QSqlDatabase::removeDatabase("Connection");
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

Collection ModelClass::getCollectionObject(int collectionID){
    QString id(QString::fromStdString(std::to_string(collectionID)));
    query.exec("SELECT collectionID, museumID, name, description FROM collections WHERE collectionID = "+id+";");
    query.next();
    if (!query.isValid())
    {
        throw ModelException("No collection entity stored in database");
    }
    std::string name = query.value(2).toString().toStdString();
    std::string description = query.value(3).toString().toStdString();
    std::string intoduction = "This is "+query.value(2).toString().toStdString();
    Museum museum = this->getMuseumObject(query.value(1).toInt());
    query.finish();
    return Collection(name, description, museum, collectionID);
}

std::vector<Collection> ModelClass::getCollectionListByMuseumID(int museumID){
    std::vector<Collection> collectionList;
    Museum museum = this->getMuseumObject(museumID);
    QString id(QString::fromStdString(std::to_string(museumID)));
    query.exec("SELECT collectionID, museumID, name, description FROM collections WHERE museumID = "+id+";");
    query.next();
    if (!query.isValid())
    {
        //no exception should be thrown. just returned empty list.
        //throw ModelException("No collection entity stored in database");
        return collectionList;
    }
    do
    {

        std::string name = query.value(2).toString().toStdString();
        std::string description = query.value(3).toString().toStdString();
        int id = query.value(0).toInt();
        std::string intoduction = "This is "+query.value(2).toString().toStdString();
        collectionList.push_back(Collection(name, description, museum, id));
    }while(query.next());
    query.finish();
    return collectionList;
}

/**
 * @brief ModelClass::getMuseumList
 * @return
 */
std::vector<Museum> ModelClass::getMuseumList(){
    query.exec("SELECT museumID, userID, name, description FROM museum;");
    std::vector<Museum> museumList;
    this->query.next();
    if (!this->query.isValid())
    {
        throw ModelException("No museum entity stored in database");
    }
    if (!query.isValid())
    {
        throw ModelException("No museum entity stored in database");
    }
    do{
        std::string name = query.value(2).toString().toStdString();
        std::string introduction = "This is "+ query.value(2).toString().toStdString();
        std::string description = query.value(3).toString().toStdString();
        int id = query.value(0).toString().toInt();
        int userID = query.value(1).toString().toInt();
        std::cerr<<"Name of museum being pushed back " <<name<<std::endl;
        User user("","","");
        user.setUserID(userID);
        museumList.push_back(Museum(name, description, user, id));
    }while(query.next());
    query.finish();

    for (Museum museum : museumList)
    {
        museum.setUser(this->getUserObject(museum.getUser().getUserID()));
    }
    return museumList;
}
/**
 * @brief ModelClass::getMuseumListJSON Returns a list of museums in the database as a JSON ARRAY
 * Please remove this function once the erro with  getMuseumList() is fixed.
 * @return A JSON array of schema below
 * [
 *  {"name": string,
 *   "introduction": string,
 *   "description": string,
 *   "id": int,
 *   "userID": int
 * },
 * ....
 * ]
 */
json ModelClass::getMuseumListJSON() {
    query.exec("SELECT museumID, userID, name, description FROM museum;");
    this->query.next();
    if (!this->query.isValid())
    {
        throw ModelException("No museum entity stored in database");
    }
    json array = json::array();
    do{
        json object;
        object["name"] = this->query.value(2).toString().toStdString();
        object["introduction"] = "This is "+ this->query.value(2).toString().toStdString();
        object["description"] = this->query.value(3).toString().toStdString();
        object["id"] = this->query.value(0).toString().toInt();
        object["userID"] = this->query.value(1).toString().toInt();
        array.push_back(object);
    }while(this->query.next());
    this->query.finish();
    return array;
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

    int seed = std::chrono::steady_clock::now().time_since_epoch().count();
    std::default_random_engine eng(seed);
    int nextCollectionIndex = eng();
    QString name = QString::fromStdString(collection.getName());
    QString museumID(QString::fromStdString(std::to_string(collection.getMuseum().getMuseumID())));
    QString id(QString::fromStdString(std::to_string(nextCollectionIndex)));
    QString desc = QString::fromStdString(collection.getDescription());
    query.prepare("INSERT INTO collections(museumID, collectionID, name, description)"
                  " VALUES ("+museumID+", "+id+", '"+name+"', '"+desc+"')");

    if(!query.exec())
    {
        std::string err = query.lastError().databaseText().toStdString()+". Cause: "+query.lastError().driverText().toStdString();
        throw ModelException("Saving collection failed. Reason: "+err);
    }
    collection.setID(nextCollectionIndex);
    query.finish();
    std::cout<<"Saved collection at  ID "<<nextCollectionIndex;
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
    bool done = query.exec("UPDATE collections SET name = '"+name+"', description = '"+description+"' WHERE collectionID = "+id+";");
    if(!done)
    {
        std::string err = query.lastError().databaseText().toStdString()+". Cause: "+query.lastError().driverText().toStdString();
        throw ModelException("Collection object failed to update in database. Reason: "+err);
    }
    query.finish();
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
    query.exec("DELETE FROM collections WHERE collectionID = "+id+";");
    bool done = query.numRowsAffected() == 1;
    if (done)
    {
       collection.setID(-1);
    }
    else
    {
        throw ModelException("Collection object does not exist in database");
    }
    query.finish();
}

Museum ModelClass::getMuseumObject(int museumID){
    QString id(QString::fromStdString(std::to_string(museumID)));
    query.exec("SELECT name, description, userID, museumID FROM museum WHERE museumID = "+id+";");
    query.next();
    if (!query.isValid())
    {
        throw ModelException("MuseumID does not exist in database");
    }
    QJsonObject object;
    std::string name = query.value(0).toString().toStdString();
    std::string introduction = "This is "+ query.value(0).toString().toStdString();
    std::string description = query.value(1).toString().toStdString();
    int userID = query.value(2).toString().toInt();
    query.finish();
    User curator = this->getUserObject(userID);
    return Museum(name, description, curator, museumID);

}

Museum ModelClass::getMuseumObject(std::string museumName){
    QString name = QString::fromStdString(museumName);
    query.exec("SELECT museumID, userID, name, description FROM museum where name GLOB '"+name+"';");
    query.next();
    if (!query.isValid())
    {
        throw ModelException("Museum name does not exist in database");
    }
    int museumID = query.value(0).toInt();
    int userID = query.value(1).toInt();
    std::string _museumName = query.value(2).toString().toStdString();
    std::string description = query.value(3).toString().toStdString();
    query.finish();
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
    query.prepare("INSERT INTO museum(museumID, userID, name, description)"
                  " VALUES ("+museumID+", "+userID+", '"+name+"', '"+desc+"')");

    if(!query.exec())
    {
        std::string err = query.lastError().databaseText().toStdString()+". Cause: "+query.lastError().driverText().toStdString();
        throw ModelException("Saving museum failed. Reason: "+err);
    }
    museum.setMuseumID(nextMuseumIndex);
    query.finish();
    std::cout<<"Saved Museum at  ID "<<nextMuseumIndex;
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
    query.exec("DELETE FROM museum WHERE museumID = "+id+";");

    bool done = query.numRowsAffected() == 1;
    if (done)
    {
       museum.setMuseumID(-1);
    }
    else
    {
        throw ModelException("Museum object does not exist in database");
    }
    query.finish();
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
    bool done = query.exec("UPDATE museum SET name = '"+name+"', description = '"+description+"' WHERE museumID = "+museumID+";");
    if(!done)
    {
        std::string err = query.lastError().databaseText().toStdString()+". Cause: "+query.lastError().driverText().toStdString();
        throw ModelException("Museum object failed to update in database. Reason: "+err);
    }
    query.finish();
}

User ModelClass::getUserObject(std::string username){
    QString name = QString::fromStdString(username);
    query.exec("SELECT username, email, password, userID FROM public WHERE username GLOB '"+name+"';");
    query.next();
    if (!query.isValid())
    {
        throw ModelException("Username does not exist in database");
    }
    std::string uname = query.value(0).toString().toStdString();
    std::string email = query.value(1).toString().toStdString();
    std::string password = query.value(2).toString().toStdString();
    int userID = query.value(3).toInt();
    return User(uname, email, password, userID);
}

User ModelClass::getUserObject(int userID){
    QString id(QString::fromStdString(std::to_string(userID)));
    query.exec("SELECT username, email, password FROM public WHERE userID = "+id+";");
    query.next();
    if (!query.isValid())
    {
        throw ModelException("Username does not exist in database");
    }
    std::string uname = query.value(0).toString().toStdString();
    std::string email = query.value(1).toString().toStdString();
    std::string password = query.value(2).toString().toStdString();
    query.finish();
    return User(uname, email, password, userID);
}

void ModelClass::saveUserToDB(User & user){
    if (user.indb())
    {
        std::cout<<"In db";
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
    bool done = query.exec(queryText);
    if (!done)
    {
        std::string err = query.lastError().databaseText().toStdString()+". Cause: "+query.lastError().driverText().toStdString();
        throw ModelException("Museum object could not be saved. Reason: "+err);
    }
    user.setUserID(nextUserIndex);
    query.finish();
}

void ModelClass::removeUserFromDB(User & user){
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
        std::cout<<"Help";
        throw ModelException("User object does not exist in database");
    }
    query.finish();
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
    bool done = query.exec(queryText);
    if (!done)
    {
        std::string err = query.lastError().databaseText().toStdString()+". Cause: "+query.lastError().driverText().toStdString();
        throw ModelException("User object could not be updated. Reason: "+err);
    }
    query.finish();
}
