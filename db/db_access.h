#ifndef DB_ACCESS_H
#define DB_ACCESS_H

#include <iostream>
#include <QSqlDatabase>
#include <QDir>

using namespace std;

/**
 * @brief The db_access class enables other classes to access the sqlite database.
 */
class db_access
{
public:
    db_access();

    ~db_access();

    // Getter
    QSqlDatabase& getDatabase();

    //void dropTables();

    //void createTables();

private:

    // The database itself.
    QSqlDatabase m_db;
};

#endif // DB_ACCESS_H
