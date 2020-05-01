#include "modelclass.h"

int main(int argc, char **argv) {
    std::cout << "Start." <<std::endl;
    ModelClass::initdb("../../lafayette53/");
    ModelClass * ptr = ModelClass::getInstance(ModelClass::test);
    ptr->createTables();
    User user("sena", "s@lafayette.edu", "password");
    ptr->saveUserToDB(user);
    Museum museum("museum", "desc", "intro", user);
    ptr->saveMuseumToDB(museum);
    Collection collection("artifact", "desc", "intro", "photo", museum);
    ptr->saveCollectionToDB(collection);
    Artifact artifact("artifact", "desc", "intro", "photo", museum);
    ptr->saveArtifactToDB(artifact);
    ptr->addArtifactCollection(artifact, collection);
    artifact.setPhoto("newPhoto");
    artifact.setName("newName");
    artifact.setDescription("newDesc");
    artifact.setIntro("newIntro");
    std::cout << "Works 1." <<std::endl;
    std::cout << "Works 2." <<std::endl;
    Edit<Artifact> edit(artifact, Edit<Artifact>::edit, user, ptr->getCollectionsByArtifact(artifact.getID()));
    ptr->saveEditToDB(edit);
    std::cout << edit.getObject().toString() << std::endl;
    std::cout << "Works 3." <<std::endl;
    ptr->getArtifactActions(user.getUserID());
    std::cout << "Works 4." <<std::endl;
    std::cout << ptr->getArtifactActions(user.getUserID()).size() << " : " << artifact.toString() << std::endl;
    std::cout << "Works 5." <<std::endl;
    ptr->removeUserFromDB(user);
    delete ptr;
    return 0;
}

