#include "handlertest.h"

pplx::task<http_response> HandlerTest::make_task_request(
   method mtd,
   std::string uri,
   json::value const & jvalue)
{
    uri_builder builder(U(uri));
   return (mtd == methods::GET || mtd == methods::HEAD) ?
      this->client.request(mtd, builder.to_string()) :
      this->client.request(mtd, builder.to_string(), jvalue);
}

std::string HandlerTest::requestTask(method mtd,
                   std::string uri,
                   json::value const & jvalue) {
    pplx::task<string_t> requestTask = this->make_task_request(mtd, uri, jvalue)
            .then([=](http_response response) {
        std::cout<<"Response Code: " <<response.status_code()<<std::endl;
        return response.extract_string();
    });
    try {
        requestTask.wait();
        return utility::conversions::to_string_t(requestTask.get());
    } catch (const std::exception &e) {
        std::cerr<<  "Error exception: " << e.what() << std::endl;
        throw e;
    }
    return "";
}

TEST_F(HandlerTest, TEST_SERVER_RUNNING) {
    try {
         std::string result = this->requestTask(methods::GET, "");
         std::cout<<result;
    } catch (const std::exception &e){
        FAIL() <<  e.what();
    }
}

TEST_F(HandlerTest, TEST_GET)  {
//    auto putvalue = json::value::object();
//    putvalue[L"username"] = json::value::string(L"amalolan");
//    putvalue[L"email"] = json::value::string(L"amalolan@gmail.com");
//    putvalue[L"password"] = json::value::string(L"iamgenius");


//   std::string result = this->requestTask(methods::POST, "/get-data/user", putvalue);
   std::string result = this->requestTask(methods::GET, "/get-data/museum-list");
   std::cout<<result;
   ASSERT_NE(result, "");
}
