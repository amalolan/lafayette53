#include "modelclass.h"
#include "gtest/gtest.h"

class GameTest : public ::testing::Test {
    protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    GameTest() {
    // You can do set-up work for each test here.
    }

    virtual ~GameTest() {
    // You can do clean-up work that doesn't throw exceptions here.
    }

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    virtual void SetUp() {
    // Code here will be called immediately after the constructor (right
    // before each test).
    }

    virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
    }
};

TEST_F(GameTest, testingModelClassOpenClose){
    ModelClass::initdb();
    ASSERT_TRUE(ModelClass::open());
    ASSERT_TRUE(ModelClass::status());
    ASSERT_TRUE(ModelClass::close());
    ASSERT_FALSE(ModelClass::status());


}

TEST_F(GameTest, testingJSONUserMuseumModelClass){
    ModelClass::open();
    User newUser("sena", "s@lafayette.edu", "password");
    ASSERT_TRUE(ModelClass::saveUserToDB(newUser));
    EXPECT_EQ(newUser.getJson(), ModelClass::getUserInfoJson("sena"));
    Museum museum("aMuseum", "aMuseum", newUser);
    ASSERT_TRUE(ModelClass::saveMuseumToDB(museum));
    EXPECT_EQ(museum.getJson(), ModelClass::getMuseumInfoJson(museum.getMuseumID()));
    ASSERT_TRUE(ModelClass::removeMuseumFromDB(museum));
    ASSERT_TRUE(ModelClass::removeUserFromDB(newUser));
}

TEST_F(GameTest, testingMuseumInput){
    User newUser("sena", "s@lafayette.edu", "password");
    ModelClass::open();
    ASSERT_TRUE(ModelClass::saveUserToDB(newUser));
    ASSERT_TRUE(newUser.indb());
    Museum museum("aMuseum", "a sample museum", newUser);
    ASSERT_TRUE(ModelClass::saveMuseumToDB(museum));
    ASSERT_TRUE(museum.indb());
    museum.setName("newName");
    EXPECT_TRUE(ModelClass::updateMuseumInDB(museum));
    museum.getUser().setPassword("able");
    EXPECT_EQ(newUser.getPassword(), museum.getUser().getPassword());
    ASSERT_TRUE(ModelClass::removeMuseumFromDB(museum));
    ASSERT_TRUE(!museum.indb());
    ASSERT_TRUE(ModelClass::removeUserFromDB(newUser));
    ASSERT_TRUE(!newUser.indb());
    ModelClass::close();
}

TEST_F(GameTest, TestingModelClassMuseumAndUserReturns){
    User newUser("sena", "s@lafayette.edu", "password");
    ModelClass::open();
    ASSERT_TRUE(ModelClass::saveUserToDB(newUser));
    Museum museum("aMuseum", "a sample museum", newUser);
    ASSERT_TRUE(ModelClass::saveMuseumToDB(museum));
    EXPECT_EQ(museum.getMuseumID(), ModelClass::getMuseumObject("aMuseum").getMuseumID());
    EXPECT_EQ(museum.getName(), ModelClass::getMuseumObject("aMuseum").getName());
    EXPECT_EQ(museum.getJSON(), ModelClass::getMuseumObject("aMuseum").getJSON());
    EXPECT_EQ(museum.getDescription(), ModelClass::getMuseumObject("aMuseum").getDescription());
    EXPECT_EQ(museum.getUser().getUserID(), ModelClass::getMuseumObject("aMuseum").getUser().getUserID());
    EXPECT_EQ(museum.getUser().getName(), ModelClass::getMuseumObject("aMuseum").getUser().getName());
    EXPECT_EQ(museum.getUser().getEmail(), ModelClass::getMuseumObject("aMuseum").getUser().getEmail());
    EXPECT_EQ(museum.getUser().getPassword(), ModelClass::getMuseumObject("aMuseum").getUser().getPassword());
    EXPECT_EQ(museum.getUser().getJSON(), ModelClass::getMuseumObject("aMuseum").getUser().getJSON());
    ASSERT_TRUE(ModelClass::removeMuseumFromDB(museum));
    ASSERT_TRUE(!museum.indb());
    ASSERT_TRUE(ModelClass::removeUserFromDB(newUser));
    ASSERT_TRUE(!newUser.indb());
}

TEST_F(GameTest, TestingModelClassExceptions){
    User newUser("sena", "s@lafayette.edu", "password");
    ModelClass::open();
    ASSERT_THROW(ModelClass::updateUserInDB(newUser), ModelException);
    ASSERT_THROW(ModelClass::removeUserFromDB(newUser), ModelException);
    ASSERT_THROW(ModelClass::getMuseumObject(""), ModelException);
    ASSERT_THROW(ModelClass::getPasswordHash(""), ModelException);
    ASSERT_THROW(ModelClass::getUserObject(""), ModelException);
    ASSERT_THROW(ModelClass::getUserObject(-1), ModelException);

    Museum museum("aMuseum", "a sample museum", newUser);
    ASSERT_THROW(ModelClass::saveMuseumToDB(museum), ModelException);
    ASSERT_THROW(ModelClass::updateMuseumInDB(museum), ModelException);
    ASSERT_THROW(ModelClass::removeMuseumFromDB(museum), ModelException);

    ASSERT_NO_THROW(ModelClass::saveUserToDB(newUser));
    ASSERT_THROW(ModelClass::saveMuseumToDB(museum), ModelException);
    museum.getUser().setUserID(0);
    ASSERT_THROW(ModelClass::saveMuseumToDB(museum), ModelException);
    museum.setUser(newUser); //Remember that museum objects have no connection with user objects.
    //User object has to be reset to connect museum with User
    ASSERT_NO_THROW(ModelClass::saveMuseumToDB(museum));
    ASSERT_NO_THROW(ModelClass::updateUserInDB(newUser));
    ASSERT_NO_THROW(ModelClass::removeMuseumFromDB(museum));
    ASSERT_NO_THROW(ModelClass::removeUserFromDB(newUser));
    Museum museum2("", "", newUser);
    ASSERT_THROW(ModelClass::saveMuseumToDB(museum), ModelException);
    User user2("","","");
    ASSERT_THROW(ModelClass::saveUserToDB(user2), ModelException);
    ModelClass::close();
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

