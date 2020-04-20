#ifndef MODELCLASSEXT_H
#define MODELCLASSEXT_H
#include <../model/modelclass.h>
class ModelClassExt:ModelClass
{
    void saveCollectionToDB();
    std::string getCollectionInfoJSON(int colID);

};
#endif // MODELCLASSEXT_H
