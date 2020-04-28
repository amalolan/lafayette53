#include "utiltest.h"

TEST_F(UtilTest, successJSON) {
    string message = "hi";
    json j1 = {
        {"success", true},
        {"message", message}
    };
    ASSERT_EQ(json::parse(Util::getSuccessJsonStr(message)), j1);

    message = "";
    json j2 = {
        {"success", true},
        {"message", message}
    };
    ASSERT_EQ(json::parse(Util::getSuccessJsonStr(message)), j2);
}


TEST_F(UtilTest, failureJSON) {
    string message = "hi";
    json j1 = {
        {"success", false},
        {"message", message}
    };
    ASSERT_EQ(json::parse(Util::getFailureJsonStr(message)), j1);

    message = "";
    json j2 = {
        {"success", false},
        {"message", message}
    };
    ASSERT_EQ(json::parse(Util::getFailureJsonStr(message)), j2);
}
