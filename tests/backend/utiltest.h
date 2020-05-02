#ifndef UTILTEST_H
#define UTILTEST_H
// Other libraries
#include <../../nlohmann/json.hpp>
// GTest related includes
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mockmodelclass.h"
// Backend
#include "../../backend/util.h"

using namespace std;
using json = nlohmann::json;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::Throw;

/**
 * @brief The MockObject class Mocks an object with a toJSON method.
 */
class MockObject {
public:
    MockObject(){}
    MOCK_METHOD0(toJSON, json());
};

/**
 * @brief The UtilTest class Test fixture for Util class tests.
 */
class UtilTest : public ::testing::Test {
};

#endif // UTILTEST_H
