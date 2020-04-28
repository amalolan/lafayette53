#include "handlertest.h"
/**
 * @brief HandlerTest::make_task_request Makes a task object to be used to make an HTTP call. Helper function for requestTask()
 * @param mtd The HTTP Method Called (GET, POST, etc.)
 * @param uri the uri to send the request  to
 * @param jvalue if POST, PUT, DEL, the data as a json object to send with the request
 * @return the task which can then make the request on callback
 */
pplx::task<http_response> HandlerTest::make_task_request(
   method mtd,
   string uri,
   json const & jvalue)
{
    uri_builder builder(U(uri));    
   return (mtd == methods::GET || mtd == methods::HEAD) ?
      this->client.request(mtd, builder.to_string()) :
      this->client.request(mtd, builder.to_string(), jvalue.dump());
}

/**
 * @brief HandlerTest::requestTask The task object made by make_task_reqeust()
 * @param mtd The HTTP Method Called (GET, POST, etc.)
 * @param uri the uri to send the request to
 * @param jvalue if POST, PUT, DEL, the data as a json object to send with the request
 * @return the body of the response message from the server as a string
 */
string HandlerTest::requestTask(method mtd,
                   string uri,
                   json const & jvalue) {
    pplx::task<string_t> requestTask = this->make_task_request(mtd, uri, jvalue)
            .then([=](http_response response) {
        cout<<"Response Code: " <<response.status_code()<<endl;
        return response.extract_string();
    });
    try {
        requestTask.wait();
        return utility::conversions::to_string_t(requestTask.get());
    } catch (const std::exception &e) {
        cerr<<  "Error exception in test requestTask: " << e.what() << endl;
        throw e;
    }
    return "";
}

/**
 * Returns the content type instead of the response's body.
 *
 * @copydoc HandlerTest::requestTask()
 */
string HandlerTest::requestTaskContentType(method mtd, string uri, const json &jvalue) {
    pplx::task<string_t> requestTask = this->make_task_request(mtd, uri, jvalue)
            .then([=](http_response response) {
        cout<<"Response Code: " <<response.status_code()<<endl;
        return  response.headers().content_type();
    });
    try {
        requestTask.wait();
        return utility::conversions::to_string_t(requestTask.get());
    } catch (const std::exception &e) {
        cerr<<  "Error exception in test requestTask " << e.what() << endl;
        throw e;
    }
    return "";
}

/**
 * @brief TEST_F Checks if the server has started running.
 */
TEST_F(HandlerTest, serverRunning) {
     string result = this->requestTaskContentType(methods::GET, "/");
     ASSERT_EQ(result, "text/html");
     usleep(200);
     result = this->requestTask(methods::GET,"/");
     ASSERT_NE(result, "");

}

///**
// * @brief TEST_F Tests GET requests for erroneous URL inputs to ensure server doesn't break
// * Front-end handles 404 errors. Backend merely returns the same index.html file.
// * The test ensures the html file is returned.
// */
TEST_F(HandlerTest, handle_get)  {
   string result, url;
   url = "/fae.css";
   result = this->requestTaskContentType(methods::GET, url);
   ASSERT_EQ(result, "text/html");
   usleep(200);
   result = this->requestTask(methods::GET, url);
   ASSERT_NE(result, "");
   usleep(200);

   url = "/fa3e.css1";
   result = this->requestTaskContentType(methods::GET, url);
   ASSERT_EQ(result, "text/html");
   usleep(200);
   result = this->requestTask(methods::GET, url);
   ASSERT_NE(result, "");
   usleep(200);

   url = "/fa~e.css";
   result = this->requestTaskContentType(methods::GET, url);
   ASSERT_EQ(result, "text/html");
   usleep(200);
   result = this->requestTask(methods::GET, url);
   ASSERT_NE(result, "");
   usleep(200);

   url = "/fae2.png";
   result = this->requestTaskContentType(methods::GET, url);
   ASSERT_EQ(result, "text/html");
   usleep(200);
   result = this->requestTask(methods::GET, url);
   ASSERT_NE(result, "");
   usleep(200);

   url = "/request/3e1.json";
   result = this->requestTaskContentType(methods::GET, url);
   ASSERT_EQ(result, "text/html");
   usleep(200);
   result = this->requestTask(methods::GET, url);
   ASSERT_NE(result, "");
   usleep(200);

   url = "/abc/def";
   result = this->requestTaskContentType(methods::GET, url);
   ASSERT_EQ(result, "text/html");
   usleep(200);
   result = this->requestTask(methods::GET, url);
   ASSERT_NE(result, "");
   usleep(200);

   url = "/request1/fae";
   result = this->requestTaskContentType(methods::GET, url);
   ASSERT_EQ(result, "text/html");
   usleep(200);
   result = this->requestTask(methods::GET, url);
   ASSERT_NE(result, "");
   usleep(200);

   url = "request1/fae.css";
   result = this->requestTaskContentType(methods::GET, url);
   ASSERT_EQ(result, "text/html");
   usleep(200);
   result = this->requestTask(methods::GET, url);
   ASSERT_NE(result, "");
   usleep(200);

   url = "///";
   result = this->requestTaskContentType(methods::GET, url);
   ASSERT_EQ(result, "text/html");
   usleep(200);
   result = this->requestTask(methods::GET, url);
   ASSERT_NE(result, "");
   usleep(200);

   url = "/request/index.html";
   result = this->requestTaskContentType(methods::GET, url);
   ASSERT_EQ(result, "text/html");
   usleep(200);
   result = this->requestTask(methods::GET, url);
   ASSERT_NE(result, "");
}


//TEST_F(HandlerTest, returnMuseumList) {

//}

//TEST_F(HandlerTest, returnMuseumById) {}

//TEST_F(HandlerTest, returnCollectionById) {}

//TEST_F(HandlerTest, returnArtifactById) {}

//TEST_F(HandlerTest, validateLogin) {}

//TEST_F(HandlerTest, getUserProfile) {}

//TEST_F(HandlerTest, handle_post) {}

//TEST_F(HandlerTest, addMuseum) {}

//TEST_F(HandlerTest, addUser) {}

//TEST_F(HandlerTest, addCollection) {}

//TEST_F(HandlerTest, addArtifact) {}


////TEST_F(HandlerTest, user) {
////    json user =
////    {
////        {"username",  "malolan12345"},
////        {"password", "123"},
////        {"email",  "malo@gmail.com324332"}
////    };
////    User obj("malolan12345", user["email"],user["password"]);
////    EXPECT_CALL(this->model, saveUserToDB(obj));
////    string result = this->requestTask(methods::POST, "/request/register", user);
////    cout<<result;
////}
