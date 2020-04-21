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
    static void saveCollectionToDB(Collection *);
    static json getCollectionInfoJSON(int);
    static json getMuseumAndCollectionInfoJSON(int);
};
#endif // MODELCLASSEXT_H
