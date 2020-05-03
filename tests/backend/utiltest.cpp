#include "utiltest.h"

/**
 * @brief TEST_F Tests Util::validateJSON()
 * 12 Test Cases:
 * 1. Empty object, empty keys list.
 * 2. Empty object, non-empty keys list.
 * 3. Singleton object, singleton keys list, key in object.
 * 4. Singleton object, empty list of keys.
 * 5. Singleton object, singleton keys list, key not in object.
 * 6. Singleton object, non-singleton keys list, one key in object the other key(s) not in object.
 * 7. Non-singleton object (many key value pairs, including nested objects), keys list with all keys in main object.
 * 8. Non-singleton object, all keys from keylist in object, but not all of the objects' keys in  keylist.
 * 9. Non-singleton object, empty keylist.
 * 10. Non-singleton object, some, but not all, keys from keylist in object.
 * 11. Non-singleton object, all objects' keys in keylist and one key in keylist and not in object.
 * 12. Non-singleton object, nested objects' key in keylist, rest of the keys in keylist are in object.
 */
TEST_F(UtilTest, validateJSON) {
    json obj =  json::object();
    ASSERT_NO_THROW(Util::validateJSON(obj, {})); /**< Case 1 */
    EXPECT_THROW(Util::validateJSON(obj, {"a", "b", "c"}), json::other_error); /**< Case 2 */


    obj = {{"a", true}};
    ASSERT_NO_THROW(Util::validateJSON(obj, {"a"}));     /**< Case 3 */
    ASSERT_NO_THROW(Util::validateJSON(obj, {}));     /**< Case 4 */
    EXPECT_THROW(Util::validateJSON(obj, {"aa"}), json::other_error);     /**< Case 5 */
    EXPECT_THROW(Util::validateJSON(obj, {"a", "b", "c"}), json::other_error);     /**< Case 6 */


    obj = {
        {"1", 3},
        {"31",  "12"},
        {"keya",
         {{"a,b", 23.4},  {"b", true}} // Validating for b must result in an exception as it is in a nested object.
        },
        {"extra", 1.21}
    };

    ASSERT_NO_THROW(Util::validateJSON(obj, {"1", "31", "keya", "extra"}));     /**< Case 7 */
    ASSERT_NO_THROW(Util::validateJSON(obj, {"1", "31", "keya"}));     /**< Case 8 */
    ASSERT_NO_THROW(Util::validateJSON(obj, {}));     /**< Case 9 */
    EXPECT_THROW(Util::validateJSON(obj, {"11", "31", "keya", "extra"}), json::other_error);     /**< Case 10 */
    EXPECT_THROW(Util::validateJSON(obj, {"1", "31", "keya", "extra", "e231"}), json::other_error);     /**< Case 11 */
    EXPECT_THROW(Util::validateJSON(obj, {"1", "31", "keya","b", "extra"}), json::other_error);     /**< Case 12 */


}

/**
 * @brief TEST_F Tests Util::getObjectWithKeys()
 * 12 Test Cases:
 * 1. T.toJSON() = {}, Empty keys list.
 * 2. T.toJSON() = {}, Non-empty keys list.
 * 3. T.toJSON() = singleton object, empty keys list.
 * 4. T.toJSON() = singleton object, singleton keys list, key in object.
 * 5. T.toJSON() = singleton object, singleton keys list, key not in object.
 * 6. T.toJSON() = singleton object, non-singleton keys list, one key in object the other key(s) not in object.
 * 7. T.toJSON() = non-singleton object (many key value pairs, including nested objects), keys list with all keys in main object.
 * 8. T.toJSON() = non-singleton object, all keys from keylist in object, but not all of the objects' keys in  keylist.
 * 9. T.toJSON() = non-singleton object, empty keylist.
 * 10. T.toJSON() = non-singleton object, some, but not all, keys from keylist in object.
 * 11. T.toJSON() = non-singleton object, all objects' keys in keylist and one key in keylist and not in object.
 * 12. T.toJSON() = non-singleton object, nested objects' key in keylist, rest of the keys in keylist are in object.
 */
TEST_F(UtilTest, getObjectWithKeys) {
    MockObject* mock = new MockObject();
    {
        InSequence s;
        json objectJSON = json::object();
        json expectation = json::object();
        /**< Empty T.toJSON() */
        EXPECT_CALL(*mock, toJSON())
                .Times(2)
                .WillRepeatedly(Return(objectJSON))
                .RetiresOnSaturation();
        ASSERT_EQ(Util::getObjectWithKeys<MockObject*>(mock, {}), expectation); /**< Case 1 */
        EXPECT_THROW(Util::getObjectWithKeys<MockObject*>(mock, {"a", "b", "c"}), json::other_error); /**< Case 2 */

        /**< Singleton T.toJSON() */
        objectJSON = {{"a", true}};
        EXPECT_CALL(*mock, toJSON())
                .Times(4)
                .WillRepeatedly(Return(objectJSON))
                .RetiresOnSaturation();

        ASSERT_EQ(Util::getObjectWithKeys<MockObject*>(mock, {}), expectation); /**< Case 3 */
        expectation = {{"a", true}};
        ASSERT_EQ(Util::getObjectWithKeys<MockObject*>(mock, {"a"}), expectation); /**< Case 4 */
        EXPECT_THROW(Util::getObjectWithKeys<MockObject*>(mock, {"aa"}), json::other_error); /**< Case 5 */
        EXPECT_THROW(Util::getObjectWithKeys<MockObject*>(mock, {"a", "b", "c"}), json::other_error); /**< Case 6 */

        /**< Non-singleton T.toJSON() */
        objectJSON = {
            {"1", 3},
            {"31",  "12"},
            {"keya",
             {{"a,b", 23.4},  {"b", true}} // Looking for b must result in an exception as it is in a nested object.
            },
            {"extra", 1.21}
        };
        EXPECT_CALL(*mock, toJSON())
                .Times(6)
                .WillRepeatedly(Return(objectJSON))
                .RetiresOnSaturation();

        expectation = {
            {"1", 3},
            {"31",  "12"},
            {"keya",
             {{"a,b", 23.4},  {"b", true}}
            },
            {"extra", 1.21}
        };
        ASSERT_EQ(Util::getObjectWithKeys<MockObject*>(mock, {"1", "31", "keya", "extra"}), expectation); /**< Case 7 */

        expectation = {
            {"1", 3},
            {"31",  "12"},
        };
        ASSERT_EQ(Util::getObjectWithKeys<MockObject*>(mock, {"1", "31"}), expectation); /**< Case 8 */
        expectation = json::object();
        ASSERT_EQ(Util::getObjectWithKeys<MockObject*>(mock, {}), expectation); /**< Case 9 */
        EXPECT_THROW(Util::getObjectWithKeys<MockObject*>(mock, {"11", "31", "keya", "extra"}), json::other_error);     /**< Case 10 */
        EXPECT_THROW(Util::getObjectWithKeys<MockObject*>(mock, {"1", "31", "keya", "extra", "e231"}), json::other_error);     /**< Case 11 */
        EXPECT_THROW(Util::getObjectWithKeys<MockObject*>(mock, {"1", "31", "keya","b", "extra"}), json::other_error);     /**< Case 12 */
    }
    delete mock;
}

/**
 * @brief TEST_F Tests Util::arrayFromVector()
 * 8 Test Cases:
 * 1. Empty vector
 * 2. Empty vector, non-empty keys list.
 * 3. Non-empty vector, all and only keys in top level of object in keyslist.
 * 4. Non-empty vector, only keys in top level of object in keyslist, not all object's keys in keylist.
 * 5. Non-empty vector, empty keys list.
 * 6. Non-empty vector, not all keys in keyslist in vector.
 * 7. Non-empty vector, all objects' keys in keylist and one key in keylist and not in object.
 * 8. Non-empty vector, nested objects' key in keylist, rest of the keys in keylist are in object.
 */
TEST_F(UtilTest, arrayFromVector) {
    vector<MockObject*> mockList;
    json objectJSON = json::object();
    json expectation = json::array();
    /**< Empty vector<T> */
    json result = Util::arrayFromVector<MockObject*>(mockList, {});
    ASSERT_EQ(result, expectation); /**< Case 1 */
    result = Util::arrayFromVector<MockObject*>(mockList, {"1", "2"});
    ASSERT_EQ(result, expectation); /**< Case 2 */

    /**< Non-empty vector<T> */
    objectJSON = {
        {"1", 3},
        {"31",  "12"},
        {"keya",
         {{"a,b", 23.4},  {"b", true}} // Looking for b must result in an exception as it is in a nested object.
        },
        {"extra", 1.21}
    };
    for (int i = 0; i < 4; i++) {
        mockList.push_back(new MockObject());
        expectation.push_back(objectJSON);
    }
    EXPECT_CALL(*(mockList).at(0), toJSON()) /**< Will be called for all cases 3 - 8 */
            .Times(6)
            .WillRepeatedly(Return(objectJSON));
    for (int i = 1; i < 4; i++) {
        EXPECT_CALL(*(mockList.at(i)), toJSON())
                .Times(3) /**< Will be called only for cases 3 - 5 */
                .WillRepeatedly(Return(objectJSON));

    }
    result = Util::arrayFromVector<MockObject*>(mockList, {"1", "31", "keya", "extra"});
    ASSERT_EQ(result, expectation); /**< Case 3 */

    expectation = json::array();
    for (int i = 0; i < 4; i++) {
        expectation.push_back({
                                  {"1", 3},
                                  {"31",  "12"},
                              });
    }
    ASSERT_EQ(Util::arrayFromVector<MockObject*>(mockList, {"1", "31"}), expectation); /**< Case 4 */

    expectation = json::array();
    for (int i = 0; i < 4; i++) {
        expectation.push_back(json::object());
    }
    ASSERT_EQ(Util::arrayFromVector<MockObject*>(mockList, {}), expectation); /**< Case 5 */

    EXPECT_THROW(Util::arrayFromVector<MockObject*>(mockList, {"11", "31", "keya", "extra"}), json::other_error);     /**< Case 6 */
    EXPECT_THROW(Util::arrayFromVector<MockObject*>(mockList, {"1", "31", "keya", "extra", "e231"}), json::other_error);     /**< Case 7 */
    EXPECT_THROW(Util::arrayFromVector<MockObject*>(mockList, {"1", "31", "keya","b", "extra"}), json::other_error);     /**< Case 8 */
    for (MockObject* mock: mockList) {
        delete mock;
    }
}

/**
 * @brief TEST_F Tests Util::checkLogin() Helper test function to ensure log in actually works.
 * Called by many POST request test functions.
 * Validity of user JSON objects  are tested in model tests.
 * Validity of Username, Password asserted on front-end.
 * 4 Test Cases:
 * 1. Invalid POST data (no username & password).
 * 2. Valid data, Username not found in DB.
 * 3. Valid data, Username Found, Password mismatch.
 * 4. Valid data, Username Found, Password match.
 * @param url The POST url to perform the login test on.
 */
TEST_F(UtilTest, checkLogin) {
    string username;
    json data;
    json expectation;
    MockModelClass model;
    {
        InSequence s;
        /**< Case 1 */
        data = {
            {"username",  "anfann12"},
            {"passwor12d", "password~*?"}
        };
        EXPECT_THROW(Util::checkLogin(data, &model), json::other_error); /**< Case 1 */

        /**< Case 2 */
        data = {
            {"username",  "anfann12"},
            {"password", "password~*?"}
        };
        EXPECT_CALL(model, getUserObject((string) data["username"]))
                .WillOnce(testing::Throw(ModelException("Can't find User")))
                .RetiresOnSaturation();

        EXPECT_THROW(Util::checkLogin(data, &model), ModelException); /**< Case 2 */

        /**< Case 3 */
        User user(data["username"],"email@example.com", "realpassword", 12);
        EXPECT_CALL(model, getUserObject((string) data["username"]))
                .WillOnce(testing::Return(user))
                .RetiresOnSaturation();
        EXPECT_THROW(Util::checkLogin(data, &model), BackendException); /**< Case 3 */

        /**< Case 4 */
        data["password"] = "realpassword";
        EXPECT_CALL(model, getUserObject((string) data["username"]))
                .Times(testing::AtLeast(1))
                .WillOnce(testing::Return(user))
                .RetiresOnSaturation();

        ASSERT_EQ(user, Util::checkLogin(data, &model)); /**< Case 4 */
    }
}

/**
 * @brief TEST_F Tests Util::getSuccessJsonStr()
 * 2 Test Cases:
 * 1. Empty string message
 * 2. Non-empty string message
 */
TEST_F(UtilTest, successJSON) {
    /**< Case 1 */
    string message = "";
    json j2 = {
        {"success", true},
        {"message", message}
    };
    ASSERT_EQ(json::parse(Util::getSuccessJsonStr(message)), j2);

    /**< Case 2 */
    message = "hi";
    json j1 = {
        {"success", true},
        {"message", message}
    };
    ASSERT_EQ(json::parse(Util::getSuccessJsonStr(message)), j1);
}

/**
 * @brief TEST_F Tests Util::getFailureJsonStr()
 * 2 Test Cases:
 * 1. Empty string message
 * 2. Non-empty string message
 */
TEST_F(UtilTest, failureJSON) {
    /**< Case 1 */
    string message = "";
    json j2 = {
        {"success", false},
        {"message", message}
    };
    ASSERT_EQ(json::parse(Util::getFailureJsonStr(message)), j2);

    /**< Case 2 */
    message = "hi";
    json j1 = {
        {"success", false},
        {"message", message}
    };
    ASSERT_EQ(json::parse(Util::getFailureJsonStr(message)), j1);
}

/**
 * @brief TEST_F Tests Util::getCollectionEdit()
 * 1 Test Case
 * 1. Simple Edit::edit and Edit::pending
 */
TEST_F(UtilTest, getCollectionEdit) {
    User user("curator", "email", "password", 3);
    Museum museum("museumName", "museumDescription", "museumIntroduction", "museumImage", user, 55);
    Collection collection("collectionName", "collectionDescription", "collectionIntroduction", "collectionImage", museum, 32);
    Edit<Collection> edit(collection, Edit<Collection>::edit, user, Edit<Collection>::pending, 2);
    json expectation = {
        {"id", 2},
        {"type", "Edit"},
        {"category", "collection"},
        {"approvalStatus","Under review"},
        {"reviewer", {{"username", "curator"}}},
        {"collection", {
             {"collection", {
                  {"id", 32},
                  {"name", "collectionName"},
                  {"description", "collectionDescription"},
                  {"introduction", "collectionIntroduction"},
                  {"image", "collectionImage"}
              }},
             {"museum", {
                  {"id", 55},
                  {"name", "museumName"}
              }}
         }
        }
    };
    json result = Util::getCollectionEdit(edit);
    ASSERT_EQ(result, expectation); /**< Case 1 */
}


/**
 * @brief TEST_F Tests Util::getArtifactEdit()
 * 3 Test Case
 * 1. Empty collection list, Edit::add and Edit::approve type.
 * 2. Singleton collection list, Edit::del and Edit::reject type.
 * 3. Non-singleton collection list, Edit::edit and Edit::approve type.
 */
TEST_F(UtilTest, getArtifactEdit) {
    User user("curator", "email", "password", 3);
    Museum museum("museumName", "museumDescription", "museumIntroduction", "museumImage", user, 55);
    vector<Collection> collectionList = {
        Collection("collection2Name", "collection2Description", "collection2Introduction", "collection2Image", museum, 33),
    };
    Artifact artifact("artifactName", "artifactDescription", "artifactIntroduction", "artifactImage", museum, 1232);
    Edit<Artifact> edit1(artifact, Edit<Artifact>::add, user, {}, Edit<Artifact>::approve, 1);
    json expectation = {
        {"id", 1},
        {"type", "Addition"},
        {"category", "artifact"},
        {"approvalStatus","Approved"},
        {"reviewer", {{"username", "curator"}}},
        {"artifact", {
             {"artifact", {
                  {"id", 1232},
                  {"name", "artifactName"},
                  {"description", "artifactDescription"},
                  {"introduction", "artifactIntroduction"},
                  {"image", "artifactImage"}
              }},
             {"museum", {
                  {"id", 55},
                  {"name", "museumName"}
              }},
             {"collectionList", json::array()}
         }
        }
    };
    json result = Util::getArtifactEdit(edit1);
    cout<<result.dump(2)<<endl;
    cout<<expectation.dump(2)<<endl;
    ASSERT_EQ(result, expectation); /**< Case 1 */

    Edit<Artifact> edit2(artifact, Edit<Artifact>::del, user, collectionList, Edit<Artifact>::reject, 2);
    expectation = {
        {"id", 2},
        {"type", "Deletion"},
        {"category", "artifact"},
        {"approvalStatus","Rejected"},
        {"reviewer", {{"username", "curator"}}},
        {"artifact", {
             {"artifact", {
                  {"id", 1232},
                  {"name", "artifactName"},
                  {"description", "artifactDescription"},
                  {"introduction", "artifactIntroduction"},
                  {"image", "artifactImage"}
              }},
             {"museum", {
                  {"id", 55},
                  {"name", "museumName"}
              }},
             {"collectionList", {
                  {
                      {"id", 33},
                      {"name", "collection2Name"}
                  }
              }}
         }
        }
    };
    result = Util::getArtifactEdit(edit2);
    cout<<result.dump(2)<<endl;
    cout<<expectation.dump(2)<<endl;
    ASSERT_EQ(result, expectation); /**< Case 2 */

    collectionList.push_back(Collection("collectionName", "collectionDescription", "collectionIntroduction", "collectionImage", museum, 32));
    Edit<Artifact> edit3(artifact, Edit<Artifact>::edit, user, collectionList, Edit<Artifact>::approve, 3);
    expectation = {
        {"id", 3},
        {"type", "Edit"},
        {"category", "artifact"},
        {"approvalStatus","Approved"},
        {"reviewer", {{"username", "curator"}}},
        {"artifact", {
             {"artifact", {
                  {"id", 1232},
                  {"name", "artifactName"},
                  {"description", "artifactDescription"},
                  {"introduction", "artifactIntroduction"},
                  {"image", "artifactImage"}
              }},
             {"museum", {
                  {"id", 55},
                  {"name", "museumName"}
              }},
             {"collectionList", {
                  {
                      {"id", 33},
                      {"name", "collection2Name"}
                  },
                  {
                      {"id", 32},
                      {"name", "collectionName"}
                  }

              }}
         }
        }
    };
    result = Util::getArtifactEdit(edit3);
    ASSERT_EQ(result, expectation); /**< Case 3 */

}

