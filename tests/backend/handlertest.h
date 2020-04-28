#ifndef HANDLERTEST_H
#define HANDLERTEST_H
#ifdef __APPLE__
    #define CODE_BASE_DIRECTORY "../../../lafayette53/"
#elif __linux
    #define CODE_BASE_DIRECTORY "/../../../lafayette53/"
#endif
#define BOOST_ASIO_HAS_STD_ATOMIC

#include <iostream>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/json.h>
#include "../nlohmann/json.hpp"
#include "../../backend/modelclassext.h"
#include "../../backend/controller.h"
#include "../../backend/handler.h"

#include "mockmodelclass.h"
using json = nlohmann::json;
using namespace std;

struct Response {
    status_code status;
    string type;
    string content;
};


/**
 * @brief The HandlerTest class The fixture for testing class Handler
 */
class HandlerTest : public ::testing::Test {
protected:
    http_client client;
    Controller* c;
    MockModelClass model;

    HandlerTest() : client(U("http://localhost:5300/")), c(nullptr)
                    {
        ModelClass::initdb(CODE_BASE_DIRECTORY);
//        ModelClass *model =  ModelClass::getInstance(ModelClass::pro);
//        ModelClassExt *model =  ModelClassExt::getInstance(CODE_BASE_DIRECTORY);
//        model->createTables();
        this->c =  new Controller(U("http://127.0.0.1:5300"), &this->model, CODE_BASE_DIRECTORY);
    }

    virtual ~HandlerTest() {
        c->on_shutdown();
        delete c;
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
    Response requestTask(method mtd, std::string uri, json const & jvalue = json());

};
#endif // HANDLERTEST_H
