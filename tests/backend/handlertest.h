#ifndef HANDLERTEST_H
#define HANDLERTEST_H
#include <iostream>
#include "gtest/gtest.h"
#include "../../backend/handler.h"
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/json.h>
#include "../nlohmann/json.hpp"
using json = nlohmann::json;
//using namespace utility;                    // Common utilities like string conversions
//using namespace web;                        // Common features like URIs.
//using namespace web::http;                  // Common HTTP functionality
//using namespace web::http::client;          // HTTP client features
//using namespace concurrency::streams;

/**
 * @brief The HandlerTest class The fixture for testing class Handler
 */
class HandlerTest : public ::testing::Test {
protected:
    http_client client;

    HandlerTest() : client(U("http://localhost:5300/")){

    }

    virtual ~HandlerTest() {
    // You can do clean-up work that doesn't throw exceptions here.
    }


    virtual void SetUp() {
    // Code here will be called immediately after the constructor (right
    // before each test).
    }

    virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
    }

    pplx::task<http_response> make_task_request(method mtd, std::string uri, json const & jvalue);
    std::string requestTask(method mtd, std::string uri, json const & jvalue = json());

};
#endif // HANDLERTEST_H
