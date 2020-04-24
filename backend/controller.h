#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <iostream>
#include "handler.h"
#define BOOST_ASIO_HAS_STD_ATOMIC

//using namespace web;
using namespace web::http;
using namespace utility;
using namespace web::http::experimental::listener;

class Controller
{
public:
    Handler *g_httpHandler;
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

    void on_shutdown()
    {
        this->g_httpHandler->close().wait();
        delete this->g_httpHandler;
        return;
    }

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
