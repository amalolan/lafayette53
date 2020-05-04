#include "modelclass.h"

bool ModelClass::instanceFlagTest = false;

bool ModelClass::instanceFlagPro = false;

const bool ModelClass::test = false;

const bool ModelClass::pro = true;

std::string ModelClass::path = "";

ModelClass * ModelClass::singlePro = nullptr;

ModelClass * ModelClass::singleTest = nullptr;

void ModelClass::initdb(std::string databasePath){
    ModelClass::path = databasePath;
    ModelClass::setup();
}

void ModelClass::setup(){
    std::vector<std::string> array = {"database/testdb.db", "database/db.db"};
    for (std::string item : array)
    {
        std::string currPath = ModelClass::path + item;
        std::ifstream file;
        file.open(currPath);
        if (!file)
        {
            std::cout << "Will create if necessary database at " + currPath << std::endl;
        }
        else
        {
            std::cout << "Database at " + currPath + " already exists." << std::endl;
        }
        file.close();
    }
}

ModelClass* ModelClass::getInstance(bool kind){
    if (kind)
    {
        if (!ModelClass::instanceFlagPro)
        {
            ModelClass::singlePro = new ModelClass(ModelClass::path + "database/db.db", ModelClass::pro);
            ModelClass::instanceFlagPro = true;
        }
        return ModelClass::singlePro;
    }
    else
    {
        if(!ModelClass::instanceFlagTest)
        {
            ModelClass::singleTest = new ModelClass(ModelClass::path + "database/testdb.db", ModelClass::test);
            ModelClass::instanceFlagTest = true;
        }
        return ModelClass::singleTest;
    }
}


/**
 * @brief ModelClass::ModelClass Opens the database at databasePath, creates the query
 * @param databasePath
 */
ModelClass::ModelClass(std::string databasePath) {
    this->kind = true;
    db = QSqlDatabase::addDatabase("QSQLITE", "Connection");
    db.setDatabaseName(QString::fromStdString(databasePath));
    if(!db.open()){
        qDebug("Model class build failed for linux and macos builds.");
        throw ModelException("Database failed to open.");
    }else{
        qDebug("Model class build successful.");
    }
    query = QSqlQuery(db);
    query.exec("PRAGMA foreign_keys = ON;");
}

ModelClass::ModelClass(std::string path, bool kind){
    this->kind = kind;
    db = QSqlDatabase::addDatabase("QSQLITE", "Connection");
    db.setDatabaseName(QString::fromStdString(path));
    if(!db.open()){
        qDebug("Model class build failed for linux and macos builds.");
        throw ModelException("Database failed to open.");
    }else{
        qDebug("Model class build successful.");
    }
    query = QSqlQuery(db);
    query.exec("PRAGMA foreign_keys = ON;");
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
    if (this->kind == ModelClass::test)
    {
        ModelClass::instanceFlagTest = false;
    }
    else
    {
        ModelClass::instanceFlagPro = false;
    }
}

bool ModelClass::open(){
    if(!db.isOpen()){
        db.open();
        query.exec("PRAGMA foreign_keys = ON;");
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

void ModelClass::createTables(){
    QString q =     "CREATE TABLE IF NOT EXISTS "
                    "public("
                    "userID INTEGER NOT NULL PRIMARY KEY, "
                    "username TEXT NOT NULL UNIQUE, "
                    "email TEXT NOT NULL UNIQUE, "
                    "password TEXT NOT NULL, "
                    "curator INTEGER DEFAULT 0, "
                    "photo TEXT"
                    ");";
    if (!query.exec(q))
    {
        throw ModelException("Public table failed to create.");
    }

    q =             "CREATE TABLE IF NOT EXISTS "
                    "museum("
                    "museumID INTEGER NOT NULL PRIMARY KEY, "
                    "userID INTEGER NOT NULL, "
                    "name TEXT NOT NULL UNIQUE, "
                    "introduction TEXT, "
                    "description TEXT, "
                    "photo TEXT, "
                    "FOREIGN KEY(userID) REFERENCES public(userID) "
                    "ON DELETE CASCADE "
                    "ON UPDATE CASCADE"
                    ");";
    if (!query.exec(q))
    {
        throw ModelException("Museum table failed to create.");
    }

    q =             "CREATE TABLE IF NOT EXISTS "
                    "collections("
                    "collectionID INTEGER NOT NULL PRIMARY KEY, "
                    "museumID INTEGER NOT NULL, "
                    "name TEXT NOT NULL UNIQUE, "
                    "introduction TEXT, "
                    "description TEXT, "
                    "photo TEXT, "
                    "FOREIGN KEY(museumID) REFERENCES museum(museumID) "
                    "ON DELETE CASCADE "
                    "ON UPDATE CASCADE"
                    ");";
    if (!query.exec(q))
    {
        throw ModelException("Collections table failed to create.");
    }

    q =             "CREATE TABLE IF NOT EXISTS "
                    "artifacts("
                    "artifactID INTEGER NOT NULL PRIMARY KEY, "
                    "museumID INTEGER NOT NULL, "
                    "name TEXT NOT NULL UNIQUE, "
                    "introduction TEXT, "
                    "description TEXT, "
                    "photo TEXT, "
                    "FOREIGN KEY(museumID) REFERENCES museum(museumID) "
                    "ON DELETE CASCADE "
                    "ON UPDATE CASCADE"
                    ");";
    if (!query.exec(q))
    {
        throw ModelException("Artifacts table failed to create.");
    }

    q =             "CREATE TABLE IF NOT EXISTS "
                    "artifactCollection("
                    "artifactID INTEGER NOT NULL, "
                    "collectionID INTEGER NOT NULL, "
                    "FOREIGN KEY(artifactID) REFERENCES artifacts(artifactID) "
                    "ON DELETE CASCADE "
                    "ON UPDATE CASCADE, "
                    "FOREIGN KEY(collectionID) REFERENCES collections(collectionID) "
                    "ON DELETE CASCADE "
                    "ON UPDATE CASCADE, "
                    "PRIMARY KEY(artifactID, collectionID)"
                    ");";
    if (!query.exec(q))
    {
        throw ModelException("ArtifactCollection table failed to create.");
    }

    q =             "CREATE TABLE IF NOT EXISTS "
                    "edit("
                    "editID INTEGER NOT NULL, "
                    "kind INTEGER NOT NULL, "
                    "status INTEGER NOT NULL, "
                    "museumID INTEGER DEFAULT -2, "
                    "collectionID INTEGER DEFAULT -2, "
                    "artifactID INTEGER DEFAULT -2, "
                    "userID INTEGER NOT NULL, "
                    "name TEXT NOT NULL, "
                    "description TEXT, "
                    "introduction TEXT, "
                    "photo TEXT, "
                    "list TEXT, "
                    "FOREIGN KEY(artifactID) REFERENCES artifacts(artifactID) "
                    "ON DELETE CASCADE "
                    "ON UPDATE CASCADE, "
                    "FOREIGN KEY(collectionID) REFERENCES collections(collectionID) "
                    "ON DELETE CASCADE "
                    "ON UPDATE CASCADE, "
                    "FOREIGN KEY(museumID) REFERENCES museum(museumID) "
                    "ON DELETE CASCADE "
                    "ON UPDATE CASCADE, "
                    "FOREIGN KEY(userID) REFERENCES public(userID) "
                    "ON DELETE CASCADE "
                    "ON UPDATE CASCADE, "
                    "PRIMARY KEY(editID)"
                    ");";
    if (!query.exec(q))
    {
        throw ModelException("ArtifactCollection table failed to create.");
    }
}

void ModelClass::artifactCheck(const Artifact & artifact){
    if(!artifact.indb())
    {
        throw ModelException("Artifact does not exists in database");
    }
    else if (!artifact.getMuseum().indb())
    {
        throw ModelException("Museum object of artifact does not exist in database");
    }
    else if (!artifact.getMuseum().getUser().indb())
    {
        throw ModelException("User object of Museum object of artifact does not exist in database");
    }
    else
    {
        try
        {
            Museum museum = this->getMuseumObject(artifact.getMuseum().getName());
        } catch (ModelException e)
        {
            throw ModelException("Museum object of artifact does not exist in database");
        }

        try
        {
            User user = this->getUserObject(artifact.getMuseum().getUser().getName());
        } catch (ModelException e)
        {
            throw ModelException("User object of Museum object of artifact does not exist in database");
        }
    }
}

void ModelClass::artifactPreCheck(const Artifact & artifact){
    if(artifact.indb())
    {
        throw ModelException("Artifact already exists in database");
    }
    else if (!artifact.getMuseum().indb())
    {
        throw ModelException("Museum object of artifact does not exist in database");
    }
    else if (!artifact.getMuseum().getUser().indb())
    {
        throw ModelException("User object of Museum object of artifact does not exist in database");
    }
    else
    {
        try
        {
            Museum museum = this->getMuseumObject(artifact.getMuseum().getName());
        } catch (ModelException e)
        {
            throw ModelException("Museum object of artifact does not exist in database");
        }

        try
        {
            User user = this->getUserObject(artifact.getMuseum().getUser().getName());
        } catch (ModelException e)
        {
            throw ModelException("User object of Museum object of artifact does not exist in database");
        }
    }
}

void ModelClass::collectionCheck(const Collection & collection){
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
}

void ModelClass::preCollectionCheck(const Collection & collection){
    if(collection.indb())
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
}

void ModelClass::museumCheck(const Museum & museum){
    if (!museum.indb())
    {
        throw ModelException("Museum object not stored in database");
    }
    else if(museum.empty())
    {
        throw ModelException("Museum object empty");
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
}

void ModelClass::saveEditToDB(Edit<Museum> & edit){
    if (edit.indb())
    {
        throw ModelException("Edit object already in database");
    }
    museumCheck(edit.getObject());
    query.exec("PRAGMA foreign_keys = OFF;");
    int seed = std::chrono::steady_clock::now().time_since_epoch().count();
    std::default_random_engine eng(seed);
    int nextEditIndex = eng();
    QString editID = QString::fromStdString(std::to_string(nextEditIndex));
    QString museumID = QString::fromStdString(std::to_string(edit.getObject().getMuseumID()));
    QString description = QString::fromStdString(edit.getObject().getDescription());
    QString intro = QString::fromStdString(edit.getObject().getIntro());
    QString photo = QString::fromStdString(edit.getObject().getPhoto());
    QString name = QString::fromStdString(edit.getObject().getName());
    QString userID = QString::fromStdString(std::to_string(edit.getEditor().getUserID()));
    QString status = QString::fromStdString(std::to_string(edit.getStatus()));
    QString kind = QString::fromStdString(std::to_string(edit.getKind()));

    bool done =
    query.exec("INSERT INTO edit(museumID, editID, userID, name, description, photo, introduction, kind, status)"
               " VALUES ("+museumID+", "+editID+", "+userID+", '"+name+"', '"+description+"', '"+photo+"', "
                 "'"+intro+"', "+kind+", "+status+")");

    if(!done)
    {
        std::string err = query.lastError().databaseText().toStdString()+". Cause: "+query.lastError().driverText().toStdString();
        throw ModelException("Saving Edit<Museum> failed. Reason: "+err);
    }
    edit.setID(nextEditIndex);
    query.exec("PRAGMA foreign_keys = ON;");
    query.finish();
}

void ModelClass::saveEditToDB(Edit<Collection> & edit){
    if (edit.indb())
    {
        throw ModelException("Edit object already in database");
    }
    else if (edit.getKind() != Edit<Collection>::add)
    {
        collectionCheck(edit.getObject());
    }
    else
    {
        preCollectionCheck(edit.getObject());
    }
    query.exec("PRAGMA foreign_keys = OFF;");
    int seed = std::chrono::steady_clock::now().time_since_epoch().count();
    std::default_random_engine eng(seed);
    int nextEditIndex = eng();
    QString editID = QString::fromStdString(std::to_string(nextEditIndex));
    QString collectionID = QString::fromStdString(std::to_string(edit.getObject().getID()));
    QString museumID = QString::fromStdString(std::to_string(edit.getObject().getMuseum().getMuseumID()));
    QString description = QString::fromStdString(edit.getObject().getDescription());
    QString intro = QString::fromStdString(edit.getObject().getIntro());
    QString photo = QString::fromStdString(edit.getObject().getPhoto());
    QString name = QString::fromStdString(edit.getObject().getName());
    QString status = QString::fromStdString(std::to_string(edit.getStatus()));
    QString kind = QString::fromStdString(std::to_string(edit.getKind()));
    QString userID = QString::fromStdString(std::to_string(edit.getEditor().getUserID()));
    bool done =
    query.exec("INSERT INTO edit(museumID, editID, userID, collectionID, name, description, photo, introduction, kind, status)"
               " VALUES ("+museumID+", "+editID+", "+userID+", "+collectionID+", '"+name+"', '"+description+"', '"+photo+"', "
                 "'"+intro+"', "+kind+", "+status+")");

    if(!done)
    {
        std::string err = query.lastError().databaseText().toStdString()+". Cause: "+query.lastError().driverText().toStdString();
        throw ModelException("Saving Edit<Collection> failed. Reason: "+err);
    }
    edit.setID(nextEditIndex);
    query.exec("PRAGMA foreign_keys = ON;");
    query.finish();
}

void ModelClass::saveEditToDB(Edit<Artifact> & edit){
    if (edit.indb())
    {
        throw ModelException("Edit object already in database");
    }
    //TODO those have to be fixed some typos.
    else if (edit.getKind() != Edit<Artifact>::add){
        artifactCheck(edit.getObject());
    }
    else
    {
        artifactPreCheck(edit.getObject());
    }
    query.exec("PRAGMA foreign_keys = OFF;");
    int seed = std::chrono::steady_clock::now().time_since_epoch().count();
    std::default_random_engine eng(seed);
    int nextEditIndex = eng();
    QString editID = QString::fromStdString(std::to_string(nextEditIndex));
    QString artifactID = QString::fromStdString(std::to_string(edit.getObject().getID()));
    QString museumID = QString::fromStdString(std::to_string(edit.getObject().getMuseum().getMuseumID()));
    QString description = QString::fromStdString(edit.getObject().getDescription());
    QString intro = QString::fromStdString(edit.getObject().getIntro());
    QString photo = QString::fromStdString(edit.getObject().getPhoto());
    QString name = QString::fromStdString(edit.getObject().getName());
    QString status = QString::fromStdString(std::to_string(edit.getStatus()));
    QString kind = QString::fromStdString(std::to_string(edit.getKind()));
    QString userID = QString::fromStdString(std::to_string(edit.getEditor().getUserID()));

    std::string stdList;
    for(unsigned long i = 0; i < edit.getCollectionList().size(); i++)
    {
        stdList += std::to_string(edit.getCollectionList().at(i).getID());

        if (i != edit.getCollectionList().size() - 1)
        {
            stdList += " ";
        }
    }
    QString list = QString::fromStdString(stdList);
    bool done =
    query.exec("INSERT INTO edit(editID, museumID, userID, artifactID, name, description, photo, introduction, kind, status, list)"
               " VALUES ("+editID+", "+museumID+", "+userID+", "+artifactID+", '"+name+"', '"+description+"', '"+photo+"', "
                 "'"+intro+"', "+kind+", "+status+", '"+list+"')");

    if(!done)
    {
        std::string err = query.lastError().databaseText().toStdString()+". Cause: "+query.lastError().driverText().toStdString();
        throw ModelException("Saving Edit<Artifact> failed. Reason: "+err);
    }
    edit.setID(nextEditIndex);
    query.exec("PRAGMA foreign_keys = ON;");
    query.finish();
}

void ModelClass::updateEditInDB(Edit<Collection> & edit){
    if (!edit.indb())
    {
        throw ModelException("Edit object not in database");
    }
    else if (edit.getKind()!= Edit<Artifact>::add){
        collectionCheck(edit.getObject());
    }
    else
    {
        preCollectionCheck(edit.getObject());
    }
    query.exec("PRAGMA foreign_keys = OFF;");
    QString status = QString::fromStdString(std::to_string(edit.getStatus()));
    QString id = QString::fromStdString(std::to_string(edit.getID()));
    bool done =
    query.exec("UPDATE edit SET status = "+status+" WHERE editID = "+id+";");

    if(!done)
    {
        std::string err = query.lastError().databaseText().toStdString()+". Cause: "+query.lastError().driverText().toStdString();
        throw ModelException("Saving Edit<Museum> failed. Reason: "+err);
    }
    query.exec("PRAGMA foreign_keys = ON;");
    query.finish();
    if (edit.getStatus() != Edit<Artifact>::approve)
    {
        return;
    }

    Collection collection = edit.getObject();
    if (edit.getKind() == Edit<Collection>::add)
    {
        this->saveCollectionToDB(collection);
        QString collectionID = QString::fromStdString(std::to_string(collection.getID()));
        query.exec("PRAGMA foreign_keys = OFF;");
        query.exec("UPDATE edit SET collectionID = "+collectionID+" WHERE editID = "+id+";");
        query.exec("PRAGMA foreign_keys = ON;");
        edit.setObject(collection);
    }
    else if (edit.getKind() == Edit<Collection>::edit)
    {
        this->updateCollectionInDB(collection);
    }
    else if(edit.getKind() == Edit<Collection>::del)
    {
        query.exec("PRAGMA foreign_keys = OFF;");
        query.exec("UPDATE edit SET collectionID = -1 WHERE editID = "+id+";");
        query.exec("PRAGMA foreign_keys = ON;");
        this->removeCollectionInDB(collection);
        edit.setObject(collection);
    }
}

void ModelClass::updateEditInDB(Edit<Artifact> & edit){
    if (!edit.indb())
    {
        throw ModelException("Edit object not in database");
    }
    else if (edit.getKind() != Edit<Artifact>::add){
        artifactCheck(edit.getObject());
    }
    else
    {
        artifactPreCheck(edit.getObject());
    }
    query.exec("PRAGMA foreign_keys = OFF;");
    QString status = QString::fromStdString(std::to_string(edit.getStatus()));
    QString id = QString::fromStdString(std::to_string(edit.getID()));
    QString cmd("UPDATE edit SET status = "+status+" WHERE editID = "+id+";");
    bool done = query.exec(cmd);
    if(!done)
    {
        std::string err = query.lastError().databaseText().toStdString()+". Cause: "+query.lastError().driverText().toStdString();
        throw ModelException("Saving Edit<Artifact> failed. Reason: "+err);
    }
    query.exec("PRAGMA foreign_keys = ON;");
    query.finish();

    if (edit.getStatus() != Edit<Artifact>::approve)
    {
        return;
    }

    Artifact artifact = edit.getObject();
    if (edit.getKind() == Edit<Artifact>::add)
    {
        this->saveArtifactToDB(artifact);
        QString artifactID = QString::fromStdString(std::to_string(artifact.getID()));
        query.exec("PRAGMA foreign_keys = OFF;");
        query.exec("UPDATE edit SET artifactID = "+artifactID+" WHERE editID = "+id+";");
        query.exec("PRAGMA foreign_keys = ON;");
        edit.setObject(artifact);
    }
    else if (edit.getKind() == Edit<Artifact>::edit)
    {
        this->updateArtifactInDB(artifact);
    }
    else if(edit.getKind() == Edit<Artifact>::del)
    {
        query.exec("PRAGMA foreign_keys = OFF;");
        query.exec("UPDATE edit SET artifactID = -1 WHERE editID = "+id+";");
        query.exec("PRAGMA foreign_keys = ON;");
        this->removeArtifactInDB(artifact);
        edit.setObject(artifact);
    }
}

void ModelClass::updateEditInDB(Edit<Museum> & edit){
    if (!edit.indb())
    {
        throw ModelException("Edit object not in database");
    }
    museumCheck(edit.getObject());
    query.exec("PRAGMA foreign_keys = OFF;");
    QString status = QString::fromStdString(std::to_string(edit.getStatus()));
    QString id = QString::fromStdString(std::to_string(edit.getID()));
    bool done = query.exec("UPDATE edit SET status = "+status+" WHERE editID = "+id+";");

    if(!done)
    {
        std::string err = query.lastError().databaseText().toStdString()+". Cause: "+query.lastError().driverText().toStdString();
        throw ModelException("Saving Edit<Museum> failed. Reason: "+err);
    }
    query.exec("PRAGMA foreign_keys = ON;");
    query.finish();

    if (edit.getStatus() != Edit<Museum>::approve)
    {
        return;
    }

    Museum museum = edit.getObject();
    if (edit.getKind() == Edit<Museum>::edit)
    {
        this->updateMuseumInDB(museum);
    }
}

void ModelClass::removeEditInDB(Edit<Collection> & edit){
    if (!edit.indb())
    {
        throw ModelException("Edit object not in database");
    }
    QString id = QString::fromStdString(std::to_string(edit.getID()));
    query.exec("DELETE FROM edit WHERE editID = "+id+";");
    bool done = query.numRowsAffected() == 1;

    if (!done)
    {
        std::string err = query.lastError().databaseText().toStdString()+". Cause: "+query.lastError().driverText().toStdString();
        throw ModelException("Deleting museum object failed. Reason: "+err);
        throw ModelException(err);
    }
    edit.setID(-1);
}

void ModelClass::removeEditInDB(Edit<Artifact> & edit){
    if (!edit.indb())
    {
        throw ModelException("Edit object not in database");
    }
    QString id = QString::fromStdString(std::to_string(edit.getID()));
    query.exec("DELETE FROM edit WHERE editID = "+id+";");
    bool done = query.numRowsAffected() == 1;

    if (!done)
    {
        std::string err = query.lastError().databaseText().toStdString()+". Cause: "+query.lastError().driverText().toStdString();
        throw ModelException("Deleting museum object failed. Reason: "+err);
        throw ModelException(err);
    }
    edit.setID(-1);
}

void ModelClass::removeEditInDB(Edit<Museum> & edit){
    if (!edit.indb())
    {
        throw ModelException("Edit object not in database");
    }
    QString id = QString::fromStdString(std::to_string(edit.getID()));
    query.exec("DELETE FROM edit WHERE editID = "+id+";");
    bool done = query.numRowsAffected() == 1;

    if (!done)
    {
        std::string err = query.lastError().databaseText().toStdString()+". Cause: "+query.lastError().driverText().toStdString();
        throw ModelException("Deleting museum object failed. Reason: "+err);
        throw ModelException(err);
    }
    edit.setID(-1);
}

std::vector<Edit<Museum>> ModelClass::getMuseumEdits(int userID){
    QString id(QString::fromStdString(std::to_string(userID)));
    User user = this->getUserObject(userID);
    bool done = query.exec
    ("SELECT museumID, name, description, photo, introduction, status, kind, editID"
     " FROM edit WHERE userID = "+id+" AND collectionID = -2 AND artifactID = -2;");
    if (!done)
    {
        throw ModelException
        ("Failure retrieving data from db. Reason: "+query.lastError().databaseText().toStdString()
         +" Cause: "+query.lastError().driverText().toStdString());
    }
    std::vector<Edit<Museum>> output;
    query.next();
    if (!query.isValid())
    {
        return output;
    }
    std::vector<std::vector<std::string>> queryList;
    do
    {
        std::vector<std::string> rowList;
        rowList.push_back(query.value(0).toString().toStdString());
        rowList.push_back(query.value(1).toString().toStdString());
        rowList.push_back(query.value(2).toString().toStdString());
        rowList.push_back(query.value(3).toString().toStdString());
        rowList.push_back(query.value(4).toString().toStdString());
        rowList.push_back(query.value(5).toString().toStdString());
        rowList.push_back(query.value(6).toString().toStdString());
        rowList.push_back(query.value(7).toString().toStdString());
        queryList.push_back(rowList);
    }while (query.next());

    for (std::vector<std::string> row : queryList)
    {
        Museum museum = this->getMuseumObject(std::stoi(row.at(0)));
        museum.setName(row.at(1));
        museum.setDescription(row.at(2));
        museum.setIntro(row.at(4));
        museum.setPhoto(row.at(3));
        Edit<Museum> edit(museum, std::stoi(row.at(6)), user, std::stoi(row.at(5)), std::stoi(row.at(7)));
        output.push_back(edit);
    }
    return output;
}

std::vector<Edit<Collection>> ModelClass::getCollectionEdits(int userID){
    QString id(QString::fromStdString(std::to_string(userID)));
    User user = this->getUserObject(userID);
    bool done = query.exec
    ("SELECT collectionID, name, description, photo, introduction, status, kind, editID, museumID"
     " FROM edit WHERE userID = "+id+" AND artifactID == -2;");

    if (!done)
    {
        throw ModelException
        ("Failure retrieving data from db. Reason: "+query.lastError().databaseText().toStdString()
         +" Cause: "+query.lastError().driverText().toStdString());
    }
    std::vector<Edit<Collection>> output;
    query.next();
    if (!query.isValid())
    {
        return output;
    }

    std::vector<std::vector<std::string>> queryList;
    do
    {
        std::vector<std::string> rowList;
        rowList.push_back(query.value(0).toString().toStdString());
        rowList.push_back(query.value(1).toString().toStdString());
        rowList.push_back(query.value(2).toString().toStdString());
        rowList.push_back(query.value(3).toString().toStdString());
        rowList.push_back(query.value(4).toString().toStdString());
        rowList.push_back(query.value(5).toString().toStdString());
        rowList.push_back(query.value(6).toString().toStdString());
        rowList.push_back(query.value(7).toString().toStdString());
        rowList.push_back(query.value(8).toString().toStdString());
        queryList.push_back(rowList);
    }while (query.next());

    for (std::vector<std::string> row : queryList)
    {
        Collection collection = Collection("","","","",this->getMuseumObject(std::stoi(row.at(8))));
        collection.setName(row.at(1));
        collection.setDescription(row.at(2));
        collection.setIntro(row.at(4));
        collection.setPhoto(row.at(3));
        collection.setID(std::stoi(row.at(0)));
        Edit<Collection> edit(collection, std::stoi(row.at(6)), user, std::stoi(row.at(5)), std::stoi(row.at(7)));
        output.push_back(edit);
    }
    return output;
}

std::vector<Edit<Artifact>> ModelClass::getArtifactEdits(int userID){
    QString id(QString::fromStdString(std::to_string(userID)));
    User user = this->getUserObject(userID);
    bool done = query.exec
    ("SELECT artifactID, name, description, photo, introduction, status, kind, editID, list, museumID"
     " FROM edit WHERE userID = "+id+" AND artifactID != -2;");
    if (!done)
    {
        throw ModelException
        ("Failure retrieving data from db. Reason: "+query.lastError().databaseText().toStdString()
         +" Cause: "+query.lastError().driverText().toStdString());
    }
    std::vector<Edit<Artifact>> output;
    query.next();
    if (!query.isValid())
    {
        return output;
    }
    std::vector<std::vector<std::string>> queryList;
    do
    {
        std::vector<std::string> rowList;
        rowList.push_back(query.value(0).toString().toStdString());
        rowList.push_back(query.value(1).toString().toStdString());
        rowList.push_back(query.value(2).toString().toStdString());
        rowList.push_back(query.value(3).toString().toStdString());
        rowList.push_back(query.value(4).toString().toStdString());
        rowList.push_back(query.value(5).toString().toStdString());
        rowList.push_back(query.value(6).toString().toStdString());
        rowList.push_back(query.value(7).toString().toStdString());
        rowList.push_back(query.value(8).toString().toStdString());
        rowList.push_back(query.value(9).toString().toStdString());
        queryList.push_back(rowList);
    }while (query.next());

    for (std::vector<std::string> row : queryList)
    {
        Artifact artifact("","","","", this->getMuseumObject(std::stoi(row.at(9))));
        artifact.setName(row.at(1));
        artifact.setDescription(row.at(2));
        artifact.setIntro(row.at(4));
        artifact.setPhoto(row.at(3));
        artifact.setID(std::stoi(row.at(0)));
        std::vector<Collection> list;
        std::stringstream ss(row.at(8));
        std::string token;
        while (std::getline(ss, token, ' '))
        {
            try
            {
                list.push_back(this->getCollectionObject(std::stoi(token)));
            } catch (ModelException e)
            {

            }
        }
        Edit<Artifact> edit(artifact, std::stoi(row.at(6)), user, list, std::stoi(row.at(5)), std::stoi(row.at(7)));
        output.push_back(edit);
    }
    return output;
}

std::vector<Edit<Museum>> ModelClass::getMuseumActions(int museumID){
    Museum museum = this->getMuseumObject(museumID);
    QString id(QString::fromStdString(std::to_string(museumID)));
    query.exec
    ("SELECT userID, name, description, photo, introduction, status, kind, editID"
     " FROM edit WHERE museumID = "+id+" AND collectionID = -2 AND artifactID = -2 AND status = 0;");

    std::vector<Edit<Museum>> output;
    query.next();
    if (!query.isValid())
    {
        return output;
    }
    std::vector<std::vector<std::string>> queryList;
    do
    {
        std::vector<std::string> rowList;
        rowList.push_back(query.value(0).toString().toStdString());
        rowList.push_back(query.value(1).toString().toStdString());
        rowList.push_back(query.value(2).toString().toStdString());
        rowList.push_back(query.value(3).toString().toStdString());
        rowList.push_back(query.value(4).toString().toStdString());
        rowList.push_back(query.value(5).toString().toStdString());
        rowList.push_back(query.value(6).toString().toStdString());
        rowList.push_back(query.value(7).toString().toStdString());
        queryList.push_back(rowList);

    }while (query.next());

    for (std::vector<std::string> row : queryList)
    {
        museum.setName(row.at(1));
        museum.setDescription(row.at(2));
        museum.setIntro(row.at(4));
        museum.setPhoto(row.at(3));
        User user = getUserObject(std::stoi(row.at(0)));
        Edit<Museum> edit(museum, std::stoi(row.at(6)), user, std::stoi(row.at(5)), std::stoi(row.at(7)));
        output.push_back(edit);
    }
    return output;
}

std::vector<Edit<Collection>> ModelClass::getCollectionActions(int museumID){
    this->getMuseumObject(museumID);
    QString id(QString::fromStdString(std::to_string(museumID)));
    bool done = query.exec
    ("SELECT collectionID, name, description, photo, introduction, status, kind, editID, userID"
     " FROM edit WHERE museumID = "+id+" AND collectionID != -2 AND status = 0;");

    if (!done)
    {
        throw ModelException
        ("Failure retrieving data from db. Reason: "+query.lastError().databaseText().toStdString()
         +" Cause: "+query.lastError().driverText().toStdString());
    }
    std::vector<Edit<Collection>> output;
    query.next();
    if (!query.isValid())
    {
        return output;
    }
    std::vector<std::vector<std::string>> queryList;
    do
    {
        std::vector<std::string> rowList;
        rowList.push_back(query.value(0).toString().toStdString());
        rowList.push_back(query.value(1).toString().toStdString());
        rowList.push_back(query.value(2).toString().toStdString());
        rowList.push_back(query.value(3).toString().toStdString());
        rowList.push_back(query.value(4).toString().toStdString());
        rowList.push_back(query.value(5).toString().toStdString());
        rowList.push_back(query.value(6).toString().toStdString());
        rowList.push_back(query.value(7).toString().toStdString());
        rowList.push_back(query.value(8).toString().toStdString());
        queryList.push_back(rowList);
    }while (query.next());

    for (std::vector<std::string> row : queryList)
    {
        Collection collection = Collection("","","","", this->getMuseumObject(museumID));
        collection.setName(row.at(1));
        collection.setDescription(row.at(2));
        collection.setIntro(row.at(4));
        collection.setPhoto(row.at(3));
        collection.setID(std::stoi(row.at(0)));
        User user = getUserObject(std::stoi(row.at(8)));
        Edit<Collection> edit(collection, std::stoi(row.at(6)), user, std::stoi(row.at(5)), std::stoi(row.at(7)));
        output.push_back(edit);
    }
    return output;
}

std::vector<Edit<Artifact>> ModelClass::getArtifactActions(int museumID){
    Museum museum = this->getMuseumObject(museumID);
    QString id(QString::fromStdString(std::to_string(museumID)));
    bool done = query.exec
    ("SELECT artifactID, name, description, photo, introduction, status, kind, editID, userID, list"
     " FROM edit WHERE museumID = "+id+" AND artifactID != -2 AND status = 0;");
    if (!done)
    {
        throw ModelException
        ("Failure retrieving data from db. Reason: "+query.lastError().databaseText().toStdString()
         +" Cause: "+query.lastError().driverText().toStdString());
    }
    std::vector<Edit<Artifact>> output;
    query.next();
    if (!query.isValid())
    {
        return output;
    }
    std::vector<std::vector<std::string>> queryList;
    do
    {
        std::vector<std::string> rowList;
        rowList.push_back(query.value(0).toString().toStdString());
        rowList.push_back(query.value(1).toString().toStdString());
        rowList.push_back(query.value(2).toString().toStdString());
        rowList.push_back(query.value(3).toString().toStdString());
        rowList.push_back(query.value(4).toString().toStdString());
        rowList.push_back(query.value(5).toString().toStdString());
        rowList.push_back(query.value(6).toString().toStdString());
        rowList.push_back(query.value(7).toString().toStdString());
        rowList.push_back(query.value(8).toString().toStdString());
        rowList.push_back(query.value(9).toString().toStdString());
        queryList.push_back(rowList);
    }while (query.next());

    for (std::vector<std::string> row : queryList)
    {
        Artifact artifact("","","","", museum);
        artifact.setName(row.at(1));
        artifact.setDescription(row.at(2));
        artifact.setIntro(row.at(4));
        artifact.setPhoto(row.at(3));
        artifact.setID(std::stoi(row.at(0)));
        User user = getUserObject(std::stoi(row.at(8)));
        std::vector<Collection> list;
        std::stringstream ss(row.at(9));
        std::string token;
        while (std::getline(ss, token, ' '))
        {
            try
            {
                list.push_back(this->getCollectionObject(std::stoi(token)));
            } catch (ModelException e)
            {

            }
        }
        Edit<Artifact> edit(artifact, std::stoi(row.at(6)), user, list, std::stoi(row.at(5)), std::stoi(row.at(7)));
        output.push_back(edit);
    }
    return output;
}

Edit<Museum> ModelClass::getEditMuseumObject(int editID){
    QString id = QString::fromStdString(std::to_string(editID));
    bool done = query.exec
    ("SELECT museumID, name, description, photo, introduction, status, kind, editID, userID"
     " FROM edit WHERE editID = "+id+" AND museumID != -2 AND collectionID = -2 AND artifactID = -2;");
    if (!done)
    {
        throw ModelException
        ("Failure retrieving data from db. Reason: "+query.lastError().databaseText().toStdString()
         +" Cause: "+query.lastError().driverText().toStdString());
    }
    query.next();
    if (!query.isValid())
    {
        throw ModelException("Edit<Museum> object does not exist in database");
    }
    std::vector<std::string> rowList;
    rowList.push_back(query.value(0).toString().toStdString());
    rowList.push_back(query.value(1).toString().toStdString());
    rowList.push_back(query.value(2).toString().toStdString());
    rowList.push_back(query.value(3).toString().toStdString());
    rowList.push_back(query.value(4).toString().toStdString());
    rowList.push_back(query.value(5).toString().toStdString());
    rowList.push_back(query.value(6).toString().toStdString());
    rowList.push_back(query.value(7).toString().toStdString());
    rowList.push_back(query.value(8).toString().toStdString());
    Museum museum = this->getMuseumObject(std::stoi(rowList.at(0)));
    museum.setName(rowList.at(1));
    museum.setDescription(rowList.at(2));
    museum.setIntro(rowList.at(4));
    museum.setPhoto(rowList.at(3));
    User user = getUserObject(std::stoi(rowList.at(8)));
    Edit<Museum> edit(museum, std::stoi(rowList.at(6)), user, std::stoi(rowList.at(5)), std::stoi(rowList.at(7)));
    return edit;
}

Edit<Collection> ModelClass::getEditCollectionObject(int editID){
    QString id = QString::fromStdString(std::to_string(editID));
    bool done = query.exec
    ("SELECT collectionID, name, description, photo, introduction, status, kind, editID, userID, museumID"
     " FROM edit WHERE editID = "+id+" AND artifactID = -2 AND collectionID != -2;");
    if (!done)
    {
        throw ModelException
        ("Failure retrieving data from db. Reason: "+query.lastError().databaseText().toStdString()
         +" Cause: "+query.lastError().driverText().toStdString());
    }
    query.next();
    if (!query.isValid())
    {
        throw ModelException("Edit<Collection> object does not exist in database");
    }
    std::vector<std::string> rowList;
    rowList.push_back(query.value(0).toString().toStdString());
    rowList.push_back(query.value(1).toString().toStdString());
    rowList.push_back(query.value(2).toString().toStdString());
    rowList.push_back(query.value(3).toString().toStdString());
    rowList.push_back(query.value(4).toString().toStdString());
    rowList.push_back(query.value(5).toString().toStdString());
    rowList.push_back(query.value(6).toString().toStdString());
    rowList.push_back(query.value(7).toString().toStdString());
    rowList.push_back(query.value(8).toString().toStdString());
    rowList.push_back(query.value(9).toString().toStdString());
    Collection collection = Collection("","","","", this->getMuseumObject(std::stoi(rowList.at(9))));
    collection.setName(rowList.at(1));
    collection.setDescription(rowList.at(2));
    collection.setIntro(rowList.at(4));
    collection.setPhoto(rowList.at(3));
    collection.setID(std::stoi(rowList.at(0)));
    User user = getUserObject(std::stoi(rowList.at(8)));
    Edit<Collection> edit(collection, std::stoi(rowList.at(6)), user, std::stoi(rowList.at(5)), std::stoi(rowList.at(7)));
    return edit;
}

Edit<Artifact> ModelClass::getEditArtifactObject(int editID){
    QString id = QString::fromStdString(std::to_string(editID));
    bool done = query.exec
    ("SELECT artifactID, name, description, photo, introduction, status, kind, museumID, userID, list"
     " FROM edit WHERE editID = "+id+" AND collectionID = -2 AND artifactID != -2;");
    if (!done)
    {
        throw ModelException
        ("Failure retrieving data from db. Reason: "+query.lastError().databaseText().toStdString()
         +" Cause: "+query.lastError().driverText().toStdString());
    }
    query.next();
    if (!query.isValid())
    {
        throw ModelException("Edit<Artifact> object does not exist in database");
    }

    std::vector<std::string> rowList;
    rowList.push_back(query.value(0).toString().toStdString());
    rowList.push_back(query.value(1).toString().toStdString());
    rowList.push_back(query.value(2).toString().toStdString());
    rowList.push_back(query.value(3).toString().toStdString());
    rowList.push_back(query.value(4).toString().toStdString());
    rowList.push_back(query.value(5).toString().toStdString());
    rowList.push_back(query.value(6).toString().toStdString());
    rowList.push_back(query.value(7).toString().toStdString());
    rowList.push_back(query.value(8).toString().toStdString());
    rowList.push_back(query.value(9).toString().toStdString());

    Artifact artifact("","","","", this->getMuseumObject(std::stoi(rowList.at(7))));
    artifact.setName(rowList.at(1));
    artifact.setDescription(rowList.at(2));
    artifact.setIntro(rowList.at(4));
    artifact.setPhoto(rowList.at(3));
    artifact.setID(std::stoi(rowList.at(0)));
    User user = getUserObject(std::stoi(rowList.at(8)));
    std::vector<Collection> list;
    std::stringstream ss(rowList.at(9));
    std::string token;
    while (std::getline(ss, token, ' '))
    {
        try
        {
            list.push_back(this->getCollectionObject(std::stoi(token)));
        } catch (ModelException e)
        {

        }
    }
    Edit<Artifact> edit(artifact, std::stoi(rowList.at(6)), user, list, std::stoi(rowList.at(5)), editID);
    return edit;
}

Artifact ModelClass::getArtifact(int artifactID){
    QString id(QString::fromStdString(std::to_string(artifactID)));
    query.exec("SELECT artifactID, museumID, name, description, photo, introduction FROM artifacts WHERE artifactID = "+id+";");
    query.next();
    if (!query.isValid())
    {
        throw ModelException("No artifact entity stored in database");
    }
    std::string name = query.value(2).toString().toStdString();
    std::string description = query.value(3).toString().toStdString();
    std::string photo = query.value(4).toString().toStdString();
    std::string intro = query.value(5).toString().toStdString();
    Museum museum = this->getMuseumObject(query.value(1).toInt());
    query.finish();
    return Artifact(name, description, intro, photo, museum, artifactID);
}

void ModelClass::saveArtifactToDB(Artifact & artifact){
    if(artifact.indb())
    {
        throw ModelException("Artifact already exists in database");
    }
    else if (artifact.empty())
    {
        throw ModelException("Artifact object is empty");
    }
    else if (!artifact.getMuseum().indb())
    {
        throw ModelException("Museum object of artifact does not exist in database");
    }
    else if (!artifact.getMuseum().getUser().indb())
    {
        throw ModelException("User object of Museum object of artifact does not exist in database");
    }
    else
    {
        try
        {
            Museum museum = this->getMuseumObject(artifact.getMuseum().getName());
        } catch (ModelException e)
        {
            throw ModelException("Museum object of artifact does not exist in database");
        }

        try
        {
            User user = this->getUserObject(artifact.getMuseum().getUser().getName());
        } catch (ModelException e)
        {
            throw ModelException("User object of Museum object of artifact does not exist in database");
        }
    }

    int seed = std::chrono::steady_clock::now().time_since_epoch().count();
    std::default_random_engine eng(seed);
    int nextArtifactIndex = eng();
    QString name = QString::fromStdString(artifact.getName());
    QString museumID(QString::fromStdString(std::to_string(artifact.getMuseum().getMuseumID())));
    QString id(QString::fromStdString(std::to_string(nextArtifactIndex )));
    QString desc = QString::fromStdString(artifact.getDescription());
    QString photo = QString::fromStdString(artifact.getPhoto());
    QString intro = QString::fromStdString(artifact.getIntro());
    bool done = query.exec("INSERT INTO artifacts(museumID, artifactID, name, description, photo, introduction)"
                  " VALUES ("+museumID+", "+id+", '"+name+"', '"+desc+"', '"+photo+"', '"+intro+"')");

    if(!done)
    {
        std::string err = query.lastError().databaseText().toStdString()+". Cause: "+query.lastError().driverText().toStdString();
        throw ModelException("Saving artifact failed. Reason: "+err);
    }
    artifact.setID(nextArtifactIndex);
    query.finish();
}

void ModelClass::updateArtifactInDB(Artifact & artifact){
    artifactCheck(artifact);
    QString name = QString::fromStdString(artifact.getName());
    QString id = QString::fromStdString(std::to_string(artifact.getID()));
    QString description = QString::fromStdString(artifact.getDescription());
    QString photo = QString::fromStdString(artifact.getPhoto());
    QString intro = QString::fromStdString(artifact.getIntro());
    bool done = query.exec("UPDATE artifacts SET name = '"+name+"', description = '"+description+"', "
                           "photo = '"+photo+"', introduction = '"+intro+"'"
                           " WHERE artifactID = "+id+";");
    if(!done)
    {
        std::string err = query.lastError().databaseText().toStdString()+". Cause: "+query.lastError().driverText().toStdString();
        throw ModelException("artifact object failed to update in database. Reason: "+err);
    }
    query.finish();
}

void ModelClass::removeArtifactInDB(Artifact & artifact){
    artifactCheck(artifact);
    QString id(QString::fromStdString(std::to_string(artifact.getID())));
    query.exec("DELETE FROM artifacts WHERE artifactID = "+id+";");
    bool done = query.numRowsAffected() == 1;
    if (done)
    {
       artifact.setID(-1);
    }
    else
    {
        throw ModelException("artifact object does not exist in database");
    }
    query.finish();
}

std::vector<Artifact> ModelClass::getArtifactsByMuseum(int museumID){
    std::vector<Artifact> artifactList;
    Museum museum = this->getMuseumObject(museumID);
    QString id(QString::fromStdString(std::to_string(museumID)));
    query.exec("SELECT artifactID, museumID, name, description, photo, introduction FROM artifacts WHERE museumID = "+id+";");
    query.next();
    if (!query.isValid())
    {
        return artifactList;
    }
    do
    {
        std::string name = query.value(2).toString().toStdString();
        std::string description = query.value(3).toString().toStdString();
        int id = query.value(0).toInt();
        std::string photo = query.value(4).toString().toStdString();
        std::string intro = query.value(5).toString().toStdString();
        artifactList.push_back(Artifact(name, description, intro, photo, museum, id));
    }while(query.next());
    query.finish();
    return artifactList;
}

std::vector<Collection> ModelClass::getCollectionsByArtifact(int artifactID){
    std::vector<Collection> collectionList;
    QString id(QString::fromStdString(std::to_string(artifactID)));
    this->getArtifact(artifactID); //Will throw exception if artifactID does not exist in database
    query.exec("SELECT collectionID FROM artifactCollection WHERE artifactID = "+id+";");
    query.next();
    if (!query.isValid())
    {
        return collectionList;
    }
    std::vector<int> list;
    do
    {
        list.push_back(query.value(0).toInt());
    }while(query.next());
    query.finish();

    for (int index : list)
    {
        collectionList.push_back(ModelClass::getCollectionObject(index));
    }
    return collectionList;
}

std::vector<Artifact> ModelClass::getArtifactsByCollection(int collectionID){
    std::vector<Artifact> artifactList;
    QString id(QString::fromStdString(std::to_string(collectionID)));
    this->getCollectionObject(collectionID); //Will throw exception if collectionID does not exist in database
    query.exec("SELECT artifactID FROM artifactCollection WHERE collectionID = "+id+";");
    query.next();
    if (!query.isValid())
    {
        return artifactList;
    }
    std::vector<int> list;
    do
    {
        list.push_back(query.value(0).toInt());
    }while(query.next());
    query.finish();

    for (int index : list)
    {
        artifactList.push_back(ModelClass::getArtifact(index));
    }
    return artifactList;
}

void ModelClass::addArtifactCollection(const Collection & collection, const Artifact & artifact){
    ModelClass::artifactCheck(artifact);
    ModelClass::collectionCheck(collection);
    QString collectionID = QString::fromStdString(std::to_string(collection.getID()));
    QString artifactID = QString::fromStdString(std::to_string(artifact.getID()));
    query.prepare("INSERT INTO artifactCollection(artifactID, collectionID)"
                  " VALUES ("+artifactID+", "+collectionID+")");
    if(!query.exec())
    {
        std::string err = query.lastError().databaseText().toStdString()+". Cause: "+query.lastError().driverText().toStdString();
        throw ModelException("Saving artifact-collection affliation failed. Reason: "+err);
    }
    query.finish();
}

void ModelClass::addArtifactCollection(const Artifact & artifact, const Collection & collection){
    ModelClass::artifactCheck(artifact);
    ModelClass::collectionCheck(collection);

    QString collectionID = QString::fromStdString(std::to_string(collection.getID()));
    QString artifactID = QString::fromStdString(std::to_string(artifact.getID()));
    query.prepare("INSERT INTO artifactCollection(artifactID, collectionID)"
                  " VALUES ("+artifactID+", "+collectionID+")");
    if(!query.exec())
    {
        std::string err = query.lastError().databaseText().toStdString()+". Cause: "+query.lastError().driverText().toStdString();
        throw ModelException("Saving artifact-collection affliation failed. Reason: "+err);
    }
    query.finish();
}

void ModelClass::removeArtifactCollection(const Collection & collection, const Artifact & artifact){
    ModelClass::artifactCheck(artifact);
    ModelClass::collectionCheck(collection);

    QString collectionID = QString::fromStdString(std::to_string(collection.getID()));
    QString artifactID = QString::fromStdString(std::to_string(artifact.getID()));

    bool done = query.exec("DELETE FROM artifactCollection WHERE "
                  "artifactID = "+artifactID+" AND collectionID = "+collectionID+";");
    if(!done)
    {
        std::string err = query.lastError().databaseText().toStdString()+". Cause: "+query.lastError().driverText().toStdString();
        throw ModelException("Deleting artifact-collection affliation failed. Reason: "+err);
    }
    done = query.numRowsAffected() != 0;
    if (!done)
    {
        std::string err = query.lastError().databaseText().toStdString()+". Cause: "+query.lastError().driverText().toStdString();
        throw ModelException("Deleting artifact-collection affliation failed. Reason: "+err);
    }
}

void ModelClass::removeArtifactCollection(const Artifact & artifact, const Collection & collection){
    ModelClass::artifactCheck(artifact);
    ModelClass::collectionCheck(collection);

    QString collectionID = QString::fromStdString(std::to_string(collection.getID()));
    QString artifactID = QString::fromStdString(std::to_string(artifact.getID()));

    bool done = query.exec("DELETE FROM artifactCollection WHERE "
                  "artifactID = "+artifactID+" AND collectionID = "+collectionID+";");
    if(!done)
    {
        std::string err = query.lastError().databaseText().toStdString()+". Cause: "+query.lastError().driverText().toStdString();
        throw ModelException("Deleting artifact-collection affliation failed. Reason: "+err);
    }
    done = query.numRowsAffected() != 0;
    if (!done)
    {
        std::string err = query.lastError().databaseText().toStdString()+". Cause: "+query.lastError().driverText().toStdString();
        throw ModelException("Deleting artifact-collection affliation failed. Reason: "+err);
    }
}

void ModelClass::removeArtifactCollection(const Artifact & artifact){
    ModelClass::artifactCheck(artifact);
    QString artifactID = QString::fromStdString(std::to_string(artifact.getID()));
    bool done = query.exec("DELETE FROM artifactCollection WHERE "
                  "artifactID = "+artifactID+";");
    if(!done)
    {
        std::string err = query.lastError().databaseText().toStdString()+". Cause: "+query.lastError().driverText().toStdString();
        throw ModelException("Deleting artifact-collection affliation failed. Reason: "+err);
    }
    done = query.numRowsAffected() != 0;
    if (!done)
    {
        std::string err = query.lastError().databaseText().toStdString()+". Cause: "+query.lastError().driverText().toStdString();
        throw ModelException("Deleting artifact-collection affliation failed. Reason: "+err);
    }
}

void ModelClass::removeArtifactCollection(const Collection & collection){
    ModelClass::collectionCheck(collection);
    QString collectionID = QString::fromStdString(std::to_string(collection.getID()));
    bool done = query.exec("DELETE FROM artifactCollection WHERE "
                  "collectionID = "+collectionID+";");
    if(!done)
    {
        std::string err = query.lastError().databaseText().toStdString()+". Cause: "+query.lastError().driverText().toStdString();
        throw ModelException("Deleting artifact-collection affliation failed. Reason: "+err);
    }
    done = query.numRowsAffected() != 0;
    if (!done)
    {
        std::string err = query.lastError().databaseText().toStdString()+". Cause: "+query.lastError().driverText().toStdString();
        throw ModelException("Deleting artifact-collection affliation failed. Reason: "+err);
    }
}

Collection ModelClass::getCollectionObject(int collectionID){
    QString id(QString::fromStdString(std::to_string(collectionID)));
    query.exec("SELECT collectionID, museumID, name, description, introduction, photo FROM collections WHERE collectionID = "+id+";");
    query.next();
    if (!query.isValid())
    {
        throw ModelException("No collection entity stored in database");
    }
    std::string name = query.value(2).toString().toStdString();
    std::string description = query.value(3).toString().toStdString();
    std::string introduction = query.value(4).toString().toStdString();
    std::string photo = query.value(5).toString().toStdString();
    Museum museum = this->getMuseumObject(query.value(1).toInt());
    query.finish();
    return Collection(name, description, introduction, photo, museum, collectionID);
}

std::vector<Collection> ModelClass::getCollectionListByMuseumID(int museumID){
    std::vector<Collection> collectionList;
    Museum museum = this->getMuseumObject(museumID);
    QString id(QString::fromStdString(std::to_string(museumID)));
    query.exec("SELECT collectionID, museumID, name, description, introduction, photo FROM collections WHERE museumID = "+id+";");
    query.next();
    if (!query.isValid())
    {
        return collectionList;
    }
    do
    {
        std::string name = query.value(2).toString().toStdString();
        std::string description = query.value(3).toString().toStdString();
        int id = query.value(0).toInt();
        std::string introduction = query.value(4).toString().toStdString();
        std::string photo = query.value(5).toString().toStdString();
        collectionList.push_back(Collection(name, description, introduction, photo, museum, id));
    }while(query.next());
    query.finish();
    return collectionList;
}

void ModelClass::saveCollectionToDB(Collection & collection){
    if(collection.indb())
    {
        throw ModelException("Collection already exists in database");
    }
    else if(collection.empty())
    {
        throw ModelException("Museum object empty");
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
    QString intro = QString::fromStdString(collection.getIntro());
    QString photo = QString::fromStdString(collection.getPhoto());
    bool done = query.exec("INSERT INTO collections(museumID, collectionID, name, description, introduction, photo)"
                  " VALUES ("+museumID+", "+id+", '"+name+"', '"+desc+"', '"+intro+"', '"+photo+"')");

    if(!done)
    {
        std::string err = query.lastError().databaseText().toStdString()+". Cause: "+query.lastError().driverText().toStdString();
        throw ModelException("Saving collection failed. Reason: "+err);
    }
    collection.setID(nextCollectionIndex);
    query.finish();
}

void ModelClass::updateCollectionInDB(Collection &collection){
    collectionCheck(collection);
    QString name = QString::fromStdString(collection.getName());
    QString id = QString::fromStdString(std::to_string(collection.getID()));
    QString description = QString::fromStdString(collection.getDescription());
    QString intro = QString::fromStdString(collection.getIntro());
    QString photo = QString::fromStdString(collection.getPhoto());
    bool done = query.exec("UPDATE collections SET name = '"+name+"', description = '"+description+"', "
                           "introduction = '"+intro+"', photo = '"+photo+"'"
                           " WHERE collectionID = "+id+";");
    if(!done)
    {
        std::string err = query.lastError().databaseText().toStdString()+". Cause: "+query.lastError().driverText().toStdString();
        throw ModelException("Collection object failed to update in database. Reason: "+err);
    }
    query.finish();
}

void ModelClass::removeCollectionInDB(Collection &collection){
    collectionCheck(collection);
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

/**
 * @brief ModelClass::getMuseumList
 * @return
 */
std::vector<Museum> ModelClass::getMuseumList(){
    query.exec("SELECT museumID, userID, name, description, introduction, photo FROM museum ORDER BY name ASC;");
    std::vector<Museum> museumList;
    this->query.next();
    if (!this->query.isValid())
    {
        query.finish();
        return museumList;
    }
    do{
        std::string name = query.value(2).toString().toStdString();
        std::string introduction = query.value(4).toString().toStdString();
        std::string photo = query.value(5).toString().toStdString();
        std::string description = query.value(3).toString().toStdString();
        int id = query.value(0).toString().toInt();
        int userID = query.value(1).toString().toInt();
        User user("","","");
        user.setUserID(userID);
        museumList.push_back(Museum(name, description, introduction, photo, user, id));
    }while(query.next());
    query.finish();

    for (Museum & museum : museumList)
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

Museum ModelClass::getMuseumObject(int museumID){
    QString id(QString::fromStdString(std::to_string(museumID)));
    query.exec("SELECT name, description, userID, museumID, introduction, photo FROM museum WHERE museumID = "+id+";");
    query.next();
    if (!query.isValid())
    {
        throw ModelException("MuseumID does not exist in database");
    }
    std::string name = query.value(0).toString().toStdString();
    std::string introduction = query.value(4).toString().toStdString();
    std::string photo = query.value(5).toString().toStdString();
    std::string description = query.value(1).toString().toStdString();
    int userID = query.value(2).toString().toInt();
    query.finish();
    User curator = this->getUserObject(userID);
    return Museum(name, description, introduction, photo, curator, museumID);
}

Museum ModelClass::getMuseumObject(std::string museumName){
    QString name = QString::fromStdString(museumName);
    query.exec("SELECT museumID, userID, name, description, introduction, photo FROM museum where name GLOB '"+name+"';");
    query.next();
    if (!query.isValid())
    {
        throw ModelException("Museum name does not exist in database");
    }
    int museumID = query.value(0).toInt();
    int userID = query.value(1).toInt();
    std::string _museumName = query.value(2).toString().toStdString();
    std::string description = query.value(3).toString().toStdString();
    std::string introduction = query.value(4).toString().toStdString();
    std::string photo = query.value(5).toString().toStdString();
    query.finish();
    User user(this->getUserObject(userID));
    return Museum(_museumName, description, introduction, photo, user, museumID);
}

void ModelClass::saveMuseumToDB(Museum & museum){
    if (museum.indb())
    {
        throw ModelException("Museum object already in database");
    }
    else if(museum.empty())
    {
        throw ModelException("Museum object empty");
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

    QString intro = QString::fromStdString(museum.getIntro());
    QString photo = QString::fromStdString(museum.getPhoto());
    bool done = query.exec("INSERT INTO museum(museumID, userID, name, description, introduction, photo)"
                  " VALUES ("+museumID+", "+userID+", '"+name+"', '"+desc+"', '"+intro+"', '"+photo+"')");


    if(!done)
    {
        std::string err = query.lastError().databaseText().toStdString()+". Cause: "+query.lastError().driverText().toStdString();
        throw ModelException("Saving museum failed. Reason: "+err);
    }
    museum.setMuseumID(nextMuseumIndex);
    query.finish();
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
    else if(museum.empty())
    {
        throw ModelException("Museum object empty");
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
    QString intro = QString::fromStdString(museum.getIntro());
    QString photo = QString::fromStdString(museum.getPhoto());
    bool done = query.exec("UPDATE museum SET name = '"+name+"', description = '"+description+"', "
                           "introduction = '"+intro+"', photo = '"+photo+"' WHERE museumID = "+museumID+";");
    if(!done)
    {
        std::string err = query.lastError().databaseText().toStdString()+". Cause: "+query.lastError().driverText().toStdString();
        throw ModelException("Museum object failed to update in database. Reason: "+err);
    }
    query.finish();
}

std::vector<Museum> ModelClass::getMuseumByCurator(int userID){
    QString id = QString::fromStdString(std::to_string(userID));
    User user = this->getUserObject(userID);
    query.exec("SELECT museumID, userID, name, description, introduction, photo FROM museum WHERE userID = '"+id+"';");
    query.next();

    std::vector<Museum> output;
    if (!query.isValid())
    {
        return output;
    }

    do
    {
        output.push_back(Museum(query.value(2).toString().toStdString(),
                                query.value(3).toString().toStdString(),
                                query.value(4).toString().toStdString(),
                                query.value(5).toString().toStdString(),
                                user,
                                query.value(0).toInt()));
    }
    while(query.next());
    return output;
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
        throw ModelException("User ID does not exist in database");
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
        throw ModelException("User object already in database");
    }
    else if (user.empty())
    {
        throw ModelException("User object empty");
    }

    int intHeadCurator = 0;
    query.exec("SELECT * FROM public;");
    query.next();
    if (!query.isValid())
    {
        intHeadCurator = 1;
    }
    query.finish();
    QString headCurator = QString::fromStdString(std::to_string(intHeadCurator));
    int seed = std::chrono::steady_clock::now().time_since_epoch().count();
    std::default_random_engine eng(seed);
    int nextUserIndex = eng();
    QString name = QString::fromStdString(user.getName());
    QString userID(QString::fromStdString(std::to_string(nextUserIndex)));
    QString email = QString::fromStdString(user.getEmail());
    QString password = QString::fromStdString(user.getPassword());
    QString queryText("INSERT INTO public(userID,username,email,password,curator) VALUES ("+userID+",'"+name+"','"+email+"','"+password+"',"+headCurator+");");
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

bool ModelClass::checkHeadCurator(User &user){
    if (!user.indb())
    {
        throw ModelException("User object does not exist in database");
    }
    QString id = QString::fromStdString(std::to_string(user.getUserID()));
    query.exec("SELECT curator FROM public WHERE userID = "+id+";");
    query.next();
    if (!query.isValid())
    {
        throw ModelException("User object does not exist in database");
    }
    return query.value(0).toInt() == 1;
}
