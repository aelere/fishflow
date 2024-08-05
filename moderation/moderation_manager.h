#ifndef MODERATION_MANAGER_H
#define MODERATION_MANAGER_H

#include <iostream>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QCryptographicHash>
#include "../db/db_access.h"
#include "moderator.h"

// Class: moderation_manager
// -------------------------
// This class manages the moderation actions & moderators.
// It can assign moderators, verify their existence,
// authenticate them, retrieve information related to moderation.
// The class interacts with a database through the db_access object.

class moderation_manager
{
public:
    // Constructor that initializes the moderation_manager with a database access object
    moderation_manager(db_access& dbAccess);

    // Method that assigns a moderator with the specified user ID
    bool assignModerator(int user_id);

    // Method that checks if a moderator with the specified user ID exists
    bool checkModerator(int user_id);

    // Method that checks if a moderator with the specified username exists
    bool checkModerator(QString username);

    // Method that authenticates a moderator with the specified username and password
    bool authenticateModerator(QString username, QString password);

    // Method that logs out the current moderator
    void logout();

    // Method that updates the moderator with the specified username
    bool updateModerator(QString username);

    // Method that returns all reports as a vector of vectors of strings
    std::vector<std::vector<std::string>> all_reports();

    // Method that gets the content of a post with the specified post ID
    QString getPostContent(int post_id);

    // Method that returns the current moderator
    moderator* getCurrentModerator();

private:
    db_access& m_dbAccess;    // Reference to the database access object
    moderator* currentModerator;  // Pointer to the current moderator
};



#endif // MODERATION_MANAGER_H

