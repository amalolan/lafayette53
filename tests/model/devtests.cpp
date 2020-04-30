#include "devtests.h"

TEST_F(DevTests, testingModelClassOpenClose){
    ASSERT_TRUE(this->model->open());
    ASSERT_TRUE(this->model->status());
    ASSERT_TRUE(this->model->close());
    ASSERT_FALSE(this->model->status());
}

TEST_F(DevTests, testUserInput){
    this->model->open();
    User user("sena", "s@lafayette.edu", "password");
    User empty("", "", "");
    EXPECT_NO_THROW(this->model->saveUserToDB(user));
    EXPECT_THROW(this->model->saveUserToDB(empty), ModelException);
    ASSERT_TRUE(user.indb());
    ASSERT_FALSE(empty.indb());
    EXPECT_NO_THROW(this->model->updateUserInDB(user));
    EXPECT_THROW(this->model->updateUserInDB(empty), ModelException);
    EXPECT_THROW(this->model->saveUserToDB(user), ModelException);
    EXPECT_NO_THROW(this->model->removeUserFromDB(user));
    EXPECT_THROW(this->model->removeUserFromDB(empty), ModelException);
    EXPECT_THROW(this->model->updateUserInDB(user), ModelException);
}

TEST_F(DevTests, testingMuseumInput){
    User newUser("sena", "s@lafayette.edu", "password");
    User empty("","","");
    EXPECT_NO_THROW(this->model->saveUserToDB(newUser));
    EXPECT_TRUE(newUser.indb());
    Museum museum("aMuseum", "a sample museum", empty);
    EXPECT_THROW(this->model->saveMuseumToDB(museum), ModelException);
    museum.setUser(newUser);
    EXPECT_NO_THROW(this->model->saveMuseumToDB(museum));
    EXPECT_THROW(this->model->saveMuseumToDB(museum), ModelException);
    EXPECT_TRUE(museum.indb());
    museum.setIntro("intro");
    EXPECT_NO_THROW(this->model->updateMuseumInDB(museum));
    EXPECT_NO_THROW(this->model->removeMuseumFromDB(museum));
    EXPECT_TRUE(!museum.indb());
    EXPECT_NO_THROW(this->model->removeUserFromDB(newUser));
    EXPECT_TRUE(!newUser.indb());

    this->model->close();
    this->model->open();
    EXPECT_NO_THROW(this->model->saveUserToDB(newUser));
    Museum museum1("aMuseum", "aMuseum", newUser);
    Museum museum2("aSecondMuseum", "description", "intro", newUser);
    Museum museum3("aMuseum", "aMuseum", newUser);
    Museum museum4("", "aMuseum", newUser);
    EXPECT_NO_THROW(this->model->saveMuseumToDB(museum1));
    EXPECT_NO_THROW(this->model->saveMuseumToDB(museum2));
    EXPECT_THROW(this->model->saveMuseumToDB(museum3), ModelException);
    EXPECT_THROW(this->model->saveMuseumToDB(museum4), ModelException);
    EXPECT_THROW(this->model->updateMuseumInDB(museum3), ModelException);
    EXPECT_THROW(this->model->updateMuseumInDB(museum4), ModelException);
    EXPECT_THROW(this->model->removeMuseumFromDB(museum3), ModelException);
    EXPECT_THROW(this->model->removeMuseumFromDB(museum4), ModelException);
    EXPECT_NO_THROW(this->model->getMuseumList());
    EXPECT_NO_THROW(this->model->removeMuseumFromDB(museum1));
    EXPECT_NO_THROW(this->model->removeUserFromDB(newUser));
    EXPECT_THROW(this->model->removeMuseumFromDB(museum2), ModelException);
    EXPECT_NO_THROW(this->model->getMuseumList());
}

TEST_F(DevTests, TestingModelClassMuseumAndUserReturns){
    User newUser("sena", "s@lafayette.edu", "password");
    this->model->open();
    EXPECT_NO_THROW(this->model->saveUserToDB(newUser));
    Museum museum("aMuseum", "a sample museum", "intro", "photo", newUser);
    EXPECT_NO_THROW(this->model->saveMuseumToDB(museum));
    EXPECT_EQ(museum, this->model->getMuseumObject("aMuseum"));
    EXPECT_EQ(museum, this->model->getMuseumObject(museum.getMuseumID()));

    museum.setPhoto("newPhoto");
    museum.setDescription("newDesc");
    museum.setIntro("newIntro");
    museum.setName("newMuseum");
    newUser.setEmail("newEmail");
    newUser.setPassword("newEmail");
    ASSERT_NO_THROW(this->model->updateUserInDB(newUser));
    EXPECT_EQ(newUser, this->model->getUserObject(newUser.getUserID()));
    museum.setUser(newUser);
    ASSERT_NO_THROW(this->model->updateMuseumInDB(museum));
    EXPECT_EQ(museum, this->model->getMuseumObject("newMuseum"));
    EXPECT_EQ(museum, this->model->getMuseumObject(museum.getMuseumID()));
    EXPECT_EQ(museum, this->model->getMuseumList().front());
    EXPECT_EQ(1, this->model->getMuseumList().size());
    EXPECT_NO_THROW(this->model->removeMuseumFromDB(museum));
    std::vector<Museum> output = {};
    EXPECT_EQ(output, this->model->getMuseumList());
    ASSERT_TRUE(!museum.indb());
    EXPECT_NO_THROW(this->model->removeUserFromDB(newUser));
    ASSERT_TRUE(!newUser.indb());
}

TEST_F(DevTests, TestingUserMuseumExceptions){
    User newUser("sena", "s@lafayette.edu", "password");
    ASSERT_THROW(this->model->updateUserInDB(newUser), ModelException);
    ASSERT_THROW(this->model->removeUserFromDB(newUser), ModelException);
    ASSERT_THROW(this->model->getMuseumObject(""), ModelException);
    ASSERT_THROW(this->model->getUserObject(""), ModelException);
    ASSERT_THROW(this->model->getUserObject(-1), ModelException);

    Museum museum("aMuseum", "a sample museum", newUser);
    ASSERT_THROW(this->model->saveMuseumToDB(museum), ModelException);
    ASSERT_THROW(this->model->updateMuseumInDB(museum), ModelException);
    ASSERT_THROW(this->model->removeMuseumFromDB(museum), ModelException);

    ASSERT_NO_THROW(this->model->saveUserToDB(newUser));
    ASSERT_THROW(this->model->saveMuseumToDB(museum), ModelException);
    museum.getUser().setUserID(0);
    ASSERT_THROW(this->model->saveMuseumToDB(museum), ModelException);
    museum.setUser(newUser); //Remember that museum objects have no connection with user objects.
    //User object has to be reset to connect museum with User
    ASSERT_NO_THROW(this->model->saveMuseumToDB(museum));
    ASSERT_NO_THROW(this->model->updateUserInDB(newUser));
    ASSERT_NO_THROW(this->model->removeMuseumFromDB(museum));
    ASSERT_NO_THROW(this->model->removeUserFromDB(newUser));
    Museum museum2("", "", newUser);
    ASSERT_THROW(this->model->saveMuseumToDB(museum), ModelException);
    User user2("","","");
    ASSERT_THROW(this->model->saveUserToDB(user2), ModelException);
    this->model->close();
}

TEST_F(DevTests, TestingArtifactInput){
    User user("sena", "s@lafayette.edu", "password");
    ASSERT_NO_THROW(this->model->saveUserToDB(user));
    Museum museum("museum", "desc", "intro", user);
    ASSERT_NO_THROW(this->model->saveMuseumToDB(museum));
    Artifact artifact("artifact", "desc", "intro", "photo", museum);
    ASSERT_NO_THROW(this->model->saveArtifactToDB(artifact));
    EXPECT_TRUE(artifact.indb());
    artifact.setPhoto("newPhoto");
    artifact.setName("newName");
    artifact.setDescription("newDesc");
    artifact.setIntro("newIntro");
    ASSERT_NO_THROW(this->model->updateArtifactInDB(artifact));
    std::vector<Artifact> output = {artifact};
    EXPECT_NO_THROW(this->model->getArtifactsByMuseum(museum.getMuseumID()));
    EXPECT_EQ(output, this->model->getArtifactsByMuseum(museum.getMuseumID()));
    this->model->removeUserFromDB(user);
    EXPECT_THROW(this->model->getArtifactsByMuseum(museum.getMuseumID()), ModelException);
}

TEST_F(DevTests, TestingCollectionInput){
    User user("sena", "s@lafayette.edu", "password");
    ASSERT_NO_THROW(this->model->saveUserToDB(user));
    Museum museum("museum", "desc", "intro", user);
    ASSERT_NO_THROW(this->model->saveMuseumToDB(museum));
    Collection collection("collection", "desc", "intro", "photo", museum);
    ASSERT_NO_THROW(this->model->saveCollectionToDB(collection));
    EXPECT_TRUE(collection.indb());
    collection.setPhoto("newPhoto");
    collection.setName("newName");
    collection.setDescription("newDesc");
    collection.setIntro("newIntro");
    ASSERT_NO_THROW(this->model->updateCollectionInDB(collection));
    std::vector<Collection> output = {collection};
    EXPECT_NO_THROW(this->model->getCollectionListByMuseumID(museum.getMuseumID()));
    EXPECT_EQ(output, this->model->getCollectionListByMuseumID(museum.getMuseumID()));
    this->model->removeUserFromDB(user);
    EXPECT_THROW(this->model->getCollectionListByMuseumID(museum.getMuseumID()), ModelException);
}

TEST_F(DevTests, TestingArtifactCollectionInput){
    User user("sena", "s@lafayette.edu", "password");
    ASSERT_NO_THROW(this->model->saveUserToDB(user));
    Museum museum("museum", "desc", "intro", user);
    ASSERT_NO_THROW(this->model->saveMuseumToDB(museum));
    Collection collection("artifact", "desc", "intro", "photo", museum);
    ASSERT_NO_THROW(this->model->saveCollectionToDB(collection));
    EXPECT_TRUE(collection.indb());
    Artifact artifact("artifact", "desc", "intro", "photo", museum);
    ASSERT_NO_THROW(this->model->saveArtifactToDB(artifact));
    EXPECT_TRUE(artifact.indb());
    ASSERT_NO_THROW(this->model->addArtifactCollection(artifact, collection));
    ASSERT_THROW(this->model->addArtifactCollection(artifact, collection), ModelException);
    EXPECT_EQ(1, this->model->getCollectionsByArtifact(artifact.getID()).size());
    EXPECT_EQ(collection, this->model->getCollectionsByArtifact(artifact.getID()).front());
    EXPECT_EQ(artifact, this->model->getArtifactsByCollection(collection.getID()).front());
    EXPECT_EQ(1, this->model->getArtifactsByCollection(collection.getID()).size());
    this->model->removeUserFromDB(user);
}
