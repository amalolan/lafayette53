#include "museum.h"

Museum::Museum()
{
    mus m1;
    m1.id = 1;
    m1.name = "Museum Number 1!";
    m1.description = "Really interesting stuff";
    museums.push_back(m1);
    mus m2;
    m2.id = 2;
    m2.name = "Museum Number 2";
    m2.description = "Interesting stuff";
    museums.push_back(m2);
}

std::string Museum::getAllMuseums(){

}
