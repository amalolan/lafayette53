#ifndef MUSEUM_H
#define MUSEUM_H
#include <string>
#include <vector>
class Museum
{
public:
    Museum();
    std::string getAllMuseums();
private:
    struct mus{
        std::string name;
        std::string description;
        int id;
    };
    std::vector<mus> museums;
};


#endif // MUSEUM_H
