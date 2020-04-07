#include <iostream>

#include "server.h"
#include "museum.h"
#include "../model/modelclass.h"
using namespace std;

int main()
{

    Museum *m = new Museum();
    //std::cout<< m->getAllMuseums() << "\n";
    ModelClass::initdb();
    std::cout<< ModelClass::getMuseumListJSON() << "\n";
    Server server;
    server.run(5300);
}
