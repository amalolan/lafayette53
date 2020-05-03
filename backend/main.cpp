#include <iostream>
#include "modelclassext.h"
#include "controller.h"
#include "sha512.h"

int main(int argc, char *argv[])
{
    ucout << sha512("lexo2001") << '\n';
    Controller::runServer(argc, argv);
    return 0;
}
