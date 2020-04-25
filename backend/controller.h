#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <iostream>
#include "handler.h"
#define BOOST_ASIO_HAS_STD_ATOMIC

//using namespace web;
using namespace web::http;
using namespace utility;
using namespace web::http::experimental::listener;

/**
 * @brief The Controller class Is the Server. When initialized, it starts listening to
 * http requests at the provided address. Requests are responded to through the Handler object.
 * When the server is stopped (by pressed enter), the handler is closed, and the server is shutdown.
 *
 *
 * To create a server instance, it is sufficient to create the Controller instance.
 *
 * To automatically create and run a server, simply call Controller:runServer()
 *
 *
 * A server will be set up at http://127.0.0.1:5300.
 *
 * If the port is being used, the server will crash. Provide it a different port with the same command.
 */
class Controller
{
public:
    Handler *g_httpHandler;
    /**
     * @brief Controller Setus up the server. Starts listening to http requests at
     * address. Also opens up a database by initializing the ModelClassExt
     * @param address string [usually "http://127.0.0.1:5300" ]
     * @param model The database/ModelClass object associated with this Controller object.
     */
    Controller(const string_t& address, ModelClassExt *model)  {
        uri_builder uri(address);
        auto addr = uri.to_uri().to_string();
        this->g_httpHandler = new Handler(addr, model);
        this->g_httpHandler->open().wait();
        ucout << utility::string_t(U("Listening for requests at: ")) << addr << std::endl;
        ucout << "Press ENTER to exit." << std::endl;
    }

    ~Controller() {
    }

    /**
     * @brief on_shutdown All existing  requests are proceessed and then the handler stops
     * listening for requests.
     */
    void on_shutdown()
    {
        this->g_httpHandler->close().wait();
        delete this->g_httpHandler;
        return;
    }

    /**
     * @brief runServer Runs the server at the port in the argument provided on the command line.
     * If nothing is provided, it runs it on the port 5300. If  the port is currently being used,
     * the server WILL CRASH. Ensure the port is free before running this.
     * http://127.0.0.1:5300 is the default address.
     * @param argc int main(argc, *argv[]) This is the first of the two arguments to main()
     * @param argv int main(argc, *argv[]) This is the second of the two arguments to main()
     * @return 0
     */
    static int runServer(int argc, char *argv[])
    {
        if(argc == 2)
        {
            Controller::runServer(argv[1]);
        } else  {
            Controller::runServer();
        }
        return 0;
    }

    /**
     * @brief runServer Runs the server at the port portString. If  the port is currently being used,
     * the server WILL CRASH. Ensure the port is free before running this.
     * @param portString the port to run the server on. By default, it is 5300.
     * @return 0
     */
    static int runServer(std::string portString="5300") {
        ModelClassExt *model = new ModelClassExt(std::string(CODE_BASE_DIRECTORY) + "database/db.db");
        Controller c(U("http://127.0.0.1:"+portString), model);
        std::string line;
        std::getline(std::cin, line);
        c.on_shutdown();
        delete model;
        return 0;
    }
};

#endif // CONTROLLER_H
