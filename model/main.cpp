#include "gtest/gtest.h"
#include "modelclass.h"

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

TEST_F(GameTest, testingModelClass){
    ModelClass::initdb();
    ASSERT_TRUE(ModelClass::open());
    ASSERT_TRUE(ModelClass::status());
    ASSERT_TRUE(ModelClass::close());
    ASSERT_FALSE(ModelClass::status());

    ModelClass::open();
    QJsonObject object;
    object["username"] = "Lekso_";
    object["email"] = "borashvl@lafayette.edu";
    QJsonDocument doc;
    doc.setObject(object);
    ASSERT_EQ(doc.toJson().toStdString(), ModelClass::getUserInfoJSON(0));
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

