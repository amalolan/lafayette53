#ifndef HANDLERTEST_H
#define HANDLERTEST_H
#ifdef __APPLE__
    #define CODE_BASE_DIRECTORY "../../../lafayette53/"
#elif __linux
    #define CODE_BASE_DIRECTORY "../../../lafayette53/"
#endif
#define BOOST_ASIO_HAS_STD_ATOMIC
// GTest related includes
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "mockmodelclass.h"
// Cpprestsdk
#include "cpprest/http_client.h"
#include <cpprest/filestream.h>
// Other libraries
#include <iostream>

// Backend
#include "../../backend/controller.h"
#include "../../backend/handler.h"

using json = nlohmann::json;
using namespace std;
using ::testing::Sequence;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::Throw;


/**
 * @brief The Response struct Used for returning data from a server request to parse in test fixtures.
 */
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

    /**
     * @brief HandlerTest Constructor. Creates a client that sends request and the server listener object.
     */
    HandlerTest() : client(U("http://localhost:5300/")), c(nullptr) {
        ModelClass::initdb(CODE_BASE_DIRECTORY);
        this->c =  new Controller(U("http://127.0.0.1:5300"), &this->model, CODE_BASE_DIRECTORY);
    }

    /**
     * @brief ~HandlerTest Destructor
     */
    virtual ~HandlerTest() {
        c->on_shutdown();
        delete c;
    }

    /**
     * @brief HandlerTest::make_task_request Makes a task object to be used to make an HTTP call. Helper function for requestTask()
     * @param mtd The HTTP Method Called (GET, POST, etc.)
     * @param uri the uri to send the request  to
     * @param jvalue if POST, PUT, DEL, the data as a json object to send with the request
     * @return the task which can then make the request on callback
     */
    pplx::task<http_response> make_task_request(method mtd, std::string uri, json const & jvalue);

    /**
     * @brief HandlerTest::requestTask The task object made by make_task_reqeust()
     * @param mtd The HTTP Method Called (GET, POST, etc.)
     * @param uri the uri to send the request to
     * @param jvalue if POST, PUT, DEL, the data as a json object to send with the request
     * @return a Response object,  containing the body of the response message from the server as a string,
     * the status_code object, and the content_type as a string.
     */
    Response requestTask(method mtd, std::string uri, json const & jvalue = json());

    void loginTest(string url, json data);
    void loginTest(string url);
};
#endif // HANDLERTEST_H
