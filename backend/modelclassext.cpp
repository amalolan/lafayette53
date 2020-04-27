#include "modelclassext.h"


ModelClassExt* ModelClassExt::getInstance(){
    return new ModelClassExt(std::string(CODE_BASE_DIRECTORY)+ "database/testdb.db");
}
