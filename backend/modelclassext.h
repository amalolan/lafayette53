#ifndef MODELCLASSEXT_H
#define MODELCLASSEXT_H
#include <../model/modelclass.h>
#include <../model/ModelException.h>
#include <../model/collection.h>
#include "../nlohmann/json.hpp"
using json=nlohmann::json;


class ModelClassExt : public ModelClass
{
public:
    static void initdb(std::string codeBaseDirectory);
    static void saveCollectionToDB(Collection *);
    static json getCollectionInfoJSON(int colID){

        json colObj = {
            {"museum",{
                 {"id",colID},
                 {"name", "testMuseum"}
             }},
            {"collection",{
                 {"id","1"},
                 {"description", "description"},
                 {"title", "Collection title"},
                 {"introduction", "introduction"}
             }}
        };
        return colObj;
    };
    static json getMuseumAndCollectionInfoJSON(int);
    //you already have string for following methods. just return json.
    static json getMuseumInfoJson(int musID){
        json musObj = {
            {"id", musID},
            {"name", "testMuseum"},
            {"description", "testMuseum Description"},
            {"userID", 2}

        };
        return musObj;
    };
    static json getUserInfoJson(std::string username){
        json userObj = {
            {"username", "testUser"},
            {"password", "test"},
            {"id", 2}
        };
        return userObj;
    }
};
#endif // MODELCLASSEXT_H
