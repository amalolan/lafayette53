#ifndef HANDLER_H
#define HANDLER_H
#define HELPER_H
#ifdef __APPLE__
#define CODE_BASE_DIRECTORY "../../../../../lafayette53/"
#elif __linux
#define CODE_BASE_DIRECTORY "../../lafayette53/"
#endif
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
#include <QDirIterator>
#include "../model/ModelException.h"
//#include "../model/modelclass.h"
#include "util.h"
#include <exception>
using namespace utility;                    // Common utilities like string conversions
//using namespace web; // Common features like URIs.
//using http_request = web::http_request;
//using http = web::http;
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams
using namespace web::http::experimental::listener;
template < class T >
class Handler
{
public:
    /**
         * @brief Handler
         */
    Handler() {
    }
    /**
         * @brief Handler sets up the object adds support to GET,PUT,POST,DEL http requests.
         * @param url the url of the server
         */
    Handler(utility::string_t url);
    /**
         * @brief ~Handler destructor.
         */
    virtual ~Handler() {

    }
    /**
         * @brief open opens the listener callback method.
         * @return  callback method
         */
    pplx::task<void>open(){return m_listener.open();}
    pplx::task<void>close(){return m_listener.close();}

protected:

private:
    void handle_get(http_request message);
    void returnFrontendFile(http_request message);
    void returnMuseumList(http_request message);
    void returnMuseumById(http_request message,int musId);
    void returnUserById(http_request message,int usrId);
    void returnCollectionById(http_request message, int colId);
    void returnWildCard(http_request message);

    void handle_put(http_request message);
    void validateLogin(http_request message);
    void getUserProfile(http_request message);

    void handle_post(http_request message);
    void addMuseum(http_request message);
    void addUser(http_request message);
    void addCollection(http_request message);

    void handle_delete(http_request message);
    void handle_error( http_request message, pplx::task<void>& t, std::string error="ERROR");
    http_listener m_listener;
};

#include "handler.tpp"
#endif // HANDLER_H
