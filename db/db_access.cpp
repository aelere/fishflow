#include "db_access.h"


db_access::db_access() {
    QString path("../../database/project_database.sqlite");  // Setting path

    // Using an unique connection name for each instance of db_access
    QString connectionName = QString("conn_%1").arg(reinterpret_cast<quintptr>(this), QT_POINTER_SIZE * 2, 16, QChar('0'));

    m_db = QSqlDatabase::addDatabase("QSQLITE", connectionName);

    m_db.setDatabaseName(path); // Givving the path to the database

    if (!m_db.open()) {
        qCritical() << "Error: connection with database fail";   // If database is not opened, throw error and exit
        exit(1);
    } else {
        qDebug() << "Database: connection ok";
    }
}


QSqlDatabase& db_access::getDatabase(){
    return m_db;
}

db_access::~db_access(){
    QString connectionName = m_db.connectionName();
    m_db.close();

    QSqlDatabase::removeDatabase(connectionName);
}
