#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <iostream>
#include "../model/LafException.h"
#include "handler.h"

//using namespace web;
using namespace web::http;
using namespace utility;
using namespace web::http::experimental::listener;

template <class T>
class Controller
{
public:
    Handler<T> *g_httpHandler;
    Controller<T>(const string_t& address)  {
        T::initdb(CODE_BASE_DIRECTORY);
        uri_builder uri(address);
        auto addr = uri.to_uri().to_string();
        this->g_httpHandler = new Handler<T>(addr);
        this->g_httpHandler->open().wait();
        ucout << utility::string_t(U("Listening for requests at: ")) << addr << std::endl;
        ucout << "Press ENTER to exit." << std::endl;
    }
    ~Controller<T>() {
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
            Controller<T>::runServer(argv[1]);
        } else  {
            Controller<T>::runServer();
        }
        return 0;
    }

    static int runServer(std::string portString="5300") {
        Controller c(U("http://127.0.0.1:"+portString));
        std::string line;
        std::getline(std::cin, line);
        c.on_shutdown();
        return 0;
    }
};

#endif // CONTROLLER_H
