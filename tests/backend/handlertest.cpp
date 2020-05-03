#include "handlertest.h"

pplx::task<http_response> HandlerTest::make_task_request(method mtd, string uri, json const & jvalue)
{
    uri_builder builder(U(uri));
    return (mtd == methods::GET || mtd == methods::HEAD) ?
                this->client.request(mtd, builder.to_string()) :
                this->client.request(mtd, builder.to_string(), jvalue.dump());
}

Response HandlerTest::requestTask(method mtd, string uri, json const & jvalue) {
    Response r;
    pplx::task<web::http::http_response> requestTask = this->make_task_request(mtd, uri, jvalue)
            .then([=](http_response response) {
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
 * @brief TEST_F Tests DEL and PUT requests for erroneous URL inputs to ensure server doesn't break
 * Since no PUT or DEL requests are implemented, this should always return null.
 */
TEST_F(HandlerTest, handlePUTDEL)  {
    int sleeptime = 200;
    string url;
    Response r;
    json data;
    url = "/fae.css";
    r = this->requestTask(methods::DEL,  url);
    data = json::parse(r.content);
    ASSERT_EQ(r.status, status_codes::NotFound);
    ASSERT_EQ(data["success"],  false);
    usleep(sleeptime);

    url = "/fa3e.css1";
    r = this->requestTask(methods::PUT,  url);
    data = json::parse(r.content);
    ASSERT_EQ(r.status, status_codes::NotFound);
    ASSERT_EQ(data["success"],  false);
    usleep(sleeptime);

    url = "/fa~e";
    r = this->requestTask(methods::DEL,  url);
    data = json::parse(r.content);
    ASSERT_EQ(r.status, status_codes::NotFound);
    ASSERT_EQ(data["success"],  false);
    usleep(sleeptime);

    url = "/fae2";
    r = this->requestTask(methods::PUT,  url);
    data = json::parse(r.content);
    ASSERT_EQ(r.status, status_codes::NotFound);
    ASSERT_EQ(data["success"],  false);
    usleep(sleeptime);

    url = "/request/3e1";
    r = this->requestTask(methods::DEL,  url);
    data = json::parse(r.content);
    ASSERT_EQ(r.status, status_codes::NotFound);
    ASSERT_EQ(data["success"],  false);
    usleep(sleeptime);

    url = "/abc/def";
    r = this->requestTask(methods::PUT,  url);
    data = json::parse(r.content);
    ASSERT_EQ(r.status, status_codes::NotFound);
    ASSERT_EQ(data["success"],  false);
    usleep(sleeptime);

    url = "/request1/fae";
    r = this->requestTask(methods::DEL,  url);
    data = json::parse(r.content);
    ASSERT_EQ(r.status, status_codes::NotFound);
    ASSERT_EQ(data["success"],  false);
    usleep(sleeptime);
}

/**
 * @brief TEST_F Tests GET requests for erroneous URL inputs to ensure server doesn't break
 * Front-end handles 404 errors. Backend merely returns the same index.html file.
 * The test ensures the html file is returned.
 */
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

    url = "request/museum/";
    r = this->requestTask(methods::GET,  url);
    ASSERT_EQ(r.status, status_codes::OK);
    ASSERT_EQ(r.type,  "text/html");
    ASSERT_NE(r.content,  "");
    usleep(sleeptime);

    url = "request/collection";
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
 * Server must return a 404 message, with a dumped JSON object
 * {
 *  success: false,
 *  message: [string]
 * }
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
    ASSERT_EQ(r.status, status_codes::InternalError);
    ASSERT_EQ(data["success"],  false);
    usleep(sleeptime);

    url = "/request/user-profile/12345";
    r = this->requestTask(methods::POST, url, user);
    data = json::parse(r.content);
    ASSERT_EQ(r.status, status_codes::NotFound);
    ASSERT_EQ(data["success"],  false);
    usleep(sleeptime);
}

//amalolan

/**
 * @brief TEST_F Tests HTTP GET request at /request/museum-list
 * Validity of  Museum JSON objects are tested in model tests.
 * 3 test cases:
 * 1. Model has an exception
 * 2. empty list
 * 3. non-empty list.
 * @see Handler::returnMuseumList() for deatils of the HTTP Message response
 */
TEST_F(HandlerTest, returnMuseumList) {
    int sleeptime = 200;
    string url = "/request/museum-list";
    Response r;
    json expectation;
    vector<Museum> museumList;
    {
        InSequence s;

        /**< Case 1 */
        EXPECT_CALL(this->model, getMuseumList())
                .WillOnce(Throw(ModelException("Museum  list not working")))
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::GET,  url);
        ASSERT_EQ(r.status, status_codes::Conflict);
        ASSERT_FALSE(json::parse(r.content)["success"]);


        /**< Case 2 */
        expectation = json::array();
        EXPECT_CALL(this->model, getMuseumList())
                .WillOnce(Return(museumList))
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::GET,  url);
        ASSERT_EQ(r.status, status_codes::OK);
        ASSERT_EQ(json::parse(r.content),  expectation);


        /**< Case Expect the json list of museums. Can use Util since it is already tested. */
        museumList = {
            Museum("1", "1desc", User("malo", "malo@", "k21")),
            Museum("2", "lsf", "21", "", User("2", "", "")),
            Museum("3", "ls2", "21432", "https://placekitten.com/200/300", User("2", "", "")),
        };
        expectation = Util::arrayFromVector<Museum>(museumList,
        {"id", "name", "description", "introduction", "userID", "image"});

        EXPECT_CALL(this->model, getMuseumList())
                .WillOnce(Return(museumList))
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::GET,  url);
        ASSERT_EQ(r.status, status_codes::OK);
        ASSERT_EQ(json::parse(r.content),  expectation);
    }
}

/**
 * @brief TEST_F Tests HTTP GET request at /request/museum/[id]
 * No ID is tested in handleGET() tests. Validity of collection JSON objects are tested in model tests.
 * 4 Test Cases:
 * 1. Invalid IDs (string and greater than INT_MAX)
 * 2. Valid ID, Not found in DB
 * 3. Valid ID, Found, Valid Museum Object, No Collections
 * 4. Valid ID, Found, Valid Museum Object, Collections present.
 */
TEST_F(HandlerTest, returnMuseumByID) {
    int sleeptime = 200;
    string url = "/request/museum/";  // /request/museum/[id]
    string id;
    Response r;
    json expectation;

    /**< Case 1 */
    id = "AB1";
    usleep(sleeptime);
    r = this->requestTask(methods::GET,  url + id);
    ASSERT_EQ(r.status, status_codes::InternalError);
    ASSERT_FALSE(json::parse(r.content)["success"]);

    id = "91111111111111111111111111111111111111111111111";
    usleep(sleeptime);
    r = this->requestTask(methods::GET,  url + id);
    ASSERT_EQ(r.status, status_codes::InternalError);
    ASSERT_FALSE(json::parse(r.content)["success"]);
    {
        InSequence s;
        /**< Case 2 */
        id = to_string(INT_MAX);
        EXPECT_CALL(this->model, getMuseumObject(stoi(id)))
                .WillOnce(testing::Throw(ModelException("Can't find Museum")))
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::GET,  url + id);
        ASSERT_EQ(r.status, status_codes::Conflict);
        ASSERT_FALSE(json::parse(r.content)["success"]);

        Museum museum("name", "description", "introduction",  "image",
                      User("username", "email", "password", 1), stoi(id));


        /**< Case 3 */
        vector<Collection> collectionList;
        expectation =  {
            {"museum", museum.toJSON()},
            {"collectionList", json::array()}
        };
        EXPECT_CALL(this->model, getMuseumObject(stoi(id)))
                .WillOnce(Return(museum))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getCollectionListByMuseumID(stoi(id)))
                .WillOnce(Return(collectionList))
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::GET, url + id);
        ASSERT_EQ(r.status, status_codes::OK);
        ASSERT_EQ(json::parse(r.content),  expectation);


        /**< Case 4 */
        collectionList.push_back(Collection("collectionName", "collectionDescription",
                                            "collectionIntroduction", "collectionImage",
                                            museum, 1));
        collectionList.push_back(Collection("collectionName", "collectionDescription",
                                            "collectionIntroduction", "collectionImage",
                                            museum, 2));
        expectation["collectionList"]  = Util::arrayFromVector<Collection>(collectionList,
        {"id",  "name", "description", "introduction", "image"});
        EXPECT_CALL(this->model, getMuseumObject(stoi(id)))
                .WillOnce(Return(museum))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getCollectionListByMuseumID(stoi(id)))
                .WillOnce(Return(collectionList))
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::GET, url + id);
        ASSERT_EQ(r.status, status_codes::OK);
        ASSERT_EQ(json::parse(r.content),  expectation);
    }
}

/**
 * @brief TEST_F Tests HTTP GET request at /request/collection/[id]
 * No ID is tested in handleGET() tests. Validity of collection JSON objects are tested in model tests.
 * 4 Test Cases:
 * 1. Invalid IDs (string and greater than INT_MAX)
 * 2. Valid ID, Not found in DB
 * 3. Valid ID, Found, Valid Collection Object, No Artifacts
 * 4. Valid ID, Found, Valid Collections Object, Artifacts present.
 */
TEST_F(HandlerTest, returnCollectionByID) {
    int sleeptime = 500;
    string url = "/request/collection/";  // /request/collection/[id]
    string id;
    Response r;
    json expectation;

    /**< Case 1 */
    id = "AB1";
    usleep(sleeptime);
    r = this->requestTask(methods::GET,  url + id);
    ASSERT_EQ(r.status, status_codes::InternalError);
    ASSERT_FALSE(json::parse(r.content)["success"]);

    id = "91111111111111111111111111111111111111111111111";
    usleep(sleeptime);
    r = this->requestTask(methods::GET,  url + id);
    ASSERT_EQ(r.status, status_codes::InternalError);
    ASSERT_FALSE(json::parse(r.content)["success"]);
    {
        InSequence s;
        /**< Case 2 */
        id = to_string(INT_MAX);
        EXPECT_CALL(this->model, getCollectionObject(stoi(id)))
                .WillOnce(testing::Throw(ModelException("Can't find Collection")))
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::GET,  url + id);
        ASSERT_EQ(r.status, status_codes::Conflict);
        ASSERT_FALSE(json::parse(r.content)["success"]);

        Museum museum("name", "description", "introduction",  "image",
                      User("username", "email", "password", 1), 2);
        Collection collection("collectionName", "collectionDescription",
                              "collectionIntroduction", "collectionImage",
                              museum, stoi(id));

        /**< Case 3 */
        vector<Artifact> artifactList;
        expectation =  {
            {"collection", Util::getObjectWithKeys<Collection>(collection,
             {"id", "name", "description", "introduction", "image"})},
            {"artifactList", json::array()},
            {"museum", Util::getObjectWithKeys<Museum>(collection.getMuseum(),{"id","name"})}
        };
        EXPECT_CALL(this->model, getCollectionObject(stoi(id)))
                .WillOnce(Return(collection))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getArtifactsByCollection(stoi(id)))
                .WillOnce(Return(artifactList))
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::GET, url + id);
        ASSERT_EQ(r.status, status_codes::OK);
        ASSERT_EQ(json::parse(r.content),  expectation);


        /**< Case 4 */
        artifactList.push_back(Artifact("artifactName", "artifactDescription",
                                        "artifactIntroduction", "artifactImage",
                                        museum, 3));
        artifactList.push_back(Artifact("artifactName", "artifactDescription",
                                        "artifactIntroduction", "artifactImage",
                                        museum, 2));
        expectation["artifactList"]  = Util::arrayFromVector<Artifact>(artifactList,
        {"id",  "name", "description", "introduction", "image"});
        EXPECT_CALL(this->model, getCollectionObject(stoi(id)))
                .WillOnce(Return(collection))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getArtifactsByCollection(stoi(id)))
                .WillOnce(Return(artifactList))
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::GET, url + id);
        ASSERT_EQ(r.status, status_codes::OK);
        ASSERT_EQ(json::parse(r.content),  expectation);
    }
}

/**
 * @brief TEST_F Tests HTTP GET request at /request/artifact/[id]
 * No ID is tested in handleGET() tests. Validity of artifact JSON objects are tested in model tests.
 * 4 Test Cases:
 * 1. Invalid IDs (string and greater than INT_MAX)
 * 2. Valid ID, Not found in DB
 * 3. Valid ID, Found, Valid Artifact Object, Single Collection Association
 * 4. Valid ID, Found, Valid Artifact Object, Multiple Collection Associations.
 */
TEST_F(HandlerTest, returnArtifactByID) {
    int sleeptime = 200;
    string url = "/request/artifact/";  // /request/artifact/[id]
    string id;
    Response r;
    json expectation;

    /**< Case 1 */
    id = "AB1";
    usleep(sleeptime);
    r = this->requestTask(methods::GET,  url + id);
    ASSERT_EQ(r.status, status_codes::InternalError);
    ASSERT_FALSE(json::parse(r.content)["success"]);

    id = "91111111111111111111111111111111111111111111111";
    usleep(sleeptime);
    r = this->requestTask(methods::GET,  url + id);
    ASSERT_EQ(r.status, status_codes::InternalError);
    ASSERT_FALSE(json::parse(r.content)["success"]);
    {
        InSequence s;
        /**< Case 2 */
        id = to_string(INT_MAX);
        EXPECT_CALL(this->model, getArtifact(stoi(id)))
                .WillOnce(testing::Throw(ModelException("Can't find Artifact")))
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::GET,  url + id);
        ASSERT_EQ(r.status, status_codes::Conflict);
        ASSERT_FALSE(json::parse(r.content)["success"]);

        Museum museum("name", "description", "introduction",  "image",
                      User("username", "email", "password", 1), 2);
        Artifact artifact("artifactName", "artifactDescription",
                          "artifactIntroduction", "artifactImage",
                          museum, stoi(id));

        /**< Case 3 */
        vector<Collection> collectionList;
        expectation =  {
            {"artifact", Util::getObjectWithKeys<Artifact>(artifact,
             {"id", "name", "description", "introduction", "image"})},
            {"collectionList", json::array()},
            {"museum", Util::getObjectWithKeys<Museum>(artifact.getMuseum(),{"id"})}
        };
        collectionList.push_back(Collection("1collectionName", "1collectionDescription",
                                            "1collectionIntroduction", "1collectionImage",
                                            museum, 12211));
        expectation["collectionList"]  = Util::arrayFromVector<Collection>(collectionList,
        {"id",  "name"});
        EXPECT_CALL(this->model, getArtifact(stoi(id)))
                .WillOnce(Return(artifact))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getCollectionsByArtifact(stoi(id)))
                .WillOnce(Return(collectionList))
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::GET, url + id);
        ASSERT_EQ(r.status, status_codes::OK);
        ASSERT_EQ(json::parse(r.content),  expectation);


        /**< Case 4 */
        collectionList.push_back(Collection("2collectionName", "2collectionDescription",
                                            "2collectionIntroduction", "2collectionImage",
                                            museum, 321));
        collectionList.push_back(Collection("3collectionName", "3collectionDescription",
                                            "3collectionIntroduction", "3collectionImage",
                                            museum, 3211));

        expectation["collectionList"]  = Util::arrayFromVector<Collection>(collectionList,
        {"id",  "name"});
        EXPECT_CALL(this->model, getArtifact(stoi(id)))
                .WillOnce(Return(artifact))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getCollectionsByArtifact(stoi(id)))
                .WillOnce(Return(collectionList))
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::GET, url + id);
        ASSERT_EQ(r.status, status_codes::OK);
        ASSERT_EQ(json::parse(r.content),  expectation);
    }
}


/**
 * @brief TEST_F Tests HTTP GET request at /request/edit/[id]
 * No ID is tested in handleGET() tests. Validity of artifact JSON objects are tested in model tests.
 * 4 Test Cases:
 * 1. Invalid IDs (string and greater than INT_MAX).
 * 2. Valid ID, Edit<Artifact> and Edit<Collection> Object Not found in DB.
 * 3. Valid ID, Found, Valid Edit<Artifact> Object, Single Collection Association.
 * 4. Valid ID, Found, Valid Edit<Artifact> Object, Multiple Collection Associations.
 * 5. Valid ID, Edit<Artifact> not found, Edit<Collection> Object found in DB.
 */
TEST_F(HandlerTest, returnEditByID) {
    int sleeptime = 200;
    string url = "/request/edit/";  // /request/edit/[id]
    string id;
    Response r;
    json expectation;

    /**< Case 1 */
    id = "AB1";
    usleep(sleeptime);
    r = this->requestTask(methods::GET,  url + id);
    ASSERT_EQ(r.status, status_codes::InternalError);
    ASSERT_FALSE(json::parse(r.content)["success"]);

    id = "91111111111111111111111111111111111111111111111";
    usleep(sleeptime);
    r = this->requestTask(methods::GET,  url + id);
    ASSERT_EQ(r.status, status_codes::InternalError);
    ASSERT_FALSE(json::parse(r.content)["success"]);
    {
        InSequence s;
        /**< Case 2 */
        id = to_string(INT_MAX);
        EXPECT_CALL(this->model, getEditArtifactObject(stoi(id)))
                .WillOnce(testing::Throw(ModelException("Can't find Edit Artifact")))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getEditCollectionObject(stoi(id)))
                .WillOnce(testing::Throw(ModelException("Can't find Edit Collection")))
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::GET,  url + id);
        ASSERT_EQ(r.status, status_codes::Conflict);
        ASSERT_FALSE(json::parse(r.content)["success"]);

        Museum museum("name", "description", "introduction",  "image",
                      User("username", "email", "password", 1), 2);
        Artifact artifact("artifactName", "artifactDescription",
                          "artifactIntroduction", "artifactImage",
                          museum, stoi(id));
        Collection collection("1collectionName", "1collectionDescription",
                              "1collectionIntroduction", "1collectionImage",
                              museum, 12211);
        /**< Case 3 */
        vector<Collection> collectionList =  {collection};
        Edit<Artifact> edit1(artifact, 0, User("a", "b@b.com", "pass"), collectionList, 1,  2134);
        expectation =  Util::getArtifactEditJSON(edit1);

        EXPECT_CALL(this->model, getEditArtifactObject(stoi(id)))
                .WillOnce(Return(edit1))
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::GET, url + id);
        ASSERT_EQ(r.status, status_codes::OK);
        ASSERT_EQ(json::parse(r.content),  expectation);


        /**< Case 4 */
        collectionList.push_back(Collection("2collectionName", "2collectionDescription",
                                            "2collectionIntroduction", "2collectionImage",
                                            museum, 321));
        collectionList.push_back(Collection("3collectionName", "3collectionDescription",
                                            "3collectionIntroduction", "3collectionImage",
                                            museum, 3211));
        Edit<Artifact> edit2(artifact, 0, User("a", "b@b.com", "pass"), collectionList, 1,  2134);
        expectation =  Util::getArtifactEditJSON(edit2);

        EXPECT_CALL(this->model, getEditArtifactObject(stoi(id)))
                .WillOnce(Return(edit2))
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::GET, url + id);
        ASSERT_EQ(r.status, status_codes::OK);
        ASSERT_EQ(json::parse(r.content),  expectation);

        /**< Case 5 */
        Edit<Collection> edit3(collection, 1, User("12a", "er@13", "af"), -1, 12);
        expectation =  Util::getCollectionEditJSON(edit3);
        EXPECT_CALL(this->model, getEditArtifactObject(stoi(id)))
                .WillOnce(testing::Throw(ModelException("Can't find Edit Artifact")))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getEditCollectionObject(stoi(id)))
                .WillOnce(Return(edit3))
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::GET,  url + id);
        ASSERT_EQ(r.status, status_codes::OK);
        ASSERT_EQ(json::parse(r.content),  expectation);
    }
}


/**
 * @brief HandlerTest::loginTest Helper test function to ensure log in actually denies unauthorized users.
 * Called by many POST request test functions.
 * Validity of user JSON objects are tested in model tests.
 * Validity of Username, Password asserted on front-end.
 * 3/4 Test Cases for logging in:
 * <b>[0. Invalid POST object (no user object). Not here. Present in POST request tests]</b>
 * 1. Invalid POST data (no username & password).
 * 2. Valid data, Username not found in DB.
 * 3. Valid data, Username Found, Password mismatch.
 * <b>[4. Valid data, Username Found, Password match. Not here. Present POST request tests]</b>
 * @param url The POST url to perform the login test on.
 * @param data The data to send with the POST url. data["user"] must be a valid object of exactly
 * data["user"] = {
 *     "username":  string,
 * }
 */
void HandlerTest::loginTest(string url, json data) {
    int sleeptime = 300;
    string username = data["user"]["username"];
    User user(username,"email@example.com", "realpassword", 12);
    Response r;
    {
        InSequence s;
        /**< Case 1 */
        data["user"] = {
        {"username",  username},
        {"passwefd", "password~*?"}
    };

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::InternalError);
        ASSERT_FALSE(json::parse(r.content)["success"]);

        /**< Case 2 */
        data["user"] = {
        {"username",  username},
        {"password", "password~*?"}
    };
        /**< Case 1 */
        EXPECT_CALL(this->model, getUserObject(username))
                .WillOnce(testing::Throw(ModelException("Can't find User")))
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::Conflict);
        ASSERT_FALSE(json::parse(r.content)["success"]);

        /**< Case 3 */
        EXPECT_CALL(this->model, getUserObject(username))
                .WillOnce(Return(user))
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        usleep(sleeptime);
        ASSERT_EQ(r.status, status_codes::Unauthorized);
        ASSERT_FALSE(json::parse(r.content)["success"]);
    }
}

/**
 * @brief HandlerTest::loginTest Helper test function to ensure log in actually denies unauthorized users.
 * Performs login tests for those that take minimal POST data of:
 * {
 *   "username": string,
 *   "password": string
 * }
 * Validity of user JSON objects are tested in model tests.
 * Validity of Username, Password asserted on front-end.
 * 3/4 Test Cases for logging in:
 * 0. Invalid POST object (no user object).
 * 1. Invalid POST data (no username & password).
 * 2. Valid data, Username not found in DB.
 * 3. Valid data, Username Found, Password mismatch.
 * <b>[4. Valid data, Username Found, Password match. Not here. Present POST request tests]</b>
 * @param url The POST url to perform the login test on.
 */
void HandlerTest::loginTest(string url) {
    int sleeptime = 300;
    Response r;
    json data;
    {
        InSequence s;
        /**< Case 0 */
        data = json::object();

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::InternalError);
        ASSERT_FALSE(json::parse(r.content)["success"]);

        data = {
            {"username", "aa"},
            {"password12", "Bb"},
        };

        /**< Case 2 */
        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::InternalError);
        ASSERT_FALSE(json::parse(r.content)["success"]);

        /**< Cases 3- */
        data = {
            {"username", "test1234"},
            {"password", "pass"},
        };

        /**< Case 3 */
        EXPECT_CALL(this->model, getUserObject((string) data["username"]))
                .WillOnce(Throw(ModelException("User not in DB")))
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::Conflict);
        ASSERT_FALSE(json::parse(r.content)["success"]);

        User user(data["username"], "email@example.com", "realpassword", 21);

        /**< Case 4 */
        EXPECT_CALL(this->model, getUserObject((string) data["username"]))
                .WillOnce(Return(user))
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::Unauthorized);
        ASSERT_FALSE(json::parse(r.content)["success"]);
    }
}

/**
 * @brief TEST_F Tests HTTP POST request at /request/login
 * @see HandlerTest::loginTest()
 * Validity of user JSON objects are tested in model tests.
 * Validity of Username, Password asserted on front-end form validation.
 * 5 Test Cases for logging in:
 * 1. Empty POST data
 * 2. Invalid POST data (no username & password).
 * 3. Valid data, Username not found in DB.
 * 4. Valid data, Username Found, Password mismatch.
 * 5. Valid data, Username Found, Password match.
 */
TEST_F(HandlerTest, validateLogin) {
    int sleeptime = 200;
    string username;
    string url = "/request/login";
    Response r;
    json data  = {
        {"username", "ad"},
        {"password", "realpassword"}
    };

    /**< Cases 1-4*/
    this->loginTest(url);
    /**< Case 4*/
    User user(data["username"], "email@example.com", "realpassword", 21);
    EXPECT_CALL(this->model, getUserObject((string) data["username"]))
            .Times(testing::AtLeast(1))
            .WillOnce(Return(user))
            .RetiresOnSaturation();

    usleep(sleeptime);
    r = this->requestTask(methods::POST, url, data);
    ASSERT_EQ(r.status, status_codes::OK);
    ASSERT_TRUE(json::parse(r.content)["success"]);

}

/**
 * @brief TEST_F Tests HTTP POST request at /request/register
 * Validity of Username, Password, email asserted on front-end form validation.
 * 4 Test Cases:
 * 1. Invalid POST data (no data).
 * 2. Invalid POST data (no username/password/email).
 * 3. Valid data, User in DB.
 * 4. Valid data, User not in DB.
 */
TEST_F(HandlerTest, addUser) {
    int sleeptime = 200;
    string url = "/request/register";
    Response r;
    json data;

    {
        InSequence s;
        /**< Case 1 */
        data = json::object();

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::InternalError);
        ASSERT_FALSE(json::parse(r.content)["success"]);

        data = {
            {"username", "aa"},
            {"password", "Bb"},
            {"email12121", 12}
        };

        /**< Case 2 */
        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::InternalError);
        ASSERT_FALSE(json::parse(r.content)["success"]);

        /**< Cases 3- */
        data = {
            {"username", "test1234"},
            {"password", "pass"},
            {"email", "test@gmail.com"}
        };
        User user(data["username"], data["email"], data["password"], -1);


        /**< Case 3 */
        EXPECT_CALL(this->model, saveUserToDB(user))
                .WillOnce(Throw(ModelException("User in DB")))
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::Conflict);
        ASSERT_FALSE(json::parse(r.content)["success"]);

        /**< Case 4 */
        EXPECT_CALL(this->model, saveUserToDB(user))
                .Times(1)
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::OK);
        ASSERT_TRUE(json::parse(r.content)["success"]);
    }
}


/**
 * @brief TEST_F Tests HTTP POST request at /request/add-museum/
 * @see HandlerTest::loginTest()
 * Needs login. Tested by helper function loginTest()
 * 8 Test Cases:
 * 1. Invalid POST data (no user/museum).
 * 2. Invalid museum POST data (bad museum nested object).
 * 3. Invalid user POST data (no username/password).
 * 4. Valid data, Username not found in DB.
 * 5. Valid data, Username Found, Password mismatch.
 * 6. Valid data, Logged in, Valid Museum data, Museum already in DB.
 * 7. Valid data, Logged in, Valid Museum data, Museum not in DB.
 */
TEST_F(HandlerTest, addMuseum) {
    int sleeptime = 200;
    string url = "/request/add-museum";
    Response r;
    json data;
    {
        InSequence s;
        /**< Case 1 */
        data = json::object();

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::InternalError);
        ASSERT_FALSE(json::parse(r.content)["success"]);

        data = {
            {"museum", {
                 {"n3ame", "museumName"},
                 {"descrip124tion", "museumDescription"},
                 {"in21troduction", "museumIntroduction"},
             }},
            {"user", {
                 {"username", "testing123"},
                 {"password", "realpassword"}
             }}
        };

        /**< Case 2 */
        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::InternalError);
        ASSERT_FALSE(json::parse(r.content)["success"]);

        /**< Cases 3- */
        data["museum"] = {
        {"name", "museumName"},
        {"description", "museumDescription"},
        {"introduction", "museumIntroduction"},
        {"image",  ""}
    };

        /**< Cases 3-5 */
        this->loginTest(url, data);
        cout<<"Finished loginTest"<<endl;
        User user(data["user"]["username"], "email@example.com", data["user"]["password"], 21);

        /**< Cases 6- */
        Museum m(data["museum"]["name"],data["museum"]["description"],
                data["museum"]["introduction"], data["museum"]["image"], user);

        /**< Case 6 */
        EXPECT_CALL(this->model, getUserObject((string) data["user"]["username"]))
                .Times(testing::AtLeast(1))
                .WillRepeatedly(Return(user))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, saveMuseumToDB(m))
                .WillOnce(Throw(ModelException("Museum in DB")))
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::Conflict);
        ASSERT_FALSE(json::parse(r.content)["success"]);

        /**< Case 7 */
        EXPECT_CALL(this->model, getUserObject((string) data["user"]["username"]))
                .Times(testing::AtLeast(1))
                .WillRepeatedly(Return(user))
                .RetiresOnSaturation();

        EXPECT_CALL(this->model, saveMuseumToDB(m))
                .Times(1)
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::OK);
        ASSERT_TRUE(json::parse(r.content)["success"]);
    }
}

/**
 * @brief TEST_F Tests HTTP POST request at /request/add-collection/
 * @see HandlerTest::loginTest()
 * Needs login. Tested by helper function loginTest()
 * 10 Test Cases:
 * 1. Invalid POST data (no user/museum/collection).
 * 2. Invalid collection POST data (bad collection nested object).
 * 3. Invalid museum POST data (no id in nested museum object).
 * 4. Invalid user POST data (no username/password).
 * 5. Valid data, Username not found in DB.
 * 6. Valid data, Username Found, Password mismatch.
 * 7. Valid data, Logged in, Museum not in DB.
 * 8. Valid data, Logged in, Curator actor, Museum and Collection in DB.
 * 9. Valid data, Logged in, Curator actor, Museum in DB, Collection not in DB.
 * 10. Valid data, Logged in, non-curator actor, Museum in DB.
 */
TEST_F(HandlerTest, addCollection) {
    int sleeptime = 200;
    string url = "/request/add-collection";
    Response r;
    json data;

    {
        InSequence s;
        /**< Case 1 */
        data = json::object();

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::InternalError);
        ASSERT_FALSE(json::parse(r.content)["success"]);

        data = {
            {"museum", {
                 {"i12d", 123}
             }},
            {"collection", {
                 {"nam1e", "col1"},
                 {"desc2ription", ""}
             }},
            {"user", {
                 {"username", "testing123"},
                 {"password", "realpassword"}
             }}
        };

        /**< Case 2 */
        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::InternalError);
        ASSERT_FALSE(json::parse(r.content)["success"]);

        /**< Cases 3- */
        data["collection"] = {
        {"name", "colName"},
        {"description", "colDescription"},
        {"introduction", "colIntroduction"},
        {"image",  ""}
    };

        /**< Case 3 */
        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::InternalError);
        ASSERT_FALSE(json::parse(r.content)["success"]);

        /**< Cases 4- */
        data["museum"] = {
        {"id", 121}
    };

        /**< Cases 4-6 */
        this->loginTest(url, data);
        cout<<"Finished loginTest"<<endl;
    }
    /**< Cases 7- */
    User user(data["user"]["username"], "email@example.com", data["user"]["password"], 21);
    Museum museum("","","","", user, data["museum"]["id"]);
    Collection collection(data["collection"]["name"],data["collection"]["description"],
            data["collection"]["introduction"], data["collection"]["image"], museum);
    User curator("curator", "curator@example.com", "1234", 22);
    Museum otherMuseum("","","","", curator, data["museum"]["id"]);
    Collection otherCollection(data["collection"]["name"],data["collection"]["description"],
            data["collection"]["introduction"], data["collection"]["image"], otherMuseum);
    Edit<Collection> edit(otherCollection, Edit<Collection>::add, user);
    {
        InSequence s;
        /**< Case 7 */
        EXPECT_CALL(this->model, getUserObject((string) data["user"]["username"]))
                .Times(testing::AtLeast(1))
                .WillRepeatedly(Return(user))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getMuseumObject((int) data["museum"]["id"]))
                .WillOnce(Throw(ModelException("Museum not in DB")))
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::Conflict);
        ASSERT_FALSE(json::parse(r.content)["success"]);

        /**< Case 8 */
        EXPECT_CALL(this->model, getUserObject((string) data["user"]["username"]))
                .Times(testing::AtLeast(1))
                .WillRepeatedly(Return(user))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getMuseumObject((int) data["museum"]["id"]))
                .WillOnce(Return(museum))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, saveCollectionToDB(collection))
                .WillOnce(Throw(ModelException("Collection in DB")))
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::Conflict);
        ASSERT_FALSE(json::parse(r.content)["success"]);

        /**< Case 9 */
        EXPECT_CALL(this->model, getUserObject((string) data["user"]["username"]))
                .Times(testing::AtLeast(1))
                .WillRepeatedly(Return(user))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getMuseumObject((int) data["museum"]["id"]))
                .WillOnce(Return(museum))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, saveCollectionToDB(collection))
                .Times(1)
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::OK);
        ASSERT_TRUE(json::parse(r.content)["success"]);

        /**< Case 10 */
        EXPECT_CALL(this->model, getUserObject((string) data["user"]["username"]))
                .Times(testing::AtLeast(1))
                .WillRepeatedly(Return(user))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getMuseumObject((int) data["museum"]["id"]))
                .WillOnce(Return(otherMuseum))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, saveEditToDB(edit))
                .Times(1)
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::OK);
        ASSERT_TRUE(json::parse(r.content)["success"]);
    }
}

/**
 * @brief TEST_F Tests HTTP POST request at /request/add-collection/
 * @see HandlerTest::loginTest()
 * Needs login. Tested by helper function loginTest()
 * 11 Test Cases:
 * 1. Invalid POST data (no user/museum/collection).
 * 2. Invalid collection POST data (bad collection nested object).
 * 3. Invalid museum POST data (no id in nested museum object).
 * 4. Invalid user POST data (no username/password).
 * 5. Valid data, Username not found in DB.
 * 6. Valid data, Username Found, Password mismatch.
 * 7. Valid data, Logged in, Museum not in DB.
 * 8. Valid data, Logged in, Curator actor, Museum in DB, Collection not in DB.
 * 9. Valid data, Logged in, Curator actor, Museum in DB, Collection and Updated Collection in DB.
 * 10. Valid data, Logged in, Curator actor, Museum and Collection in DB, and Updated not in DB.
 * 11. Valid data, Logged in, non-curator actor, Museum and Collection in DB.
 */
TEST_F(HandlerTest, editCollection) {
    int sleeptime = 200;
    string url = "/request/edit-collection";
    Response r;
    json data;

    {
        InSequence s;
        /**< Case 1 */
        data = json::object();

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::InternalError);
        ASSERT_FALSE(json::parse(r.content)["success"]);

        data = {
            {"museum", {
                 {"i12d", 123}
             }},
            {"collection", {
                 {"nam1e", "col1"},
                 {"desc2ription", ""}
             }},
            {"user", {
                 {"username", "testing123"},
                 {"password", "realpassword"}
             }}
        };

        /**< Case 2 */
        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::InternalError);
        ASSERT_FALSE(json::parse(r.content)["success"]);

        /**< Cases 3- */
        data["collection"] = {
        {"name", "colName"},
        {"description", "colDescription"},
        {"introduction", "colIntroduction"},
        {"image",  ""},
        {"id", 123}
    };

        /**< Case 3 */
        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::InternalError);
        ASSERT_FALSE(json::parse(r.content)["success"]);

        /**< Cases 4- */
        data["museum"] = {
        {"id", 121}
    };

        /**< Cases 4-6 */
        this->loginTest(url, data);
        cout<<"Finished loginTest"<<endl;
    }
    /**< Cases 7- */
    User user(data["user"]["username"], "email@example.com", data["user"]["password"], 21);
    Museum museum("","","","", user, data["museum"]["id"]);
    Collection collection(data["collection"]["name"],data["collection"]["description"],
            data["collection"]["introduction"], data["collection"]["image"],
            museum, data["collection"]["id"]);
    User curator("curator", "curator@example.com", "1234", 22);
    Museum otherMuseum("","","","", curator, data["museum"]["id"]);
    Collection otherCollection(data["collection"]["name"],data["collection"]["description"],
            data["collection"]["introduction"], data["collection"]["image"],
            otherMuseum, data["collection"]["id"]);
    Edit<Collection> edit(otherCollection, Edit<Collection>::edit, user);
    {
        InSequence s;
        /**< Case 7 */
        EXPECT_CALL(this->model, getUserObject((string) data["user"]["username"]))
                .Times(testing::AtLeast(1))
                .WillRepeatedly(Return(user))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getMuseumObject((int) data["museum"]["id"]))
                .WillOnce(Throw(ModelException("Museum not in DB")))
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::Conflict);
        ASSERT_FALSE(json::parse(r.content)["success"]);

        /**< Case 8 */
        EXPECT_CALL(this->model, getUserObject((string) data["user"]["username"]))
                .Times(testing::AtLeast(1))
                .WillRepeatedly(Return(user))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getMuseumObject((int) data["museum"]["id"]))
                .WillOnce(Return(museum))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getCollectionObject((int) data["collection"]["id"]))
                .WillOnce(Throw(ModelException("Collection not in DB")))
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::Conflict);
        ASSERT_FALSE(json::parse(r.content)["success"]);

        /**< Case 9 */
        EXPECT_CALL(this->model, getUserObject((string) data["user"]["username"]))
                .Times(testing::AtLeast(1))
                .WillRepeatedly(Return(user))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getMuseumObject((int) data["museum"]["id"]))
                .WillOnce(Return(museum))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getCollectionObject((int) data["collection"]["id"]))
                .WillOnce(Return(collection))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, updateCollectionInDB(collection))
                .WillOnce(Throw(ModelException("Update failed as another artifact exists with the same name.")))
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::Conflict);
        ASSERT_FALSE(json::parse(r.content)["success"]);

        /**< Case 10 */
        EXPECT_CALL(this->model, getUserObject((string) data["user"]["username"]))
                .Times(testing::AtLeast(1))
                .WillRepeatedly(Return(user))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getMuseumObject((int) data["museum"]["id"]))
                .WillOnce(Return(museum))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getCollectionObject((int) data["collection"]["id"]))
                .WillOnce(Return(collection))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, updateCollectionInDB(collection))
                .Times(1)
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::OK);
        ASSERT_TRUE(json::parse(r.content)["success"]);

        /**< Case 13 */
        EXPECT_CALL(this->model, getUserObject((string) data["user"]["username"]))
                .Times(testing::AtLeast(1))
                .WillRepeatedly(Return(user))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getMuseumObject((int) data["museum"]["id"]))
                .WillOnce(Return(otherMuseum))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getCollectionObject((int) data["collection"]["id"]))
                .WillOnce(Return(otherCollection))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, saveEditToDB(edit))
                .Times(1)
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::OK);
        ASSERT_TRUE(json::parse(r.content)["success"]);
    }
}

/**
 * @brief TEST_F Tests HTTP POST /request/add-artifact
 * @see HandlerTest::loginTest()
 * Needs login. Tested by helper function loginTest()
 * 12 Test Cases:
 * 1. Invalid POST data (no user/museum/artifact).
 * 2. Invalid artifact POST data (bad artifact nested object).
 * 3. Invalid museum POST data (no id in nested museum object).
 * 4. Invalid user POST data (no username/password).
 * 5. Valid data, Username not found in DB.
 * 6. Valid data, Username Found, Password mismatch.
 * 7. Valid data, Logged in, Museum not in DB.
 * 8. Valid data, Logged in, Museum found, Collection List empty.
 * 9. Valid data, Logged in, Museum found, Collection List not empty, one of the collections not in DB.
 * 10. Valid data, Logged in, Curator actor, Museum, Collections, and Artifact in DB.
 * 11. Valid data, Logged in, Curator actor, Museum and Collections in DB, Artifact not in DB.
 * 12. Valid data, Logged in, Non-curator actor, Museum and Collections in DB.
 */
TEST_F(HandlerTest, addArtifact) {
    int sleeptime = 300;
    string url = "/request/add-artifact";
    Response r;
    json data;
    /**< Cases 1-6 */
    {
        InSequence s;
        /**< Case 1 */
        data = json::object();

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::InternalError);
        ASSERT_FALSE(json::parse(r.content)["success"]);

        data =
        {
            {"collection", json::array()},
            {"museum", {{"i12d", 123}}},
            {"artifact", {
                 {"nam1e",   "col1"},
                 {"desc2ription", ""}
             }},
            {"user", {
                 {"username", "testing123"},
                 {"password", "realpassword"}
             }}
        };

        /**< Case 2 */
        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::InternalError);
        ASSERT_FALSE(json::parse(r.content)["success"]);

        /**< Cases 3- */
        data["artifact"] = {
        {"name", "aName"},
        {"description", "aDescription"},
        {"introduction", "aIntroduction"},
        {"image",  ""},
        {"id", 32},
    };

        /**< Case 3 */
        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::InternalError);
        ASSERT_FALSE(json::parse(r.content)["success"]);
        usleep(sleeptime);


        /**< Cases 4- */
        data["museum"] = {
        {"id", 121}
    };

        /**< Cases 4-6 */
        this->loginTest(url, data);
        cout<<"Finished loginTest"<<endl;
    }
    /**< Cases 7- */
    User user(data["user"]["username"], "email@example.com", data["user"]["password"], 21);
    Museum museum("","","","", user, data["museum"]["id"]);
    Artifact artifact(data["artifact"]["name"], data["artifact"]["description"], data["artifact"]["introduction"],
            data["artifact"]["image"], museum);
    User curator("curator", "curator@example.com", "1234", 22);
    Museum otherMuseum("","","","", curator, data["museum"]["id"]);
    Artifact otherArtifact(data["artifact"]["name"], data["artifact"]["description"], data["artifact"]["introduction"],
            data["artifact"]["image"], otherMuseum);
    Edit<Artifact> edit(otherArtifact, Edit<Artifact>::add, user, {Collection("", "", otherMuseum), Collection("", "", otherMuseum), Collection("", "", otherMuseum)});

    {
        InSequence s;
        /**< Case 7 */
        EXPECT_CALL(this->model, getUserObject((string) data["user"]["username"]))
                .Times(testing::AtLeast(1))
                .WillRepeatedly(Return(user))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getMuseumObject((int) data["museum"]["id"]))
                .WillOnce(Throw(ModelException("Museum not in DB")))
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::Conflict);
        ASSERT_FALSE(json::parse(r.content)["success"]);

        /**< Case 8 */
        EXPECT_CALL(this->model, getUserObject((string) data["user"]["username"]))
                .Times(testing::AtLeast(1))
                .WillRepeatedly(Return(user))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getMuseumObject((int) data["museum"]["id"]))
                .WillOnce(Return(museum))
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::Conflict);
        ASSERT_FALSE(json::parse(r.content)["success"]);

        /**< Case 9 */
        data["collection"]  = json::array({132,3144,76543});
        EXPECT_CALL(this->model, getUserObject((string) data["user"]["username"]))
                .Times(testing::AtLeast(1))
                .WillRepeatedly(Return(user))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getMuseumObject((int) data["museum"]["id"]))
                .WillOnce(Return(museum))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getCollectionObject(testing::_))
                .WillOnce(Throw(ModelException("Collection doesn't exist")))
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::Conflict);
        ASSERT_FALSE(json::parse(r.content)["success"]);

        /**< Case 10 */
        EXPECT_CALL(this->model, getUserObject((string) data["user"]["username"]))
                .Times(testing::AtLeast(1))
                .WillRepeatedly(Return(user))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getMuseumObject((int) data["museum"]["id"]))
                .WillOnce(Return(museum))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getCollectionObject(testing::_))
                .WillRepeatedly(Return(Collection("", "", museum)))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, saveArtifactToDB(artifact))
                .WillOnce(Throw(ModelException("Artifact already exists")))
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::Conflict);
        ASSERT_FALSE(json::parse(r.content)["success"]);

        cout<<"Reached Curator add"<<endl;
        /**< Case 11 */
        EXPECT_CALL(this->model, getUserObject((string) data["user"]["username"]))
                .Times(testing::AtLeast(1))
                .WillRepeatedly(Return(user))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getMuseumObject((int) data["museum"]["id"]))
                .WillOnce(Return(museum))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getCollectionObject(testing::_))
                .WillRepeatedly(Return(Collection("", "", museum)))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, saveArtifactToDB(artifact))
                .Times(1)
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, addArtifactCollection(artifact, testing::_))
                .Times(3)
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::OK);
        ASSERT_TRUE(json::parse(r.content)["success"]);

        /**< Case 12  */

        EXPECT_CALL(this->model, getUserObject((string) data["user"]["username"]))
                .Times(testing::AtLeast(1))
                .WillRepeatedly(Return(user))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getMuseumObject((int) data["museum"]["id"]))
                .WillOnce(Return(otherMuseum))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getCollectionObject(testing::_))
                .WillRepeatedly(Return(Collection("", "", otherMuseum)))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, saveEditToDB(edit))
                .Times(1)
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::OK);
        ASSERT_TRUE(json::parse(r.content)["success"]);
    }
}

/**
 * @brief TEST_F Tests HTTP POST /request/edit-artifact
 * @see HandlerTest::loginTest()
 * Needs login. Tested by helper function loginTest()
 * 12 Test Cases:
 * 1. Invalid POST data (no user/museum/artifact).
 * 2. Invalid artifact POST data (bad artifact nested object).
 * 3. Invalid museum POST data (no id in nested museum object).
 * 4. Invalid user POST data (no username/password).
 * 5. Valid data, Username not found in DB.
 * 6. Valid data, Username Found, Password mismatch.
 * 7. Valid data, Logged in, Museum not in DB.
 * 8. Valid data, Logged in, Museum found, Collection List empty.
 * 9. Valid data, Logged in, Museum found, Collection List not empty, one of the collections not in DB.
 * 10. Valid data, Logged in, Museum and Collections in DB, Artifact not in DB.
 * 11. Valid data, Logged in, Curator actor, Museum and Collections, Artifact and Updated Artifact in DB.
 * 12. Valid data, Logged in, Curator actor, Museum, Collections, and Artifact in DB, Updated not in DB.
 * 13. Valid data, Logged in, Non-curator actor, Museum found, Collections in DB, Artifact in DB.
 */
TEST_F(HandlerTest, editArtifact) {
    int sleeptime = 300;
    string url = "/request/edit-artifact";
    Response r;
    json data;
    /**< Cases 1-6 */
    {
        InSequence s;
        /**< Case 1 */
        data = json::object();

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::InternalError);
        ASSERT_FALSE(json::parse(r.content)["success"]);

        data =
        {
            {"collection", json::array()},
            {"museum", {{"i12d", 123}}},
            {"artifact", {
                 {"nam1e",   "col1"},
                 {"desc2ription", ""}
             }},
            {"user", {
                 {"username", "testing123"},
                 {"password", "realpassword"}
             }}
        };

        /**< Case 2 */
        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::InternalError);
        ASSERT_FALSE(json::parse(r.content)["success"]);

        /**< Cases 3- */
        data["artifact"] = {
        {"name", "aName"},
        {"description", "aDescription"},
        {"introduction", "aIntroduction"},
        {"image",  ""},
        {"id", 32},
    };

        /**< Case 3 */
        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::InternalError);
        ASSERT_FALSE(json::parse(r.content)["success"]);


        /**< Cases 4- */
        data["museum"] = {
        {"id", 121}
    };

        /**< Cases 4-6 */
        this->loginTest(url, data);
        cout<<"Finished loginTest"<<endl;
    }

    /**< Cases 7- */
    User user(data["user"]["username"], "email@example.com", data["user"]["password"], 21);
    Museum museum("","","","", user, data["museum"]["id"]);
    Artifact artifact(data["artifact"]["name"], data["artifact"]["description"], data["artifact"]["introduction"],
            data["artifact"]["image"], museum, data["artifact"]["id"]);
    User curator("curator", "curator@example.com", "1234", 22);
    Museum otherMuseum("","","","", curator, data["museum"]["id"]);
    Artifact otherArtifact(data["artifact"]["name"], data["artifact"]["description"], data["artifact"]["introduction"],
            data["artifact"]["image"], otherMuseum, data["artifact"]["id"]);
    Edit<Artifact> edit(otherArtifact, Edit<Artifact>::edit, user,
    {Collection("", "", otherMuseum), Collection("", "", otherMuseum), Collection("", "", otherMuseum)});
    {
        InSequence s;
        /**< Case 7 */
        EXPECT_CALL(this->model, getUserObject((string) data["user"]["username"]))
                .Times(testing::AtLeast(1))
                .WillRepeatedly(Return(user))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getMuseumObject((int) data["museum"]["id"]))
                .WillOnce(Throw(ModelException("Museum not in DB")))
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::Conflict);
        ASSERT_FALSE(json::parse(r.content)["success"]);


        /**< Case 8 */
        EXPECT_CALL(this->model, getUserObject((string) data["user"]["username"]))
                .Times(testing::AtLeast(1))
                .WillRepeatedly(Return(user))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getMuseumObject((int) data["museum"]["id"]))
                .WillOnce(Return(museum))
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::Conflict);
        ASSERT_FALSE(json::parse(r.content)["success"]);

        /**< Case 9 */
        data["collection"]  = json::array({132,3144,76543});
        EXPECT_CALL(this->model, getUserObject((string) data["user"]["username"]))
                .Times(testing::AtLeast(1))
                .WillRepeatedly(Return(user))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getMuseumObject((int) data["museum"]["id"]))
                .WillOnce(Return(museum))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getCollectionObject(testing::_))
                .WillOnce(Throw(ModelException("Collection doesn't exist")))
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::Conflict);
        ASSERT_FALSE(json::parse(r.content)["success"]);

        /**< Case 10 */
        EXPECT_CALL(this->model, getUserObject((string) data["user"]["username"]))
                .Times(testing::AtLeast(1))
                .WillRepeatedly(Return(user))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getMuseumObject((int) data["museum"]["id"]))
                .WillOnce(Return(museum))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getCollectionObject(testing::_))
                .WillRepeatedly(Return(Collection("", "", museum)))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getArtifact((int)data["artifact"]["id"]))
                .WillOnce(Throw(ModelException("Artifact doesn't exist")))
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::Conflict);
        ASSERT_FALSE(json::parse(r.content)["success"]);

        /**< Case 11 */
        EXPECT_CALL(this->model, getUserObject((string) data["user"]["username"]))
                .Times(testing::AtLeast(1))
                .WillRepeatedly(Return(user))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getMuseumObject((int) data["museum"]["id"]))
                .WillOnce(Return(museum))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getCollectionObject(testing::_))
                .WillRepeatedly(Return(Collection("", "", museum)))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getArtifact((int) data["artifact"]["id"]))
                .WillRepeatedly(Return(artifact))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, updateArtifactInDB(artifact))
                .Times(1)
                .WillOnce(Throw(ModelException("Update failed as another artifact exists with the same name.")))
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::Conflict);
        ASSERT_FALSE(json::parse(r.content)["success"]);

        /**< Case 12 */
        EXPECT_CALL(this->model, getUserObject((string) data["user"]["username"]))
                .Times(testing::AtLeast(1))
                .WillRepeatedly(Return(user))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getMuseumObject((int) data["museum"]["id"]))
                .WillOnce(Return(museum))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getCollectionObject(testing::_))
                .WillRepeatedly(Return(Collection("", "", museum)))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getArtifact((int) data["artifact"]["id"]))
                .WillRepeatedly(Return(artifact))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, updateArtifactInDB(artifact))
                .Times(1)
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, removeArtifactCollection(artifact))
                .Times(testing::AtLeast(1))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, addArtifactCollection(artifact, testing::_))
                .Times(3)
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::OK);
        ASSERT_TRUE(json::parse(r.content)["success"]);

        /**< Case 13 */
        EXPECT_CALL(this->model, getUserObject((string) data["user"]["username"]))
                .Times(testing::AtLeast(1))
                .WillRepeatedly(Return(user))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getMuseumObject((int) data["museum"]["id"]))
                .WillOnce(Return(otherMuseum))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getCollectionObject(testing::_))
                .WillRepeatedly(Return(Collection("", "", otherMuseum)))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getArtifact((int) data["artifact"]["id"]))
                .WillRepeatedly(Return(otherArtifact))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, saveEditToDB(edit))
                .Times(1)
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::OK);
        ASSERT_TRUE(json::parse(r.content)["success"]);
    }
}

/**
 * @brief TEST_F Tests HTTP POST /request/delete-museum
 * @see HandlerTest::loginTest()
 * Needs login. Tested by helper function loginTest()
 * 19 Test Cases:
 * 1. Invalid POST data (no user/editId/category/action).
 * 2. Invalid category POST data.
 * 3. Invalid user POST data (no username/password).
 * 4. Valid data, Username not found in DB.
 * 5. Valid data, Username Found, Password mismatch.
 * 6. Valid data, Logged in, edit not in DB.
 * 7. Valid data, Logged in, edit found, already acted on.
 * 8. Valid data, Logged in, edit found, not acted on, not curator.
 * 9. Valid data, Logged in, edit found, not acted on, curator, reject add Artifact.
 * 10. Valid data, Logged in, edit found, not acted on, curator, reject edit Collection.
 * 11. Valid data, Logged in, edit found, not acted on, curator, reject delete Artifact.
 * 12. Valid data, Logged in, edit found, not acted on, curator, approve add Artifact.
 * 13. Valid data, Logged in, edit found, not acted on, curator, approve edit Artifact.
 * 14. Valid data, Logged in, edit found, not acted on, curator, approve delete Artifact.
 * 15. Valid data, Logged in, edit found, not acted on, curator, approve add Collection.
 * 16. Valid data, Logged in, edit found, not acted on, curator, approve edit Artifact.
 * 17. Valid data, Logged in, edit found, not acted on, curator, approve delete Artifact.
 * 18. Valid data, Logged in, edit found, not acted on, curator, reject edit Museum.
 * 19. Valid data, Logged in, edit found, not acted on, curator, approve delete Museum.
 */
TEST_F(HandlerTest, reviewEdit) {

}



/**
 * @brief TEST_F Tests HTTP POST request at /request/user-profile
 * @see HandlerTest::loginTest()
 * 4 Test Cases:
 * 1. Invalid POST data (no data).
 * 2. Invalid POST data (no username/password).
 * 3. Valid data, User not in DB.
 * 4. Valid data, User in DB, password mismatch.
 * 5. Logged in.
 */
TEST_F(HandlerTest, getUserProfile) {
    int sleeptime = 300;
    string url = "/request/user-profile";
    Response r;
    json data = {
        {"username", "abc"},
        {"password", "realpassword"}
    };
    json expectation;
    /**< Cases 1-4*/
    this->loginTest(url);
    /**< Cases 4-*/
    // getArtifactEdits
    // getMuseumByCurator
    // for each museum: getArtifactActions
    User user(data["username"], "email@example.com", data["password"], 21);
    //    {
    //        InSequence s;

    //        /**< Case 5 */
    //        data["password"] = "realpassword";
    //        EXPECT_CALL(this->model, getUserObject((string) data["username"]))
    //                .Times(testing::AtLeast(1))
    //                .WillRepeatedly(Return(user))
    //                .RetiresOnSaturation();
    //    }
}


/**
 * @brief TEST_F Tests HTTP POST /request/delete-museum
 * @see HandlerTest::loginTest()
 * Needs login.
 * 8 Test Cases:
 * 1. Invalid POST data (no data).
 * 2. Invalid POST data (no username/password).
 * 3. Valid data, User not in DB.
 * 4. Valid data, User in DB, password mismatch.
 * 5. Logged in, Museum not in DB
 * 6. Logged in, Not curator.
 * 7. Logged in, Curator, Museum in DB.
 * 8. Logged in, Head Curator.
 */
TEST_F(HandlerTest, deleteMuseum) {
    int sleeptime = 300;
    string museumID = "2";
    string url = "/request/delete-museum/" + museumID;
    Response r;
    json data = {
        {"username", "abc"},
        {"password", "realpassword"}
    };
    /**< Cases 1-4*/
    this->loginTest(url);
    /**< Cases 4-*/
    User user(data["username"], "email@example.com", data["password"], 21);
    Museum museum("","","","", user, stoi(museumID));
    User curator("curator", "curator@example.com", "1234", 22);
    Museum otherMuseum("","","","", curator, stoi(museumID));
    {
        InSequence s;
        /**< Case 5 */
        EXPECT_CALL(this->model, getUserObject((string) data["username"]))
                .Times(testing::AtLeast(1))
                .WillRepeatedly(Return(user))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getMuseumObject(stoi(museumID)))
                .WillOnce(Throw(ModelException("Museum not found")))
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::Conflict);
        ASSERT_FALSE(json::parse(r.content)["success"]);

        /**< Case 6 */
        EXPECT_CALL(this->model, getUserObject((string) data["username"]))
                .Times(testing::AtLeast(1))
                .WillRepeatedly(Return(user))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getMuseumObject(stoi(museumID)))
                .WillOnce(Return(otherMuseum))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getHeadCurator())
                .WillOnce(Return(User("","","", 0)))
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::Unauthorized);
        ASSERT_FALSE(json::parse(r.content)["success"]);

        /**< Case 7 */
        EXPECT_CALL(this->model, getUserObject((string) data["username"]))
                .Times(testing::AtLeast(1))
                .WillRepeatedly(Return(user))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getMuseumObject(stoi(museumID)))
                .WillOnce(Return(museum))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getHeadCurator())
                .WillOnce(Return(User("","","", 0)))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, removeMuseumFromDB(museum))
                .Times(1)
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::OK);
        ASSERT_TRUE(json::parse(r.content)["success"]);

        /**< Case 8 */
        EXPECT_CALL(this->model, getUserObject((string) data["username"]))
                .Times(testing::AtLeast(1))
                .WillRepeatedly(Return(user))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getMuseumObject(stoi(museumID)))
                .WillOnce(Return(museum))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getHeadCurator())
                .WillOnce(Return(user))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, removeMuseumFromDB(museum))
                .Times(1)
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::OK);
        ASSERT_TRUE(json::parse(r.content)["success"]);

    }
}


/**
 * @brief TEST_F Tests HTTP POST /request/delete-artifact
 * @see HandlerTest::loginTest()
 * Needs login.
 * 8 Test Cases:
 * 1. Invalid POST data (no data).
 * 2. Invalid POST data (no username/password).
 * 3. Valid data, User not in DB.
 * 4. Valid data, User in DB, password mismatch.
 * 5. Logged in, Artifact not in DB.
 * 6. Logged in, Artifact in DB, Curator.
 * 7. Logged in, Artifact in DB, not Curator.
 */
TEST_F(HandlerTest, deleteArtifact) {
    int sleeptime = 300;
    string artifactID = "21232";
    string url = "/request/delete-artifact/" + artifactID;
    Response r;
    json data = {
        {"username", "abc"},
        {"password", "realpassword"}
    };
    /**< Cases 1-4*/
    this->loginTest(url);
    /**< Cases 4-*/
    User user(data["username"], "email@example.com", data["password"], 21);
    Museum museum("","","","", user, 1213);
    Artifact artifact("", "", "", "",  museum, stoi(artifactID));
    User curator("curator", "curator@example.com", "1234", 22);
    Museum otherMuseum("","","","", curator, 1213);
    Artifact otherArtifact("", "", "", "",  otherMuseum, stoi(artifactID));
    Edit<Artifact> edit(otherArtifact, Edit<Artifact>::del, user, {});
    {
        InSequence s;
        /**< Case 5 */
        EXPECT_CALL(this->model, getUserObject((string) data["username"]))
                .Times(testing::AtLeast(1))
                .WillRepeatedly(Return(user))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getArtifact(stoi(artifactID)))
                .WillOnce(Throw(ModelException("Artifact not found")))
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::Conflict);
        ASSERT_FALSE(json::parse(r.content)["success"]);

        /**< Case 6 */
        EXPECT_CALL(this->model, getUserObject((string) data["username"]))
                .Times(testing::AtLeast(1))
                .WillRepeatedly(Return(user))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getArtifact(stoi(artifactID)))
                .WillOnce(Return(artifact))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, removeArtifactInDB(artifact))
                .Times(1)
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::OK);
        ASSERT_TRUE(json::parse(r.content)["success"]);

        /**< Case 7 */
        EXPECT_CALL(this->model, getUserObject((string) data["username"]))
                .Times(testing::AtLeast(1))
                .WillRepeatedly(Return(user))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, getArtifact(stoi(artifactID)))
                .WillOnce(Return(otherArtifact))
                .RetiresOnSaturation();
        EXPECT_CALL(this->model, saveEditToDB(edit))
                .Times(1)
                .RetiresOnSaturation();

        usleep(sleeptime);
        r = this->requestTask(methods::POST, url, data);
        ASSERT_EQ(r.status, status_codes::OK);
        ASSERT_TRUE(json::parse(r.content)["success"]);
    }
}
