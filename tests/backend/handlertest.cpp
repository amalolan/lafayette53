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
 * @return a Response object,  containing the body of the response message from the server as a string,
 * the status_code object, and the content_type as a string.
 */
Response HandlerTest::requestTask(method mtd,
                                 string uri,
                                 json const & jvalue) {
    Response r;
    pplx::task<web::http::http_response> requestTask = this->make_task_request(mtd, uri, jvalue)
            .then([=](http_response response) {
        //        cout<<"Response Code: " <<response.status_code()<<endl;
        return response.content_ready();
    });
    try {
        requestTask.wait();
        r.status =  requestTask.get().status_code();
        r.type  =  requestTask.get().headers().content_type();
        pplx::task<string_t> stringTask =  requestTask.get().extract_string();
        stringTask.wait();
        r.content =  utility::conversions::to_string_t(stringTask.get());
    } catch (const std::exception &e) {
        cerr<<  "Error exception in test requestTask: " << e.what() << endl;
        throw e;
    }
    return r;
}

/**
 * @brief TEST_F Checks if the server has started running.
 */
TEST_F(HandlerTest, serverRunning) {
    Response r = this->requestTask(methods::GET,  "/");
    ASSERT_EQ(r.status, status_codes::OK);
    ASSERT_EQ(r.type,  "text/html");
    ASSERT_NE(r.content,  "");
}


//TEST_F(HandlerTest, handleError) {}

///**
// * @brief TEST_F Tests GET requests for erroneous URL inputs to ensure server doesn't break
// * Front-end handles 404 errors. Backend merely returns the same index.html file.
// * The test ensures the html file is returned.
// */
TEST_F(HandlerTest, handleGET)  {
    int sleeptime = 200;
    string url;
    Response r;
    url = "/fae.css";
    r = this->requestTask(methods::GET,  url);
    ASSERT_EQ(r.status, status_codes::OK);
    ASSERT_EQ(r.type,  "text/html");
    ASSERT_NE(r.content,  "");
    usleep(sleeptime);

    url = "/fa3e.css1";
    r = this->requestTask(methods::GET,  url);
    ASSERT_EQ(r.status, status_codes::OK);
    ASSERT_EQ(r.type,  "text/html");
    ASSERT_NE(r.content,  "");
    usleep(sleeptime);

    url = "/fa~e.css";
    r = this->requestTask(methods::GET,  url);
    ASSERT_EQ(r.status, status_codes::OK);
    ASSERT_EQ(r.type,  "text/html");
    ASSERT_NE(r.content,  "");
    usleep(sleeptime);

    url = "/fae2.png";
    r = this->requestTask(methods::GET,  url);
    ASSERT_EQ(r.status, status_codes::OK);
    ASSERT_EQ(r.type,  "text/html");
    ASSERT_NE(r.content,  "");
    usleep(sleeptime);

    url = "/request/3e1.json";
    r = this->requestTask(methods::GET,  url);
    ASSERT_EQ(r.status, status_codes::OK);
    ASSERT_EQ(r.type,  "text/html");
    ASSERT_NE(r.content,  "");
    usleep(sleeptime);

    url = "/abc/def";
    r = this->requestTask(methods::GET,  url);
    ASSERT_EQ(r.status, status_codes::OK);
    ASSERT_EQ(r.type,  "text/html");
    ASSERT_NE(r.content,  "");
    usleep(sleeptime);

    url = "/request1/fae";
    r = this->requestTask(methods::GET,  url);
    ASSERT_EQ(r.status, status_codes::OK);
    ASSERT_EQ(r.type,  "text/html");
    ASSERT_NE(r.content,  "");
    usleep(sleeptime);

    url = "request1/fae.css";
    r = this->requestTask(methods::GET,  url);
    ASSERT_EQ(r.status, status_codes::OK);
    ASSERT_EQ(r.type,  "text/html");
    ASSERT_NE(r.content,  "");
    usleep(sleeptime);

    url = "///";
    r = this->requestTask(methods::GET,  url);
    ASSERT_EQ(r.status, status_codes::OK);
    ASSERT_EQ(r.type,  "text/html");
    ASSERT_NE(r.content,  "");
    usleep(sleeptime);

    url = "/request/index.html";
    r = this->requestTask(methods::GET,  url);
    ASSERT_EQ(r.status, status_codes::OK);
    ASSERT_EQ(r.type,  "text/html");
    ASSERT_NE(r.content,  "");
    usleep(sleeptime);
}

/**
 * @brief TEST_F Tests POST requests for erroneous URL inputs to ensure server doesn't break
 * Server must return a 404 message
 */
TEST_F(HandlerTest, handlePOST) {
    int sleeptime = 200;
    string url;
    Response r;
    json data;
    json user = {
        {"user", {{"username","malo"},{"password", "123456"}}}
    };
    // All must 404 before trying to log user in.
    EXPECT_CALL(this->model, getUserObject(testing::_))
            .Times(0);

    url = "/fae.css";
    r = this->requestTask(methods::POST, url, user);
    data = json::parse(r.content);
    ASSERT_EQ(r.status, status_codes::NotFound);
    ASSERT_EQ(data["success"],  false);
    usleep(sleeptime);

    url = "/";
    r = this->requestTask(methods::POST, url, user);
    data = json::parse(r.content);
    ASSERT_EQ(r.status, status_codes::NotFound);
    ASSERT_EQ(data["success"],  false);
    usleep(sleeptime);

    url = "/request/fae.css";
    r = this->requestTask(methods::POST, url, user);
    data = json::parse(r.content);
    ASSERT_EQ(r.status, status_codes::NotFound);
    ASSERT_EQ(data["success"],  false);
    usleep(sleeptime);

    url = "/request1/user-profile";
    r = this->requestTask(methods::POST, url, user);
    data = json::parse(r.content);
    ASSERT_EQ(r.status, status_codes::NotFound);
    ASSERT_EQ(data["success"],  false);
    usleep(sleeptime);

    url = "/request/user-profile/bsdata";
    r = this->requestTask(methods::POST, url, user);
    data = json::parse(r.content);
    ASSERT_EQ(r.status, status_codes::NotFound);
    ASSERT_EQ(data["success"],  false);
    usleep(sleeptime);
}

//amalolan
//password: "ba3253876aed6bc22d4a6ff53d8406c6ad864195ed144ab5c87621b6c233b548baeae6956df346ec8c17f5ea10f35ee3cbc514797ed7ddd3145464e2a0bab413"

//TEST_F(HandlerTest, returnMuseumList) {
//    EXPECT_CALL(this->model, getMuseumList());
//}

//TEST_F(HandlerTest, returnMuseumById) {
//    EXPECT_CALL(this->model, getMuseumObject(testing::_));
//    EXPECT_CALL(this->model, getCollectionListByMuseumID(testing::_));
//    EXPECT_CALL(this->model, getArtifactsByMuseum(testing::_));
//}

//TEST_F(HandlerTest, returnCollectionById) {
//    EXPECT_CALL(this->model, getCollectionObject(testing::_));
//    EXPECT_CALL(this->model, getArtifactsByCollection(testing::_));
//}

//TEST_F(HandlerTest, returnArtifactById) {
//    EXPECT_CALL(this->model, getArtifact(testing::_));
//    EXPECT_CALL(this->model, getCollectionsByArtifact(testing::_));
//}


//TEST_F(HandlerTest, validateLogin) {
//    EXPECT_CALL(this->model, getUserObject(testing::_));
//}

//TEST_F(HandlerTest, addUser) {
//    EXPECT_CALL(this->model, saveUserToDB(testing::_));
//}



//TEST_F(HandlerTest, addMuseum) {
//    EXPECT_CALL(this->model, getUserObject(testing::_));
//    EXPECT_CALL(this->model, getUserObject(testing::_));
//    EXPECT_CALL(this->model, saveMuseumToDB(testing::_));
//}


//TEST_F(HandlerTest, addCollection) {
//    EXPECT_CALL(this->model, getUserObject(testing::_));
//    EXPECT_CALL(this->model, getMuseumObject(testing::_));
//    // if  curator
//    EXPECT_CALL(this->model, saveCollectionToDB(testing::_));
//    // else
//}

//TEST_F(HandlerTest, addArtifact) {
//    EXPECT_CALL(this->model, getUserObject(testing::_));
//    EXPECT_CALL(this->model, getMuseumObject(testing::_));
//    // if curator
//    EXPECT_CALL(this->model, saveArtifactToDB(testing::_));
//    // for every collection
//    EXPECT_CALL(this->model, getCollectionObject(testing::_));
//    EXPECT_CALL(this->model, addArtifactCollection(testing::_, testing::_));
//}

//TEST_F(HandlerTest, getUserProfile) {
//    EXPECT_CALL(this->model, getUserObject(testing::_));
//    EXPECT_CALL(this->model, getUserObject(testing::_));
//}

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
