#ifndef MODELCLASSEXT_H
#define MODELCLASSEXT_H
#include <../model/modelclass.h>
class ModelClassExt : public ModelClass
{
public:
    static void saveCollectionToDB();
    static std::string getCollectionInfoJSON(int collectionID);

};
#endif // MODELCLASSEXT_H
