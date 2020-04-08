#ifndef MODELCLASS_H
#define MODELCLASS_H
#include <QtSql>
#include <QDebug>
#include <QFileInfo>
class ModelClass
{
public:
    static void initdb();
    static bool open();
    static bool close();
    static bool status();
    static std::string getMuseumListJSON();
    static std::string getUserInfoJSON(int userID);
    static std::string getMuseumInfoJSON(int museumID);

private:
    static QSqlDatabase db;
    static QSqlQuery query;
};

#endif // MODELCLASS_H
