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

    Artifact getArtifact(int artifactID)
    {
        return Artifact("Artifact N1", "The best Artifact", "nothing special", artifactID);
    }
    //TODO
    std::vector<Collection> getCollectionsByArtifact(int artifactID){
        return std::vector<Collection>();
    }
};
#endif // MODELCLASSEXT_H
