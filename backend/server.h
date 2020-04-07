#ifndef SERVER_H
#define SERVER_H
#include <string>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <iostream>
#include "url.h"
#include <thread>
#include "museum.h"
class Server
{
public:
    Server();
    int run(int PORT);
    URL parseRequest(std::string *s);
    int runSocket(int new_socket);
    std::string rout(std::string req);
};

#endif // SERVER_H
