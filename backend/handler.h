#ifndef HANDLER_H
#define HANDLER_H
#define HELPER_H
#if defined(__GNUC__) || defined(__clang__)
    #define DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
    #define DEPRECATED __declspec(deprecated)
#else
    #pragma message("WARNING: You need to implement DEPRECATED for this compiler")
    #define DEPRECATED
#endif
#define BOOST_ASIO_HAS_STD_ATOMIC
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
//#include <execinfo.h>
#include <exception>
//#include "../model/modelclass.h"
#include "../model/ModelException.h"
#include "../model/collection.h"
#include "../model/user.h"
#include "../model/museum.h"
#include "../model/artifact.h"

#include "modelclassext.h"
#include "util.h"

#include "../nlohmann/json.hpp"
using json = nlohmann::json;

using namespace utility;                    // Common utilities like string conversions
//using namespace web; // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams
using namespace web::http::experimental::listener;


class Handler
{
public:

    /**
         * @brief Handler sets up the object adds support to GET,PUT,POST,DEL http requests.
         * @param url the url of the server
         */
    Handler(utility::string_t url, ModelClassExt *model, std::string codeBaseDirectory) : m_listener(url), model(nullptr) {
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
         * @brief open opens the listener callback method.
         * @return  callback method
         */
    pplx::task<void>open(){return m_listener.open();}
    pplx::task<void>close(){return m_listener.close();}


protected:

private:
    http_listener m_listener;
    ModelClassExt* model;
    std::string codeBaseDirectory;

    void handle_get(http_request);
    void returnFrontendFile(http_request);
    void returnWildCard(http_request);
    void returnMuseumList(http_request);
    void returnMuseumByID(http_request,int);
    void returnCollectionByID(http_request, int);
    void returnArtifactByID(http_request, int);
    void returnEditByID(http_request, int);

    void validateLogin(http_request);
    void getUserProfile(http_request);

    void handle_post(http_request);
    void addMuseum(http_request);
    void addUser(http_request);
    void addCollection(http_request);
    void reviewEdit(http_request);
    void deleteMuseum(http_request, int);
    void deleteArtifact(http_request, int);
    void handle_put(http_request);
    void handle_delete(http_request);

    void addEditArtifact(http_request message, int kind);
    void handle_error( http_request, pplx::task<void>& , std::string ="An error occured.");
};

#endif // HANDLER_H
