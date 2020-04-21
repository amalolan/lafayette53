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
                 {"id",1},
                 {"description", "description"},
                 {"name", "Collection title"},
                 {"introduction", "introduction"}
             }}
        };
        return colObj;
    };
    //This method should return museum with its own collection IDs
    static json getCollectionListByMuseumID(int musID){
        json colArray = json::array
        ({
            {
                {"id",1},
                {"description", "description for collection 1"},
                {"name", "collection N1"},
                {"introduction", "introduction for collection 1"}
            },
            {
                {"id",2},
                {"description", "description for collection 2"},
                {"name", "collection N2"},
                {"introduction", "introduction for collection 2"}
            }
        });
        return colArray;
    };
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
