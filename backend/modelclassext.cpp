#include "modelclassext.h"

void ModelClassExt::initdb(std::string codeBaseDirectory){
    db.setDatabaseName(QString::fromStdString(codeBaseDirectory + "database/db.db"));
    if(!db.open()){
        qDebug("Model class build failed for linux and macos builds.");
        throw ModelException("Database failed to open.");
    }else{
        qDebug("Model class build successful.");
    }
}

void ModelClassExt::saveUserToDB(User &u) {
    if (! ModelClass::saveUserToDB(u)) {
        throw ModelException("Unable to save user to the database.");
    }
}

void ModelClassExt::saveMuseumToDB(Museum &m) {
    if (! ModelClass::saveMuseumToDB(m))  {
        throw  ModelException("Unable to save musem to the database.");
    }
}

//you already have string for following methods. just return json and rename.
json ModelClassExt::getUserInfoJSON(std::string username){
    json userJSON = {
        {"username", username},
        {"email", "am@gm1.com"},
    };
    return userJSON;
}


json ModelClassExt::getMuseumInfoJSON(int museumID) {
    json museumJSON = {
        {"id", museumID},
        {"name", "testMuseum"},
        {"description", "testMuseum Description oh yeah"},
        {"introduction", "introduction lekso is cool"},
        {"userID", 2},
        {"image", "https://i.picsum.photos/id/665/400/200.jpg"}
    };
    return museumJSON;
};

json ModelClassExt::getCollectionInfoJSON(int collectionID){
    json collectionObj = {
        {"museum",{
             {"id", 1},
             {"name", "testMuseum"}
         }},
        {"collection",{
             {"id", collectionID},
             {"description", "description"},
             {"name", "Collection title"},
             {"introduction", "introduction"}
         }}
    };
    return collectionObj;
};

/**
 * @brief ModelClassExt::saveCollectionToDB Save c to the DB. Throw ModelException  if unable to do so.
 * @param c The collection to be saved.
 */
void ModelClassExt::saveCollectionToDB(Collection &c) {

}


/**
 * @brief ModelClassExt::getCollectionListByMuseumID
 * @param museumID
 * @return All collections associated with museum containing museumID
 * [See trello  for schema]
 */
json ModelClassExt::getCollectionListByMuseumID(int museumID){
    json colArray = json::array
            ({
                 {
                     {"id",1},
                     {"description", "description for collection 1"},
                     {"name", "collection N1"},
                     {"introduction", "introduction for collection 1"}
                 },
                 {
                     {"id",2},
                     {"description", "description for collection 2"},
                     {"name", "collection N2"},
                     {"introduction", "introduction for collection 2"}
                 }
             });
    return colArray;
}

