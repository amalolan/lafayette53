#include "handlertest.h"
/**
 * @brief HandlerTest::make_task_request Makes a task object to be used to make an HTTP call. Helper function for requestTask()
 * @param mtd The HTTP Method Called (GET, POST, etc.)
 * @param uri the uri to send the request  to
 * @param jvalue if POST, PUT, DEL, the data as a json object to send with the request
 * @return the task which can then make the request on callback
 */
pplx::task<http_response> HandlerTest::make_task_request(method mtd, string uri, json const & jvalue)
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
Response HandlerTest::requestTask(method mtd, string uri, json const & jvalue) {
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

/**
 * @brief TEST_F Tests to see if exceptions are caught without crashing the server
 *//*
TEST_F(HandlerTest, handleError) {

}
*/

///**
// * @brief TEST_F Tests GET requests for erroneous URL inputs to ensure server doesn't break
// * Front-end handles 404 errors. Backend merely returns the same index.html file.
// * The test ensures the html file is returned.
// */
//TEST_F(HandlerTest, handleGET)  {
//    int sleeptime = 200;
//    string url;
//    Response r;
//    url = "/fae.css";
//    r = this->requestTask(methods::GET,  url);
//    ASSERT_EQ(r.status, status_codes::OK);
//    ASSERT_EQ(r.type,  "text/html");
//    ASSERT_NE(r.content,  "");
//    usleep(sleeptime);

//    url = "/fa3e.css1";
//    r = this->requestTask(methods::GET,  url);
//    ASSERT_EQ(r.status, status_codes::OK);
//    ASSERT_EQ(r.type,  "text/html");
//    ASSERT_NE(r.content,  "");
//    usleep(sleeptime);

//    url = "/fa~e.css";
//    r = this->requestTask(methods::GET,  url);
//    ASSERT_EQ(r.status, status_codes::OK);
//    ASSERT_EQ(r.type,  "text/html");
//    ASSERT_NE(r.content,  "");
//    usleep(sleeptime);

//    url = "/fae2.png";
//    r = this->requestTask(methods::GET,  url);
//    ASSERT_EQ(r.status, status_codes::OK);
//    ASSERT_EQ(r.type,  "text/html");
//    ASSERT_NE(r.content,  "");
//    usleep(sleeptime);

//    url = "/request/3e1.json";
//    r = this->requestTask(methods::GET,  url);
//    ASSERT_EQ(r.status, status_codes::OK);
//    ASSERT_EQ(r.type,  "text/html");
//    ASSERT_NE(r.content,  "");
//    usleep(sleeptime);

//    url = "/abc/def";
//    r = this->requestTask(methods::GET,  url);
//    ASSERT_EQ(r.status, status_codes::OK);
//    ASSERT_EQ(r.type,  "text/html");
//    ASSERT_NE(r.content,  "");
//    usleep(sleeptime);

//    url = "/request1/fae";
//    r = this->requestTask(methods::GET,  url);
//    ASSERT_EQ(r.status, status_codes::OK);
//    ASSERT_EQ(r.type,  "text/html");
//    ASSERT_NE(r.content,  "");
//    usleep(sleeptime);

//    url = "request1/fae.css";
//    r = this->requestTask(methods::GET,  url);
//    ASSERT_EQ(r.status, status_codes::OK);
//    ASSERT_EQ(r.type,  "text/html");
//    ASSERT_NE(r.content,  "");
//    usleep(sleeptime);

//    url = "request/museum/";
//    r = this->requestTask(methods::GET,  url);
//    ASSERT_EQ(r.status, status_codes::OK);
//    ASSERT_EQ(r.type,  "text/html");
//    ASSERT_NE(r.content,  "");
//    usleep(sleeptime);

//    url = "request/collection";
//    r = this->requestTask(methods::GET,  url);
//    ASSERT_EQ(r.status, status_codes::OK);
//    ASSERT_EQ(r.type,  "text/html");
//    ASSERT_NE(r.content,  "");
//    usleep(sleeptime);

//    url = "///";
//    r = this->requestTask(methods::GET,  url);
//    ASSERT_EQ(r.status, status_codes::OK);
//    ASSERT_EQ(r.type,  "text/html");
//    ASSERT_NE(r.content,  "");
//    usleep(sleeptime);

//    url = "/request/index.html";
//    r = this->requestTask(methods::GET,  url);
//    ASSERT_EQ(r.status, status_codes::OK);
//    ASSERT_EQ(r.type,  "text/html");
//    ASSERT_NE(r.content,  "");
//    usleep(sleeptime);
//}

///**
// * @brief TEST_F Tests POST requests for erroneous URL inputs to ensure server doesn't break
// * Server must return a 404 message, with a dumped JSON object
// * {
// *  success: false,
// *  message: [string]
// * }
// */
//TEST_F(HandlerTest, handlePOST) {
//    int sleeptime = 200;
//    string url;
//    Response r;
//    json data;
//    json user = {
//        {"user", {{"username","malo"},{"password", "123456"}}}
//    };
//    // All must 404 before trying to log user in.
//    EXPECT_CALL(this->model, getUserObject(testing::_))
//            .Times(0);

//    url = "/fae.css";
//    r = this->requestTask(methods::POST, url, user);
//    data = json::parse(r.content);
//    ASSERT_EQ(r.status, status_codes::NotFound);
//    ASSERT_EQ(data["success"],  false);
//    usleep(sleeptime);

//    url = "/";
//    r = this->requestTask(methods::POST, url, user);
//    data = json::parse(r.content);
//    ASSERT_EQ(r.status, status_codes::NotFound);
//    ASSERT_EQ(data["success"],  false);
//    usleep(sleeptime);

//    url = "/request/fae.css";
//    r = this->requestTask(methods::POST, url, user);
//    data = json::parse(r.content);
//    ASSERT_EQ(r.status, status_codes::NotFound);
//    ASSERT_EQ(data["success"],  false);
//    usleep(sleeptime);

//    url = "/request1/user-profile";
//    r = this->requestTask(methods::POST, url, user);
//    data = json::parse(r.content);
//    ASSERT_EQ(r.status, status_codes::NotFound);
//    ASSERT_EQ(data["success"],  false);
//    usleep(sleeptime);

//    url = "/request/user-profile/bsdata";
//    r = this->requestTask(methods::POST, url, user);
//    data = json::parse(r.content);
//    ASSERT_EQ(r.status, status_codes::NotFound);
//    ASSERT_EQ(data["success"],  false);
//    usleep(sleeptime);
//}

////amalolan
////password: "ba3253876aed6bc22d4a6ff53d8406c6ad864195ed144ab5c87621b6c233b548baeae6956df346ec8c17f5ea10f35ee3cbc514797ed7ddd3145464e2a0bab413"

///**
// * @brief TEST_F Tests HTTP GET request at /request/museum-list
// * Validity of  Museum JSON objects are tested in model tests.
// * 3 test cases:
// * 1. Model has an exception
// * 2. empty list
// * 3. non-empty list.
// * @see Handler::returnMuseumList() for deatils of the HTTP Message response
// */
//TEST_F(HandlerTest, returnMuseumList) {
//    int sleeptime = 200;
//    string url = "/request/museum-list";
//    Response r;
//    json expectation;
//    vector<Museum> museumList;
//    {
//        InSequence s;

//        /**< Case 1 */
//        EXPECT_CALL(this->model, getMuseumList())
//                .WillOnce(Throw(ModelException("Museum  list not working")))
//                .RetiresOnSaturation();

//        usleep(sleeptime);
//        r = this->requestTask(methods::GET,  url);
//        ASSERT_EQ(r.status, status_codes::Conflict);
//        ASSERT_FALSE(json::parse(r.content)["success"]);


//        /**< Case 2 */
//        expectation = json::array();
//        EXPECT_CALL(this->model, getMuseumList())
//                .WillOnce(Return(museumList))
//                .RetiresOnSaturation();

//        usleep(sleeptime);
//        r = this->requestTask(methods::GET,  url);
//        ASSERT_EQ(r.status, status_codes::OK);
//        ASSERT_EQ(json::parse(r.content),  expectation);


//        /**< Case Expect the json list of museums. Can use Util since it is already tested. */
//        museumList = {
//            Museum("1", "1desc", User("malo", "malo@", "k21")),
//            Museum("2", "lsf", "21", "", User("2", "", "")),
//            Museum("3", "ls2", "21432", "https://placekitten.com/200/300", User("2", "", "")),
//        };
//        expectation = Util::arrayFromVector<Museum>(museumList,
//        {"id", "name", "description", "introduction", "userID", "image"});

//        EXPECT_CALL(this->model, getMuseumList())
//                .WillOnce(Return(museumList))
//                .RetiresOnSaturation();

//        usleep(sleeptime);
//        r = this->requestTask(methods::GET,  url);
//        ASSERT_EQ(r.status, status_codes::OK);
//        ASSERT_EQ(json::parse(r.content),  expectation);
//    }
//}

///**
// * @brief TEST_F Tests HTTP GET request at /request/museum/[id]
// * No ID is tested in handleGET() tests. Validity of collection JSON objects are tested in model tests.
// * 4 Test Cases:
// * 1. Invalid IDs (string and greater than INT_MAX)
// * 2. Valid ID, Not found in DB
// * 3. Valid ID, Found, Valid Museum Object, No Collections
// * 4. Valid ID, Found, Valid Museum Object, Collections present.
// */
//TEST_F(HandlerTest, returnMuseumByID) {
//    int sleeptime = 200;
//    string url = "/request/museum/";  // /request/museum/[id]
//    string id;
//    Response r;
//    json expectation;

//    /**< Case 1 */
//    id = "AB1";
//    usleep(sleeptime);
//    r = this->requestTask(methods::GET,  url + id);
//    ASSERT_EQ(r.status, status_codes::InternalError);
//    ASSERT_FALSE(json::parse(r.content)["success"]);

//    id = "91111111111111111111111111111111111111111111111";
//    usleep(sleeptime);
//    r = this->requestTask(methods::GET,  url + id);
//    ASSERT_EQ(r.status, status_codes::InternalError);
//    ASSERT_FALSE(json::parse(r.content)["success"]);
//    {
//        InSequence s;
//        /**< Case 2 */
//        id = to_string(INT_MAX);
//        EXPECT_CALL(this->model, getMuseumObject(stoi(id)))
//                .WillOnce(testing::Throw(ModelException("Can't find Museum")))
//                .RetiresOnSaturation();

//        usleep(sleeptime);
//        r = this->requestTask(methods::GET,  url + id);
//        ASSERT_EQ(r.status, status_codes::Conflict);
//        ASSERT_FALSE(json::parse(r.content)["success"]);

//        Museum museum("name", "description", "introduction",  "image",
//                      User("username", "email", "password", 1), stoi(id));


//        /**< Case 3 */
//        vector<Collection> collectionList;
//        expectation =  {
//            {"museum", museum.toJSON()},
//            {"collectionList", json::array()}
//        };
//        EXPECT_CALL(this->model, getMuseumObject(stoi(id)))
//                .WillOnce(Return(museum))
//                .RetiresOnSaturation();
//        EXPECT_CALL(this->model, getCollectionListByMuseumID(stoi(id)))
//                .WillOnce(Return(collectionList))
//                .RetiresOnSaturation();

//        usleep(sleeptime);
//        r = this->requestTask(methods::GET, url + id);
//        ASSERT_EQ(r.status, status_codes::OK);
//        ASSERT_EQ(json::parse(r.content),  expectation);


//        /**< Case 4 */
//        collectionList.push_back(Collection("collectionName", "collectionDescription",
//                                            "collectionIntroduction", "collectionImage",
//                                            museum, 1));
//        collectionList.push_back(Collection("collectionName", "collectionDescription",
//                                            "collectionIntroduction", "collectionImage",
//                                            museum, 2));
//        expectation["collectionList"]  = Util::arrayFromVector<Collection>(collectionList,
//        {"id",  "name", "description", "introduction", "image"});
//        EXPECT_CALL(this->model, getMuseumObject(stoi(id)))
//                .WillOnce(Return(museum))
//                .RetiresOnSaturation();
//        EXPECT_CALL(this->model, getCollectionListByMuseumID(stoi(id)))
//                .WillOnce(Return(collectionList))
//                .RetiresOnSaturation();

//        usleep(sleeptime);
//        r = this->requestTask(methods::GET, url + id);
//        ASSERT_EQ(r.status, status_codes::OK);
//        ASSERT_EQ(json::parse(r.content),  expectation);
//    }
//}

///**
// * @brief TEST_F Tests HTTP GET request at /request/collection/[id]
// * No ID is tested in handleGET() tests. Validity of collection JSON objects are tested in model tests.
// * 4 Test Cases:
// * 1. Invalid IDs (string and greater than INT_MAX)
// * 2. Valid ID, Not found in DB
// * 3. Valid ID, Found, Valid Collection Object, No Artifacts
// * 4. Valid ID, Found, Valid Collections Object, Artifacts present.
// */
//TEST_F(HandlerTest, returnCollectionByID) {
//    int sleeptime = 500;
//    string url = "/request/collection/";  // /request/collection/[id]
//    string id;
//    Response r;
//    json expectation;

//    /**< Case 1 */
//    id = "AB1";
//    usleep(sleeptime);
//    r = this->requestTask(methods::GET,  url + id);
//    ASSERT_EQ(r.status, status_codes::InternalError);
//    ASSERT_FALSE(json::parse(r.content)["success"]);

//    id = "91111111111111111111111111111111111111111111111";
//    usleep(sleeptime);
//    r = this->requestTask(methods::GET,  url + id);
//    ASSERT_EQ(r.status, status_codes::InternalError);
//    ASSERT_FALSE(json::parse(r.content)["success"]);
//    {
//        InSequence s;
//        /**< Case 2 */
//        id = to_string(INT_MAX);
//        EXPECT_CALL(this->model, getCollectionObject(stoi(id)))
//                .WillOnce(testing::Throw(ModelException("Can't find Collection")))
//                .RetiresOnSaturation();

//        usleep(sleeptime);
//        r = this->requestTask(methods::GET,  url + id);
//        ASSERT_EQ(r.status, status_codes::Conflict);
//        ASSERT_FALSE(json::parse(r.content)["success"]);

//        Museum museum("name", "description", "introduction",  "image",
//                      User("username", "email", "password", 1), 2);
//        Collection collection("collectionName", "collectionDescription",
//                              "collectionIntroduction", "collectionImage",
//                              museum, stoi(id));

//        /**< Case 3 */
//        vector<Artifact> artifactList;
//        expectation =  {
//            {"collection", Util::getObjectWithKeys<Collection>(collection,
//             {"id", "name", "description", "introduction", "image"})},
//            {"artifactList", json::array()},
//            {"museum", Util::getObjectWithKeys<Museum>(collection.getMuseum(),{"id","name"})}
//        };
//        EXPECT_CALL(this->model, getCollectionObject(stoi(id)))
//                .WillOnce(Return(collection))
//                .RetiresOnSaturation();
//        EXPECT_CALL(this->model, getArtifactsByCollection(stoi(id)))
//                .WillOnce(Return(artifactList))
//                .RetiresOnSaturation();

//        usleep(sleeptime);
//        r = this->requestTask(methods::GET, url + id);
//        ASSERT_EQ(r.status, status_codes::OK);
//        ASSERT_EQ(json::parse(r.content),  expectation);


//        /**< Case 4 */
//        artifactList.push_back(Artifact("artifactName", "artifactDescription",
//                                        "artifactIntroduction", "artifactImage",
//                                        museum, 3));
//        artifactList.push_back(Artifact("artifactName", "artifactDescription",
//                                        "artifactIntroduction", "artifactImage",
//                                        museum, 2));
//        expectation["artifactList"]  = Util::arrayFromVector<Artifact>(artifactList,
//        {"id",  "name", "description", "introduction", "image"});
//        EXPECT_CALL(this->model, getCollectionObject(stoi(id)))
//                .WillOnce(Return(collection))
//                .RetiresOnSaturation();
//        EXPECT_CALL(this->model, getArtifactsByCollection(stoi(id)))
//                .WillOnce(Return(artifactList))
//                .RetiresOnSaturation();

//        usleep(sleeptime);
//        r = this->requestTask(methods::GET, url + id);
//        ASSERT_EQ(r.status, status_codes::OK);
//        ASSERT_EQ(json::parse(r.content),  expectation);
//    }
//}

///**
// * @brief TEST_F Tests HTTP GET request at /request/artifact/[id]
// * No ID is tested in handleGET() tests. Validity of collection JSON objects are tested in model tests.
// * 4 Test Cases:
// * 1. Invalid IDs (string and greater than INT_MAX)
// * 2. Valid ID, Not found in DB
// * 3. Valid ID, Found, Valid Artifact Object, Single Collection Association
// * 4. Valid ID, Found, Valid Artifact Object, Multiple Collection Associations.
// */
//TEST_F(HandlerTest, returnArtifactByID) {
//    int sleeptime = 200;
//    string url = "/request/artifact/";  // /request/artifact/[id]
//    string id;
//    Response r;
//    json expectation;

//    /**< Case 1 */
//    id = "AB1";
//    usleep(sleeptime);
//    r = this->requestTask(methods::GET,  url + id);
//    ASSERT_EQ(r.status, status_codes::InternalError);
//    ASSERT_FALSE(json::parse(r.content)["success"]);

//    id = "91111111111111111111111111111111111111111111111";
//    usleep(sleeptime);
//    r = this->requestTask(methods::GET,  url + id);
//    ASSERT_EQ(r.status, status_codes::InternalError);
//    ASSERT_FALSE(json::parse(r.content)["success"]);
//    {
//        InSequence s;
//        /**< Case 2 */
//        id = to_string(INT_MAX);
//        EXPECT_CALL(this->model, getArtifact(stoi(id)))
//                .WillOnce(testing::Throw(ModelException("Can't find Artifact")))
//                .RetiresOnSaturation();

//        usleep(sleeptime);
//        r = this->requestTask(methods::GET,  url + id);
//        ASSERT_EQ(r.status, status_codes::Conflict);
//        ASSERT_FALSE(json::parse(r.content)["success"]);

//        Museum museum("name", "description", "introduction",  "image",
//                      User("username", "email", "password", 1), 2);
//        Artifact artifact("artifactName", "artifactDescription",
//                          "artifactIntroduction", "artifactImage",
//                          museum, stoi(id));

//        /**< Case 3 */
//        vector<Collection> collectionList;
//        expectation =  {
//            {"artifact", Util::getObjectWithKeys<Artifact>(artifact,
//             {"id", "name", "description", "introduction", "image"})},
//            {"collectionList", json::array()},
//            {"museum", Util::getObjectWithKeys<Museum>(artifact.getMuseum(),{"id"})}
//        };
//        collectionList.push_back(Collection("1collectionName", "1collectionDescription",
//                                            "1collectionIntroduction", "1collectionImage",
//                                            museum, 12211));
//        expectation["collectionList"]  = Util::arrayFromVector<Collection>(collectionList,
//        {"id",  "name"});
//        EXPECT_CALL(this->model, getArtifact(stoi(id)))
//                .WillOnce(Return(artifact))
//                .RetiresOnSaturation();
//        EXPECT_CALL(this->model, getCollectionsByArtifact(stoi(id)))
//                .WillOnce(Return(collectionList))
//                .RetiresOnSaturation();

//        usleep(sleeptime);
//        r = this->requestTask(methods::GET, url + id);
//        ASSERT_EQ(r.status, status_codes::OK);
//        ASSERT_EQ(json::parse(r.content),  expectation);


//        /**< Case 4 */
//        collectionList.push_back(Collection("2collectionName", "2collectionDescription",
//                                   "2collectionIntroduction", "2collectionImage",
//                                    museum, 321));
//        collectionList.push_back(Collection("3collectionName", "3collectionDescription",
//                                            "3collectionIntroduction", "3collectionImage",
//                                            museum, 3211));

//        expectation["collectionList"]  = Util::arrayFromVector<Collection>(collectionList,
//        {"id",  "name"});
//        EXPECT_CALL(this->model, getArtifact(stoi(id)))
//                .WillOnce(Return(artifact))
//                .RetiresOnSaturation();
//        EXPECT_CALL(this->model, getCollectionsByArtifact(stoi(id)))
//                .WillOnce(Return(collectionList))
//                .RetiresOnSaturation();

//        usleep(sleeptime);
//        r = this->requestTask(methods::GET, url + id);
//        ASSERT_EQ(r.status, status_codes::OK);
//        ASSERT_EQ(json::parse(r.content),  expectation);
//    }
//}

///**
// * @brief HandlerTest::loginTest Helper test function to ensure log in actually works.
// * Called by many POST request test functions.
// * Validity of collection JSON objects are tested in model tests.
// * Validity of Username, Password asserted on front-end.
// * 4 Test Cases:
// * 1. Invalid POST data (no username & password).
// * 2. Valid data, Username not found in DB.
// * 3. Valid data, Username Found, Password mismatch.
// * 4. Valid data, Username Found, Password match.
// * @param url The POST url to perform the login test on.
// */
//void HandlerTest::loginTest(string url) {
//    int sleeptime = 200;
//    string username;
//    Response r;
//    json data;
//    json expectation;

//    {
//        InSequence s;
//        /**< Case 1 */
//        data = {
//            {"username",  "anfann12"   },
//            {"passwor12d", "password~*?"}
//        };
//        usleep(sleeptime);
//        r = this->requestTask(methods::POST, url, data);
//        ASSERT_EQ(r.status, status_codes::InternalError);
//        ASSERT_FALSE(json::parse(r.content)["success"]);

//        /**< Case 2 */
//        data = {
//            {"username",  "anfann12"},
//            {"password", "password~*?"}
//        };
//        User user(data["username"],"email@example.com", "realpassword", 12);
//        /**< Case 1 */
//        EXPECT_CALL(this->model, getUserObject((string) data["username"]))
//                .WillOnce(testing::Throw(ModelException("Can't find User")))
//                .RetiresOnSaturation();

//        usleep(sleeptime);
//        r = this->requestTask(methods::POST, url, data);
//        ASSERT_EQ(r.status, status_codes::Conflict);
//        ASSERT_FALSE(json::parse(r.content)["success"]);

//        /**< Case 3 */
//        EXPECT_CALL(this->model, getUserObject((string) data["username"]))
//                .WillOnce(Return(user))
//                .RetiresOnSaturation();

//        usleep(sleeptime);
//        r = this->requestTask(methods::POST, url, data);
//        usleep(sleeptime);
//        ASSERT_EQ(r.status, status_codes::Unauthorized);
//        ASSERT_FALSE(json::parse(r.content)["success"]);

//        /**< Case 4 */
//        data["password"] = "realpassword";
//        EXPECT_CALL(this->model, getUserObject((string) data["username"]))
//                .Times(testing::AtLeast(1))
//                .WillOnce(Return(user))
//                .RetiresOnSaturation();

//        usleep(sleeptime);
//        r = this->requestTask(methods::POST, url, data);
//        ASSERT_EQ(r.status, status_codes::OK);
//        ASSERT_TRUE(json::parse(r.content)["success"]);
//    }
//}


///**
// * @brief TEST_F Tests HTTP POST request at /request/login
// * @see HandlerTest::loginTest()
// * Calls the login helper function loginTest()
// */
//TEST_F(HandlerTest, validateLogin) {
//    this->loginTest("/request/login");
//}

////TEST_F(HandlerTest, addUser) {
////    EXPECT_CALL(this->model, saveUserToDB(testing::_));
////}


///**
// * @brief TEST_F Tests HTTP POST request at /request/add-museum/
// * @see HandlerTest::loginTest()
// * Needs login. Tested by helper function loginTest()
// *
// */
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
