#include "utiltest.h"

TEST_F(UtilTest, TEST_successJSON) {
    string message = "hi";
    json j1 = {
        {"success", true},
        {"message", message}
    };
    ASSERT_EQ(json::parse(Util<ModelClass>::getSuccessJsonStr(message)), j1);

    message = "";
    json j2 = {
        {"success", true},
        {"message", message}
    };
    ASSERT_EQ(json::parse(Util<ModelClass>::getSuccessJsonStr(message)), j2);
}


TEST_F(UtilTest, TEST_failureJSON) {
    string message = "hi";
    json j1 = {
        {"success", false},
        {"message", message}
    };
    ASSERT_EQ(json::parse(Util<ModelClass>::getFailureJsonStr(message)), j1);

    message = "";
    json j2 = {
        {"success", false},
        {"message", message}
    };
    ASSERT_EQ(json::parse(Util<ModelClass>::getFailureJsonStr(message)), j2);
}
