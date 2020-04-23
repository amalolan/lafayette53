#include "devtests.h"



TEST_F(DevTests, testingModelClassOpenClose){
//    this->model->initdb();
    ASSERT_TRUE(this->model->open());
    ASSERT_TRUE(this->model->status());
    ASSERT_TRUE(this->model->close());
    ASSERT_FALSE(this->model->status());

}

TEST_F(DevTests, testingJSONUserMuseumModelClass){
    this->model->open();
    User newUser("sena", "s@lafayette.edu", "password");
    EXPECT_NO_THROW(this->model->saveUserToDB(newUser));
    EXPECT_EQ(newUser.getJson(), this->model->getUserInfoJson("sena"));
    Museum museum("aMuseum", "aMuseum", newUser);
    EXPECT_NO_THROW(this->model->saveMuseumToDB(museum));
    EXPECT_EQ(museum.getJson(), this->model->getMuseumInfoJson(museum.getMuseumID()));
    EXPECT_NO_THROW(this->model->removeMuseumFromDB(museum));
    EXPECT_NO_THROW(this->model->removeUserFromDB(newUser));
}

TEST_F(DevTests, testingMuseumInput){
    User newUser("sena", "s@lafayette.edu", "password");
    EXPECT_NO_THROW(this->model->saveUserToDB(newUser));
    EXPECT_NO_THROW(newUser.indb());
    Museum museum("aMuseum", "a sample museum", newUser);
    EXPECT_NO_THROW(this->model->saveMuseumToDB(museum));
    EXPECT_NO_THROW(museum.indb());
    museum.setName("newName");
    EXPECT_NO_THROW(this->model->updateMuseumInDB(museum));
    museum.getUser().setPassword("able");
    EXPECT_EQ(newUser.getPassword(), museum.getUser().getPassword());
    EXPECT_NO_THROW(this->model->removeMuseumFromDB(museum));
    EXPECT_TRUE(!museum.indb());
    EXPECT_NO_THROW(this->model->removeUserFromDB(newUser));
    EXPECT_TRUE(!newUser.indb());
}

TEST_F(DevTests, TestingModelClassMuseumAndUserReturns){
    User newUser("sena", "s@lafayette.edu", "password");
    this->model->open();
    EXPECT_NO_THROW(this->model->saveUserToDB(newUser));
    Museum museum("aMuseum", "a sample museum", newUser);
    EXPECT_NO_THROW(this->model->saveMuseumToDB(museum));
    EXPECT_EQ(museum.getMuseumID(), this->model->getMuseumObject("aMuseum").getMuseumID());
    EXPECT_EQ(museum.getName(), this->model->getMuseumObject("aMuseum").getName());
    EXPECT_EQ(museum.getJSON(), this->model->getMuseumObject("aMuseum").getJSON());
    EXPECT_EQ(museum.getDescription(), this->model->getMuseumObject("aMuseum").getDescription());
    EXPECT_EQ(museum.getUser().getUserID(), this->model->getMuseumObject("aMuseum").getUser().getUserID());
    EXPECT_EQ(museum.getUser().getName(), this->model->getMuseumObject("aMuseum").getUser().getName());
    EXPECT_EQ(museum.getUser().getEmail(), this->model->getMuseumObject("aMuseum").getUser().getEmail());
    EXPECT_EQ(museum.getUser().getPassword(), this->model->getMuseumObject("aMuseum").getUser().getPassword());
    EXPECT_EQ(museum.getUser().getJSON(), this->model->getMuseumObject("aMuseum").getUser().getJSON());
    EXPECT_NO_THROW(this->model->removeMuseumFromDB(museum));
    ASSERT_TRUE(!museum.indb());
    EXPECT_NO_THROW(this->model->removeUserFromDB(newUser));
    ASSERT_TRUE(!newUser.indb());
}

TEST_F(DevTests, TestingModelClassExceptions){
    User newUser("sena", "s@lafayette.edu", "password");
    ASSERT_THROW(this->model->updateUserInDB(newUser), ModelException);
    ASSERT_THROW(this->model->removeUserFromDB(newUser), ModelException);
    ASSERT_THROW(this->model->getMuseumObject(""), ModelException);
    ASSERT_THROW(this->model->getPasswordHash(""), ModelException);
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
