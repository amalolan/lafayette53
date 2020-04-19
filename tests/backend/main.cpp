#include <iostream>
#include "gtest/gtest.h"
#include "../../backend/handler.h"

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/json.h>
//using namespace std;
using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;


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

    // Objects declared here can be used by all tests in the test case for Foo.

    /** replace with your own **/
    Handler h1;
    Handler h2;

    pplx::task<http_response> make_task_request(
       method mtd,
       std::string uri,
       json::value const & jvalue)
    {
        uri_builder builder(U(uri));
       return (mtd == methods::GET || mtd == methods::HEAD) ?
          this->client.request(mtd, builder.to_string()) :
          this->client.request(mtd, builder.to_string(), jvalue);
    }


    std::string requestTask(method mtd,
                       std::string uri,
                       json::value const & jvalue = json::value::null()) {
        pplx::task<string_t> requestTask = this->make_task_request(mtd, uri, jvalue)
                .then([=](http_response response) {
            return response.extract_string();
        });
        try {
            requestTask.wait();
            return utility::conversions::to_string_t(requestTask.get());
        } catch (const std::exception &e) {
            printf("Error exception:%s\n", e.what());
        }
        return "";
    }
};


TEST_F(HandlerTest, TEST_GET)  {
    auto putvalue = json::value::object();
//    putvalue[L"username"] = json::value::string(L"amalolan");
//    putvalue[L"email"] = json::value::string(L"amalolan@gmail.com");
//    putvalue[L"password"] = json::value::string(L"iamgenius");


   std::string result = this->requestTask(methods::POST, "/get-data/user", putvalue);
   std::cout<<result;
   ASSERT_NE(result, "");
}



//using namespace std;
int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
