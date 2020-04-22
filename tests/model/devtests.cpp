#include "devtests.h"


TEST_F(DevTests, testingModelClassOpenClose){
//    std::cout<<CODE_BASE_DIRECTORY<<std::endl;
//    QDir dir;
//    dir.cd("../../../");
//    std::cout<<dir.absolutePath().toStdString()<<std::endl;
    ModelClassExt::initdb(CODE_BASE_DIRECTORY);
    ASSERT_TRUE(ModelClassExt::open());
    ASSERT_TRUE(ModelClassExt::status());
    ASSERT_TRUE(ModelClassExt::close());
    ASSERT_FALSE(ModelClassExt::status());


}

TEST_F(DevTests, testingModelClassReadInfo){
   ModelClassExt::initdb(CODE_BASE_DIRECTORY);
   ModelClassExt::open();
   QJsonObject object;
   object["username"] = "Lekso_";
   object["email"] = "borashvl@lafayette.edu";
   QJsonDocument doc;
   doc.setObject(object);
   ASSERT_EQ(doc.toJson().toStdString(), ModelClassExt::getUserInfoJSON(0));

   QJsonObject museumObj;
   museumObj["name"] = "Lekso's Space";
   museumObj["introduction"] = "This is Lekso's Space";
   museumObj["description"] = "This is my space here!";
   museumObj["museumID"] = 0;
   museumObj["userID"] = 0;
   QJsonDocument museumDoc;
   museumDoc.setObject(museumObj);
   ASSERT_EQ(museumDoc.toJson().toStdString(), ModelClassExt::getMuseumInfoJSON(0));

   QJsonArray museumArray;
   QJsonObject museum0;
   QJsonObject museum1;
   QJsonObject museum2;
   QJsonObject museum3;
   museum0["name"] = "Lekso's Space";
   museum1["name"] = "Peter's Space";
   museum2["name"] = "Sena's Space";
   museum3["name"] = "Malo's Space";

   museum0["introduction"] = "This is Lekso's Space";
   museum1["introduction"] = "This is Peter's Space";
   museum2["introduction"] = "This is Sena's Space";
   museum3["introduction"] = "This is Malo's Space";

   museum0["description"] = "This is my space here!";
   museum1["description"] = "This is my space here!";
   museum2["description"] = "This is my space here!";
   museum3["description"] = "This is my space here!";

   museum0["museumID"] = 0;
   museum1["museumID"] = 1;
   museum2["museumID"] = 2;
   museum3["museumID"] = 3;

   museum0["userID"] = 0;
   museum1["userID"] = 1;
   museum2["userID"] = 2;
   museum3["userID"] = 3;
   museumArray.append(museum0);
   museumArray.append(museum1);
   museumArray.append(museum2);
   museumArray.append(museum3);
   QJsonDocument museumListDoc;
   museumListDoc.setArray(museumArray);
   ASSERT_EQ(museumListDoc.toJson().toStdString(), ModelClassExt::getMuseumListJSON());
   ModelClassExt::close();
}

TEST_F(DevTests, testingUserInput){
    User newUser("yevs", "yvs@lafayette.edu", "password");
    ModelClassExt::open();
    ASSERT_TRUE(ModelClassExt::saveUserToDB(newUser));
    ASSERT_TRUE(newUser.indb());
    EXPECT_EQ(ModelClassExt::getPasswordHash(newUser.getName()), "password");
    EXPECT_EQ(newUser.getEmail(), ModelClassExt::getUserObject("yevs").getEmail());
    EXPECT_EQ(newUser.getPassword(), ModelClassExt::getUserObject("yevs").getPassword());
    EXPECT_EQ(newUser.getUserID(), ModelClassExt::getUserObject("yevs").getUserID());
    EXPECT_EQ(newUser.getJSON(), ModelClassExt::getUserObject("yevs").getJSON());
    EXPECT_EQ(newUser.getName(), ModelClassExt::getUserObject("yevs").getName());

    EXPECT_EQ(ModelClassExt::getPasswordHash(newUser.getName()), "password");
    EXPECT_EQ(newUser.getEmail(), ModelClassExt::getUserObject(newUser.getUserID()).getEmail());
    EXPECT_EQ(newUser.getPassword(), ModelClassExt::getUserObject(newUser.getUserID()).getPassword());
    EXPECT_EQ(newUser.getUserID(), ModelClassExt::getUserObject(newUser.getUserID()).getUserID());
    EXPECT_EQ(newUser.getJSON(), ModelClassExt::getUserObject(newUser.getUserID()).getJSON());
    EXPECT_EQ(newUser.getName(), ModelClassExt::getUserObject(newUser.getUserID()).getName());

    newUser.setEmail("yevenyos");
    EXPECT_TRUE(ModelClassExt::updateUserInDB(newUser));
    ASSERT_TRUE(ModelClassExt::removeUserFromDB(newUser));
    ASSERT_TRUE(!newUser.indb());
    ModelClassExt::close();
}

TEST_F(DevTests, testingMuseumInput){
    User newUser("sena", "s@lafayette.edu", "password");
    ModelClassExt::open();
    ASSERT_TRUE(ModelClassExt::saveUserToDB(newUser));
    ASSERT_TRUE(newUser.indb());
    Museum museum("aMuseum", "a sample museum", newUser);
    ASSERT_TRUE(ModelClassExt::saveMuseumToDB(museum));
    ASSERT_TRUE(museum.indb());
    museum.setName("newName");
    EXPECT_TRUE(ModelClassExt::updateMuseumInDB(museum));
    museum.getUser().setPassword("able");
    EXPECT_EQ(newUser.getPassword(), museum.getUser().getPassword());
    ASSERT_TRUE(ModelClassExt::removeMuseumFromDB(museum));
    ASSERT_TRUE(!museum.indb());
    ASSERT_TRUE(ModelClassExt::removeUserFromDB(newUser));
    ASSERT_TRUE(!newUser.indb());
    ModelClassExt::close();
}

TEST_F(DevTests, TestingModelClassMuseumAndUserReturns){
    User newUser("sena", "s@lafayette.edu", "password");
    ModelClassExt::open();
    ASSERT_TRUE(ModelClassExt::saveUserToDB(newUser));
    Museum museum("aMuseum", "a sample museum", newUser);
    ASSERT_TRUE(ModelClassExt::saveMuseumToDB(museum));
    EXPECT_EQ(museum.getMuseumID(), ModelClassExt::getMuseumObject("aMuseum").getMuseumID());
    EXPECT_EQ(museum.getName(), ModelClassExt::getMuseumObject("aMuseum").getName());
    EXPECT_EQ(museum.getJSON(), ModelClassExt::getMuseumObject("aMuseum").getJSON());
    EXPECT_EQ(museum.getDescription(), ModelClassExt::getMuseumObject("aMuseum").getDescription());
    EXPECT_EQ(museum.getUser().getUserID(), ModelClassExt::getMuseumObject("aMuseum").getUser().getUserID());
    EXPECT_EQ(museum.getUser().getName(), ModelClassExt::getMuseumObject("aMuseum").getUser().getName());
    EXPECT_EQ(museum.getUser().getEmail(), ModelClassExt::getMuseumObject("aMuseum").getUser().getEmail());
    EXPECT_EQ(museum.getUser().getPassword(), ModelClassExt::getMuseumObject("aMuseum").getUser().getPassword());
    EXPECT_EQ(museum.getUser().getJSON(), ModelClassExt::getMuseumObject("aMuseum").getUser().getJSON());
    ASSERT_TRUE(ModelClassExt::removeMuseumFromDB(museum));
    ASSERT_TRUE(!museum.indb());
    ASSERT_TRUE(ModelClassExt::removeUserFromDB(newUser));
    ASSERT_TRUE(!newUser.indb());
}



