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

void ModelClassExt::saveCollectionToDB(Collection *c) {

}


json ModelClassExt::getMuseumInfoJson(int museumID) {
    json museumJSON = {
        {"id", museumID},
        {"name", "testMuseum"},
        {"description", "testMuseum Description"},
        {"userID", 2}

    };
    return museumJSON;
};

json ModelClassExt::getCollectionInfoJSON(int collectionID){
    json collectionObj = {
        {"museum",{
             {"id", collectionID},
             {"name", "testMuseum"}
         }},
        {"collection",{
             {"id",1},
             {"description", "description"},
             {"name", "Collection title"},
             {"introduction", "introduction"}
         }}
    };
    return collectionObj;
};

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

json ModelClassExt::getUserInfoJson(std::string username){
    json userJSON = {
        {"username", username},
        {"email", "am@gm1.com"},
    };
    return userJSON;
}
