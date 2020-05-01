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
    EXPECT_THROW(Util::validateJSON(obj, {"a", "b", "c"}), std::exception); /**< Case 2 */


    obj = {{"a", true}};
    ASSERT_NO_THROW(Util::validateJSON(obj, {"a"}));     /**< Case 3 */
    ASSERT_NO_THROW(Util::validateJSON(obj, {}));     /**< Case 4 */
    EXPECT_THROW(Util::validateJSON(obj, {"aa"}), std::exception);     /**< Case 5 */
    EXPECT_THROW(Util::validateJSON(obj, {"a", "b", "c"}), std::exception);     /**< Case 6 */


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
    EXPECT_THROW(Util::validateJSON(obj, {"11", "31", "keya", "extra"}), std::exception);     /**< Case 10 */
    EXPECT_THROW(Util::validateJSON(obj, {"1", "31", "keya", "extra", "e231"}), std::exception);     /**< Case 11 */
    EXPECT_THROW(Util::validateJSON(obj, {"1", "31", "keya","b", "extra"}), std::exception);     /**< Case 12 */


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
        EXPECT_THROW(Util::getObjectWithKeys<MockObject*>(mock, {"a", "b", "c"}), std::exception); /**< Case 2 */

        /**< Singleton T.toJSON() */
        objectJSON = {{"a", true}};
        EXPECT_CALL(*mock, toJSON())
                .Times(4)
                .WillRepeatedly(Return(objectJSON))
                .RetiresOnSaturation();

        ASSERT_EQ(Util::getObjectWithKeys<MockObject*>(mock, {}), expectation); /**< Case 3 */
        expectation = {{"a", true}};
        ASSERT_EQ(Util::getObjectWithKeys<MockObject*>(mock, {"a"}), expectation); /**< Case 4 */
        EXPECT_THROW(Util::getObjectWithKeys<MockObject*>(mock, {"aa"}), std::exception); /**< Case 5 */
        EXPECT_THROW(Util::getObjectWithKeys<MockObject*>(mock, {"a", "b", "c"}), std::exception); /**< Case 6 */

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
        EXPECT_THROW(Util::getObjectWithKeys<MockObject*>(mock, {"11", "31", "keya", "extra"}), std::exception);     /**< Case 10 */
        EXPECT_THROW(Util::getObjectWithKeys<MockObject*>(mock, {"1", "31", "keya", "extra", "e231"}), std::exception);     /**< Case 11 */
        EXPECT_THROW(Util::getObjectWithKeys<MockObject*>(mock, {"1", "31", "keya","b", "extra"}), std::exception);     /**< Case 12 */
    }
    delete mock;
}

/**
 * @brief TEST_F Tests Util::arrayFromVector()
 * Test Cases
 */
TEST_F(UtilTest, arrayFromVector) {
    vector<MockObject*> mockList;
    json objectJSON = json::object();
    json expectation = json::array();
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
    EXPECT_CALL(*(mockList).at(0), toJSON()) /**< Will be called for all cases 7 - 12 */
            .Times(6)
            .WillRepeatedly(Return(objectJSON));
    for (int i = 1; i < 4; i++) {
        EXPECT_CALL(*(mockList.at(i)), toJSON())
            .Times(3) /**< Will be called only for cases 7 - 9 */
            .WillRepeatedly(Return(objectJSON));

    }
    result = Util::arrayFromVector<MockObject*>(mockList, {"1", "31", "keya", "extra"});
    ASSERT_EQ(result, expectation); /**< Case 7 */

    expectation = json::array();
    for (int i = 0; i < 4; i++) {
        expectation.push_back({
                                  {"1", 3},
                                  {"31",  "12"},
                              });
    }
    ASSERT_EQ(Util::arrayFromVector<MockObject*>(mockList, {"1", "31"}), expectation); /**< Case 8 */

    expectation = json::array();
    for (auto mock: mockList) {
        expectation.push_back(json::object());
    }
    ASSERT_EQ(Util::arrayFromVector<MockObject*>(mockList, {}), expectation); /**< Case 9 */

    EXPECT_THROW(Util::arrayFromVector<MockObject*>(mockList, {"11", "31", "keya", "extra"}), std::exception);     /**< Case 10 */
    EXPECT_THROW(Util::arrayFromVector<MockObject*>(mockList, {"1", "31", "keya", "extra", "e231"}), std::exception);     /**< Case 11 */
    EXPECT_THROW(Util::arrayFromVector<MockObject*>(mockList, {"1", "31", "keya","b", "extra"}), std::exception);     /**< Case 12 */
    for (MockObject* mock: mockList) {
        delete mock;
    }
}


/**
 * @brief TEST_F Tests Util::checkLogin()
 */
TEST_F(UtilTest, checkLogin) {

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
