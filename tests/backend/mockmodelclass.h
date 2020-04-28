#ifndef MOCKMODELCLASS_H
#define MOCKMODELCLASS_H
#include "gmock/gmock.h"
#include "../../backend/modelclassext.h"
#include "../../model/modelclass.h"
#include "../../model/ModelException.h"
#include "../../model/collection.h"
#include "../../model/user.h"
#include "../../model/museum.h"
#include "../../model/artifact.h"

#include <vector>
using namespace std;
class MockModelClass: public  ModelClassExt {
public:
    MockModelClass() : ModelClassExt("") {}
    // User
    MOCK_METHOD1(getUserObject, User(string));
    MOCK_METHOD1(saveUserToDB, void(User&));

    // Museum
    MOCK_METHOD0(getMuseumList, vector<Museum>());
    MOCK_METHOD1(getMuseumObject, Museum(int));
    MOCK_METHOD1(saveMuseumToDB, void(Museum&));

    // Collection
    MOCK_METHOD1(getCollectionListByMuseumID, vector<Collection>(int));
    MOCK_METHOD1(getCollectionsByArtifact, vector<Collection>(int));
    MOCK_METHOD1(getCollectionObject, Collection(int));
    MOCK_METHOD1(saveCollectionToDB, void(Collection&));

    // Artifact
    MOCK_METHOD1(getArtifactsByMuseum, vector<Artifact>(int));
    MOCK_METHOD1(getArtifactsByCollection, vector<Artifact>(int));
    MOCK_METHOD2(addArtifactCollection, void(Artifact, Collection));
    MOCK_METHOD1(saveArtifactToDB, void(Artifact&));
    MOCK_METHOD1(getArtifact, Artifact(int));
};




#endif // MOCKMODELCLASS_H
