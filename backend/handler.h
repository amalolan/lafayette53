#ifndef HANDLER_H
#define HANDLER_H
#define BOOST_ASIO_HAS_STD_ATOMIC
// Cpprestsdk libraries
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/http_listener.h>              // HTTP server
#include <cpprest/json.h>                       // JSON library
#include <cpprest/uri.h>                        // URI library
#include <cpprest/ws_client.h>                  // WebSocket client
#include <cpprest/containerstream.h>            // Async streams backed by STL containers
#include <cpprest/interopstream.h>
#include <cpprest/rawptrstream.h>               // Async streams backed by raw pointer to memory
#include <cpprest/producerconsumerstream.h>
#include <cpprest/http_listener.h>
#include "cpprest/json.h"
#include "cpprest/http_listener.h"
#include "cpprest/uri.h"
#include "cpprest/asyncrt_utils.h"
#include "cpprest/json.h"
#include "cpprest/filestream.h"
#include "cpprest/containerstream.h"
#include "cpprest/producerconsumerstream.h"
// QT Libraries
#include <QDirIterator>
#include <QCryptographicHash>
// Other libraries
#include <exception>
#include "../nlohmann/json.hpp"
#include <chrono>
// Model classes
#include "../model/modelclass.h"
#include "../model/ModelException.h"
#include "../model/collection.h"
#include "../model/user.h"
#include "../model/museum.h"
#include "../model/artifact.h"
// Other backend classes
#include "util.h"
// json library for convenience
using json = nlohmann::json;

using namespace utility;                    // Common utilities like string conversions
// Cpprestsdk namespaces
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams
using namespace web::http::experimental::listener;

/**
 * @brief The Handler class Responsible for listening and responding to HTTP requests.
 * All GET, PUT, POST, and DEL HTTP requests that the server recieves are sent to the associated Handler object.
 *
 * All requests are handled asynchronously.
 */
class Handler
{
public:

    /**
         * @brief Handler sets up the object adds support to GET,PUT,POST,DEL http requests.
         * @param url the url of the server
         */
    Handler(utility::string_t url, ModelClass *model, std::string codeBaseDirectory) : m_listener(url), model(nullptr) {
        this->model = model;
        this->codeBaseDirectory =  codeBaseDirectory;
        m_listener.support(methods::GET, std::bind(&Handler::handle_get, this, std::placeholders::_1));
        m_listener.support(methods::PUT, std::bind(&Handler::handle_put, this, std::placeholders::_1));
        m_listener.support(methods::POST, std::bind(&Handler::handle_post, this, std::placeholders::_1));
        m_listener.support(methods::DEL, std::bind(&Handler::handle_delete, this, std::placeholders::_1));
    }

    /**
         * @brief ~Handler destructor.
         */
    virtual ~Handler() {
    }
    /**
         * @brief open opens the listener asynchronously and starts listening to HTTP requests.
         * @return a task object that opens the listener.
         */
    pplx::task<void>open(){return m_listener.open();}
    /**
     * @brief close closes the listener asynchronously and stops listening to HTTP requests.
     * @return a task object that closes the listener.
     */
    pplx::task<void>close(){return m_listener.close();}


protected:

private:
    http_listener m_listener;  /**< the object that listens to HTTP requests.*/
    ModelClass* model; /**< the ModelClass which is associated with this handler. Performs database operations.*/
    std::string codeBaseDirectory; /**< The directory of all the source code. */

    /**
     *  @brief handle_error This function is used to catch all runtime server errors.
         * If an error is caught, a failure response is sent accompanied by a JSON object with the error message.
         *
         * The schema is as follows:
         * <code>
         * { <br>
         * success: false if error, <br>
         * message: string (error message) <br>
         * }
         * </code>
     * @param message the message to respond to
     * @param t the asynchronous task object that we are expecting to succeed/fail
     * @param error the error message
     */
    void handle_error(http_request, pplx::task<void>& , std::string ="An error occured.");

    /**
     * @brief handle_get Handles GET HTTP requests. Depending on the url, calls its correspoding helper function in Handler.
     * @param message the GET request.
     * @result sends either an HTML file or A stringified JSON object.
     */
    void handle_get(http_request message);

    /**
     * @brief returnFrontendFile Used to return files to display on the frontend from the frontend/ folder.
     * @param message the GET request containing the file's name.
     * @result the file requested is sent back to the HTTP request origin as an HTML response.
     */
    void returnFrontendFile(http_request message);

    /**
     * @brief returnWildCard If an unknown file/url is GET requested, this function is called and it returns the index.html which displays a 404 error message
     * @param message the GET request containing the invalid  url.
     * @result the index.html file with 404 error.
     */
    void returnWildCard(http_request message);

    /**
     * @brief returnMuseumList Helper for responding to GET /request/museum-list Returns all the museums in the websites.
     * @param message the GET request.
     * @result A stringified json object is returned to the source. Schema as follows <br>
     * <code>
     * [array of museum objects: {id: int, name: string, description:string, introduction: string, image: string} ... ]
     * </code>
     *
     */
    void returnMuseumList(http_request message);

    /**
     * @brief returnMuseumByID Helper for responding to GET /request/museum/[museumID] returns the musuem object associated with that ID.
     * @param message the GET request.
     * @param museumID the ID of the museum requested.
     * @result A stringified json object is returned to the source. Schema as follows <br>
     * <code>
     * { <br>
     *    collectionList: [array of collection objects: {id: int, name: string, description:string, introduction: string, image: string} ... ],<br>
     *    museum: museum object @see returnMuseumList() <br>
     * } <br>
     * </code>
     */
    void returnMuseumByID(http_request message,int museumID);

    /**
     * @brief returnCollectionByID Helper for responding to GET /request/collection/[collectionID] returns the collection object associated with that ID.
     * @param message the GET request.
     * @param collectionID the ID of the collection requested.
     * @result A stringified json object is returned to the source. Schema as follows <br>
     * <code>
     * { <br>
     *    artifactList: [array of artifact objects: {id: int, name: string, description:string, introduction: string, image: string} ... ],<br>
     *    collection: collection object, @see returnMuseumByID() <br>
     *    museum: {id: int, name: string} <br>
     * } <br>
     * </code>
     *
     */
    void returnCollectionByID(http_request message, int collectionID);

    /**
     * @brief returnArtifactByID Helper for responding to GET /request/artifact/[artifactID] returns the artifact object associated with that ID.
     * @param message the GET request.
     * @param artifactID the ID of the artifact requested.
     * @result A stringified json object is returned to the source. Schema as follows <br>
     * <code>
     * { <br>
     *    artifactList: artifact object, @see returnCollectionByID() <br>
     *    collectionList: [array of {id: int, name:string} ... ], <br>
     *    museum: {id: int} <br>
     * } <br>
     * </code>
     *
     */
    void returnArtifactByID(http_request message, int artifactID);
    /**
     * @brief returnEditByID Helper for responding to GET /request/edit/[editID] returns the edit object associated with that ID.
     * @param message the GET request.
     * @param editID the ID of the edit requested.
     * @result A stringified json object is returned to the source. Schema for an artifact edit and collection edit provided in Util.
     * @see Util::getArtifactEditJSON()  @see Util::getCollectionEditJSON()
     */
    void returnEditByID(http_request message, int editID);

    /**
     * @brief handle_get Handles POST HTTP requests. Depending on the url, calls its correspoding helper function in Handler.
     * @param message the POST request with an accompanying JSON object containing data.
     * @result sends a success or failure code accompanied by a JSON object that is usually a JSON object containing a success and failure message (except for getUserProfile() which returns other data).
     * @see Util::getFailureJsonStr()  @see Util::getSuccessJsonStr()
     */
    void handle_post(http_request message);


    /**
     * @brief getUserProfile Helper for responding to GET /request/user-profile returns the data for associated with that user.
     *
     * Requires log in authorization. If not logged in, an Backend Exception is raised and an Unauthorized status message is sent with a failure JSON. @see Util::getFailureJsonStr().

     * Once Logged in, it returns a list of museums the user created, a list of actions he can take on suggested edits by editors on his museums, and a list of edits he suggested for other museums.
     *
     * If the user is the head curator, all museums are returned in a separate table where he has the option to delete some museums if he chooses to.
     * @param message the POST request with an accompanying JSON object containing data.
     * @arg The JSON object. Schema follows: <br>
     * <code>
     * A user input object which is <br>
     * {
     *    username: string, <br>
     *    password: string <br>
     * } <br>
     * </code>
     * @result A stringified json object is returned to the source. Schema as follows <br>
     * <code>
     * { <br>
     *    user: {id: int, username:string, email: string}, <br>
     *    actionsList: [array of unacted edit objects ], <br> @see Util::getArtifactEditJSON()  @see Util::getCollectionEditJSON()
     *    editsList: [array of edit objects], <br>
     *    museumList: [array of museums of {id: int, name: string} ... ], <br>
     *    headCuratorList: (only present if user is head curator) [array of all museums {id: int, name: string}] <br>
     * } <br>
     * </code>
     */
    void getUserProfile(http_request message);

    /**
     * @brief validateLogin Helper for responding to POST /request/login authorises a user's login.
     * @param message the POST request with an accompanying JSON object containing data.
     * @arg JSON data for the post request is the user input object. @see getUserProfile()
     */
    void validateLogin(http_request message);

    /**
     * @brief addUser Helper for responding to POST /request/register registers a user.
     *
     * A confirmation email is sent. @see Util::sendEmail()
     * @param message the POST request with an accompanying JSON object containing data.
     * @arg The JSON object. Schema follows: <br>
     * <code>
     * A user input object which is <br>
     * {
     *    username: string, <br>
     *    password: string, <br>
     *    email: string <br>
     * } <br>
     * </code>
     * @result success or failure JSON is sent. @see handle_post()
     */
    void addUser(http_request message);

    /**
     * @brief changePassword Helper for responding to POST /request/reset-password changes the password for a user who forgot his password and assigns him a new random password.
     *
     *  The password is sent to the user's email. @see Util::sendEmail()
     * @param message the POST request with an accompanying JSON object containing data.
     * @arg Schema follows <br>
     * <code>
     * { <br>
     * username: string
     * } <br>
     * </code>
     * @result success or failure JSON is sent. @see handle_post()
     */
    void changePassword(http_request message);


    /**
     * @brief addMuseum Helper for responding to POST /request/add-museum adds a museum to the database.
     *
     * Requires log in authorization. If not logged in, an Backend Exception is raised and an Unauthorized status message is sent with a failure JSON.
     * @param message the POST request with an accompanying JSON object containing data.
     * @arg Schema follows <br>
     * <code>
     * { <br>
     * user: user object, <br> @see getUserProfile()
     * museum: museum object with id = null, <br> @see returnMuseumList()
     * } <br>
     * </code>
     * @result success or failure JSON is sent. @see handle_post()
     */
    void addMuseum(http_request message);

    /**
     * @brief deleteMuseum Helper for responding to POST /request/delete-museum/[museumID] deletes a museum from the database.
     *
     * Requires log in authorization. If not logged in, an Backend Exception is raised and an Unauthorized status message is sent with a failure JSON.
     *
     * ID of museum to be deleted is specified in the URL.
     * @param message the POST request with an accompanying JSON object containing data.
     * @arg Schema follows <br>
     * <code>
     * { <br>
     * user: user object, <br> @see getUserProfile()
     * } <br>
     * </code>
     * @result success or failure JSON is sent. @see handle_post()
     */
    void deleteMuseum(http_request message, int museumID);

    /**
     * @brief addEditCollection Helper for responding to POST /request/add-collection and /request/edit-collection which adds/edits a collection in the database.
     *
     * Requires log in authorization. If not logged in, an Backend Exception is raised and an Unauthorized status message is sent with a failure JSON.
     * @param message the POST request with an accompanying JSON object containing data.
     * @arg Schema follows <br>
     * <code>
     * { <br>
     * user: user object, <br> @see getUserProfile()
     * collection: collection object (ID present for edit, not for add), <br> @see returnMuseumByID()
     * museum: {id: int} <br>
     * } <br>
     * </code>
     * @result success or failure JSON is sent. @see handle_post()
     */
    void addEditCollection(http_request message, int collectionID);

    /**
     * @brief addEditArtifact Helper for responding to POST /request/add-artifact and /request/edit-artifact which adds/edits an artifact in the database.
     * Since each artifact can be associated with many collections, a list of collections selected by the user to be associated with the artifact is alos sent in the request.
     *
     * Requires log in authorization. If not logged in, an Backend Exception is raised and an Unauthorized status message is sent with a failure JSON.
     * @param message the POST request with an accompanying JSON object containing data.
     * @arg Schema follows <br>
     * <code>
     * { <br>
     * user: user object, <br> @see getUserProfile()
     * artifact: artifact object, <br> @see returnCollectionByID() <br>
     * museum: {id: int}, <br>
     * collection: [int array of collectionsIDs]  <br>
     * } <br>
     * </code>
     * @result success or failure JSON is sent. @see handle_post()
     */
    void addEditArtifact(http_request message, int artifactID);

    /**
     * @brief deleteArtifact Helper for responding to POST /request/delete-artifact/[artifactID] deletes an artifact from the database.
     *
     * Requires log in authorization. If not logged in, an Backend Exception is raised and an Unauthorized status message is sent with a failure JSON.
     *
     * ID of artifact to be deleted is specified in the URL.
     * @param message the POST request with an accompanying JSON object containing data.
     * @arg Schema follows <br>
     * <code>
     * { <br>
     * user: user object, <br> @see getUserProfile()
     * } <br>
     * </code>
     * @result success or failure JSON is sent. @see handle_post()
     */
    void deleteArtifact(http_request message, int artifactID);

    template <typename T>
    /**
     * @brief reviewEdit Helper function for actOnEdit(). Since Edits can be of different types, this templated function was necessary to avoid code repetition.
     * Depending on the curators action (approval/denial) the database's respective methods called and the edit object is approved/denied and its state is updated in the database.
     *
     * Requires login authorization and also requires the acting user to be the museum's curator. If this isn't the case, a BackendException is raised and an Unauthorized status message is sent with a failure JSON.
     * @param editObject The edit object to set as reviewed. Could be Edit<Artifact> or Edit<Collection>
     * @param action bool true if approved, false if denied.
     * @param curator the user that is performing the review action.
     * @return the success message if no exception was raised.
     */
    std::string reviewEdit(Edit<T> editObject, bool action, User user);

    /**
     * @brief addEditArtifact Helper for responding to POST /request/add-artifact and /request/edit-artifact which adds/edits an artifact in the database.
     * Since each artifact can be associated with many collections, a list of collections selected by the user to be associated with the artifact is alos sent in the request.
     *
     * Requires log in authorization. If not logged in, an Backend Exception is raised and an Unauthorized status message is sent with a failure JSON.
     * @param message the POST request with an accompanying JSON object containing data.
     * @arg Schema follows <br>
     * <code>
     * { <br>
     * user: user object, <br> @see getUserProfile()
     * artifact: artifact object, <br> @see returnCollectionByID() <br>
     * museum: {id: int}, <br>
     * collection: [int array of collectionsIDs]  <br>
     * } <br>
     * </code>
     * @result success or failure JSON is sent. @see handle_post()
     */
    void actOnEdit(http_request message);

    /**
     * @brief handle_get Handles PUT HTTP requests. No PUT requests are implemented, so this should return a 404 error with a failure JSON.
     * @param message the PUT request.
     * @result 404 error with failure JSON. @see Util::getFailureJsonStr()
     */
    void handle_put(http_request message);

    /**
     * @brief handle_get Handles DEL HTTP requests. No DEL requests are implemented, so this should return a 404 error with a failure JSON.
     * @param message the DEL request.
     * @result 404 error with failure JSON. @see Util::getFailureJsonStr()
     */
    void handle_delete(http_request message);

};

#endif // HANDLER_H
