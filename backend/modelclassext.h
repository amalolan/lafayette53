#ifndef MODELCLASSEXT_H
#define MODELCLASSEXT_H
#include <../model/modelclass.h>
#include <../model/ModelException.h>
#include <../model/collection.h>
#include "../nlohmann/json.hpp"
using json=nlohmann::json;


class ModelClassExt : public ModelClass
{
public:
    static void initdb(std::string codeBaseDirectory);
    static void saveCollectionToDB(Collection &);
    static void saveMuseumToDB(Museum &);
    static void saveUserToDB(User &);
    //you already have string for following methods. just return json and rename.
    static json getMuseumInfoJSON(int museumID);
    static json getUserInfoJSON(std::string username);

    static json getCollectionInfoJSON(int collectionID);
    static json getCollectionListByMuseumID(int museumID);
};
#endif // MODELCLASSEXT_H
