#ifndef AUTH_MANAGER_H
#define AUTH_MANAGER_H

#include "user.h"
#include "../db/db_access.h"
#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QCryptographicHash>

class auth_manager {
public:
    auth_manager(db_access& dbAccess);

    // Method setting the current user to an already registered user.
    bool authenticate(const QString& username, const QString& password);

    // Method creating a user with automatic insertion into the database
    bool createUser(const QString& username, const QString& first_name, const QString& last_name,
                    const QString& email, const QString date, const QString& password);
    // Method creating a user with manual insertion into the database
    bool createUser(const QString& username, const QString& first_name, const QString& last_name,
                    const QString& email, const QString date, const QString& password, int id);

    // Method updating the current user.
    bool updateUser(const QString& username);

    // Method removing (not logged) a user from the database (better version implemented in moderator).
    bool removeUser(int id);

    // Method responsible for logging out.
    void logout();

    // Method that enables users to change their password.
    bool changePassword(const QString& username, const QString& newPassword);


    // Getter for user
    user* getCurrentUser();

    // Getter for user ID.
    int getUserId(QString username);

    // Method combining user interests with IDs.
    bool addUserInterest(int id, QString interest);

//    ~auth_manager(){
//        delete currentUser;
//    }

    bool changeUserInformation(const QString& username, const QString& first_name, const QString& last_name,
                               const QString& email, int id);

    bool clearIntersts(int user_id);

    //bool isBanned(const QString& username);


private:

    // Database access object.
    db_access& m_dbAccess;

    // Active user.
    user* currentUser;

    friend class conversation;
    friend class upload;
    friend class user_feed;
};

#endif // AUTH_MANAGER_H

