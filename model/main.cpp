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

TEST_F(GameTest, testingModelClassOpenClose){
    ModelClass::initdb();
    ASSERT_TRUE(ModelClass::open());
    ASSERT_TRUE(ModelClass::status());
    ASSERT_TRUE(ModelClass::close());
    ASSERT_FALSE(ModelClass::status());


}

TEST_F(GameTest, testingModelClassReadInfo){
   ModelClass::initdb();
   ModelClass::open();
   QJsonObject object;
   object["username"] = "Lekso_";
   object["email"] = "borashvl@lafayette.edu";
   QJsonDocument doc;
   doc.setObject(object);
   ASSERT_EQ(doc.toJson().toStdString(), ModelClass::getUserInfoJSON(0));

   QJsonObject museumObj;
   museumObj["name"] = "Lekso's Space";
   museumObj["description"] = "This is my space here!";
   QJsonDocument museumDoc;
   museumDoc.setObject(museumObj);
   ASSERT_EQ(museumDoc.toJson().toStdString(), ModelClass::getMuseumInfoJSON(0));

   QJsonArray museumArray;
   QJsonObject museum0;
   QJsonObject museum1;
   QJsonObject museum2;
   QJsonObject museum3;
   museum0["name"] = "Lekso's Space";
   museum1["name"] = "Peter's Space";
   museum2["name"] = "Sena's Space";
   museum3["name"] = "Malo's Space";
   museum0["description"] = "This is my space here!";
   museum1["description"] = "This is my space here!";
   museum2["description"] = "This is my space here!";
   museum3["description"] = "This is my space here!";
   museum0["id"] = "0";
   museum1["id"] = "1";
   museum2["id"] = "2";
   museum3["id"] = "3";
   museumArray.append(museum0);
   museumArray.append(museum1);
   museumArray.append(museum2);
   museumArray.append(museum3);
   QJsonDocument museumListDoc;
   museumListDoc.setArray(museumArray);
   ASSERT_EQ(museumListDoc.toJson().toStdString(), ModelClass::getMuseumListJSON());
}



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

