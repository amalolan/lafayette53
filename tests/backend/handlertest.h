#ifndef HANDLERTEST_H
#define HANDLERTEST_H
#ifdef __APPLE__
    #define CODE_BASE_DIRECTORY "../../../lafayette53/"
#elif __linux
    #define CODE_BASE_DIRECTORY "/../lafayette53/"
#endif
#include <iostream>
#include "gtest/gtest.h"
#include "../../backend/handler.h"
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/json.h>
#include "../nlohmann/json.hpp"
#include "../../backend/ModelClassExt.h"
#include "../../backend/controller.h"

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
    Controller<ModelClassExt> c;

    HandlerTest() : client(U("http://localhost:5300/")),
                    c(U("http://127.0.0.1:5300")){
//        std::string line;
//        std::getline(std::cin, line);
//        Controller<ModelClassExt>::runServer();
    }

    virtual ~HandlerTest() {
        c.on_shutdown();
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
