#ifndef MODELCLASSEXT_H
#define MODELCLASSEXT_H
#include <../model/modelclass.h>
#include <../model/ModelException.h>
#include "../nlohmann/json.hpp"
using json=nlohmann::json;


class ModelClassExt : public ModelClass
{
public:
    static void initdb(std::string codeBaseDirectory);
    static void saveCollectionToDB();
    static json getCollectionInfoJSON(int collectionID);
    static json getMuseumAndCollectionInfoJSON(int museumID);
};
#endif // MODELCLASSEXT_H
