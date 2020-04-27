#ifndef MODELCLASSEXT_H
#define MODELCLASSEXT_H
#include "../model/modelclass.h"
#include "../model/ModelException.h"
#include "../model/collection.h"
#include "../nlohmann/json.hpp"
#include "../model/artifact.h"
//#include "handler.h"
#ifndef CODE_BASE_DIRECTORY
    #ifdef __APPLE__
        #define CODE_BASE_DIRECTORY "../../../../../lafayette53/"
    #elif __linux
        #define CODE_BASE_DIRECTORY "../../lafayette53/"
    #endif
#endif// CODE_BASE_DIRECTORY
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
    static ModelClassExt* getInstance();
};
#endif // MODELCLASSEXT_H
