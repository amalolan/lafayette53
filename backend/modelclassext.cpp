#include "modelclassext.h"


ModelClassExt* ModelClassExt::getInstance(std::string baseDir){
    return new ModelClassExt(baseDir + "database/testdb.db");
}
