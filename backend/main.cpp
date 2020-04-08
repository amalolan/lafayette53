#include <iostream>

#include "server.h"
#include "../model/modelclass.h"
using namespace std;

int main()
{

    //std::cout<< m->getAllMuseums() << "\n";
    ModelClass::initdb();
    //std::cout<< ModelClass::getMuseumListJSON() << "\n";
    Server server;
    server.run(5300);
}
