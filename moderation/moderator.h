#ifndef MODERATOR_H
#define MODERATOR_H

#include "../authentication_engine/user.h"
#include "../db/db_access.h"
#include "../post/one_post.h"
#include <QObject>
#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <string>


// Class: moderator
// ----------------
// This class represents a moderator in the system. It is derived from user
// and provides additional functionality specific to moderators. A moderator
// can remove posts, shadowban users, put users in jail, and handle reports.
//  The class also contains helper functions to validate user, post, and report IDs.
class moderator : public user, public QObject {
public:
    // Constructor for the moderator class
    moderator(int user_id, const QString &username, const QString &first_name,
              const QString &last_name, const QString &email,
              const QString &date_of_registration, db_access &db);

    // Default constructor for the moderator class
    moderator();

    // Method that gets the moderator's ID
    int getModeratorId() const;

    // Method thar removes a post with the specified post ID and removal reason
    bool removePost(int post_id, const QString &removal_reason);

    // Method that shadowbans a user with the specified user ID
    bool shadowBanUser(int user_id);

    // Method that removes the shadowban on a user with the specified user ID
    bool unshadowBanUser(int user_id);

    // Method that puts a user with the specified user ID in jail for the given duration and reason
    bool putUserInJail(int user_id, int jail_duration, const QString &jail_reason);

    // Method that releases a user with the specified user ID from jail
    bool releaseUserFromJail(int user_id);

    // Method that removes a user with the specified user ID and removal reason
    bool removeUser(int user_id, const QString &removal_reason);

    // Method that removes a report with the specified report ID
    bool removeReport(int report_id);

private:
    int moderator_id;  // The ID of the moderator
    int user_id;       // The ID of the user
    db_access &db;     // Reference to the database access object

    // Helper function to check if a user with the given user ID is valid
    bool isUserValid(int user_id);

    // Helper function to check if a post with the given post ID is valid
    bool isPostValid(int post_id);

    // Helper function to check if a report with the given report ID is valid
    bool isReportValid(int report_id);
};


#endif // MODERATOR_H
