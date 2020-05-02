#ifndef MODELCLASSEXT_H
#define MODELCLASSEXT_H
#include "../model/modelclass.h"
#include "../model/ModelException.h"
#include "../model/collection.h"
#include "../nlohmann/json.hpp"
#include "../model/artifact.h"
#include <vector>
using json=nlohmann::json;


class ModelClassExt : public ModelClass
{
public:
    ModelClassExt(std::string str) :  ModelClass(str) {
    }
    virtual ~ModelClassExt()
    {
    }
    static ModelClassExt* singleInstance;
    static ModelClassExt* getInstance(std::string);
};


#endif // MODELCLASSEXT_H
