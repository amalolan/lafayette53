#include <iostream>

#include "server.h"
#include "museum.h"
using namespace std;

int main()
{

    Museum *m = new Museum();
    std::cout<< m->getAllMuseums() << "\n";

    Server server;
    server.run(5300);
}
