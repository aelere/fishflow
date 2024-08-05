#ifndef USER_H
#define USER_H

#include <QString>
#include "../db/db_access.h"

/**
 * @brief The user class
 *
 * The user class represents the currently active (logged in) user. The user is allowed to create and remove
 * their posts. The user views the general feed, and has the option to report innapropriate content.
 */
class user {
public:

    // User constructor.
    user(int id, const QString &username, const QString &firstName, const QString &lastName,
         const QString &email, const QString &dateOfRegistration);

    // Empty constructor.
    user();

    // Method that adds post created by the user to the database.
    //bool addPost(const QString& title, const QString& contents, const QString& timestamp);

    // Method that removes (not logged) a post from the database (better version in moderator).
    bool removePost(int postId);

    // Method responsible for reporting.
    bool reportPost(int postId, const QString & reason, const QString& timestamp);

    // Method counting the amount of posts done by the user.
    int getPostCount();

    // Getters.
    int getId() const;
    QString getUsername() const;
    QString getFirstName() const;
    QString getLastName() const;
    QString getEmail() const;
    QString getDateOfRegistration() const;
    std::vector<std::string> getInterests();

private:
    int m_id;
    QString m_username;
    QString m_firstName;
    QString m_lastName;
    QString m_email;
    QString m_dateOfRegistration;

    // Database access object.
    db_access m_dbAccess;
};

#endif // USER_H
