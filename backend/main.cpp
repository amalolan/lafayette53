#include <iostream>
#include "../model/LafException.h"
#include "handler.h"
#include "../model/modelclass.h"
//#include "../model/modelclass.cpp"
using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;




Handler<ModelClass> *g_httpHandler;

void on_initialize(const string_t& address)
{

    uri_builder uri(address);

    auto addr = uri.to_uri().to_string();
    g_httpHandler = new Handler<ModelClass>(addr);
    g_httpHandler->open().wait();

    ucout << utility::string_t(U("Listening for requests at: ")) << addr << std::endl;

    return;
}

void on_shutdown()
{
    g_httpHandler->close().wait();
    delete g_httpHandler;
    return;
}
void printallDir(){

    QDirIterator dirIt("../../lafayette53/frontend",QDirIterator::NoIteratorFlags);
    while(dirIt.hasNext()){
        QString s =dirIt.fileName();
        ucout << s.toStdString() << "\n";
        dirIt.next();
    }
}
int main(int argc, char *argv[])
{

    ModelClass::initdb();
    utility::string_t port = U("5300");
    if(argc == 2)
    {
        port = argv[1];
    }

    utility::string_t address = U("http://127.0.0.1:");
    address.append(port);

    on_initialize(address);
    std::cout << "Press ENTER to exit." << std::endl;

    std::string line;
    std::getline(std::cin, line);

    on_shutdown();
    return 0;
}
