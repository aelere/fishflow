#include "moderator.h"

moderator::moderator(int user_id, const QString &username, const QString &first_name,
                     const QString &last_name, const QString &email,
                     const QString &date_of_registration, db_access &db)
    : user(user_id, username, first_name, last_name, email, date_of_registration),
      moderator_id(user_id),
      db(db)
{
}

bool moderator::shadowBanUser(int user_id) {
    // Error handling
    if (!isUserValid(user_id)) {
        qWarning() << "User with id" << user_id << "does not exist.";
        return false;
    }

    // Setting the users shadowbanned column to true
    QSqlQuery query(db.getDatabase());
    query.prepare("UPDATE Users SET shadowbanned = 1 WHERE user_id = :user_id");
    query.bindValue(":user_id", user_id);

    if (!query.exec()) {
        qCritical() << "Failed to shadowban user:" << query.lastError().text();
        return false;
    }

    return true;
}


bool moderator::unshadowBanUser(int user_id) {
    // Error handling
    if (!isUserValid(user_id)) {
        qWarning() << "User with id" << user_id << "does not exist.";
        return false;
    }

    // Setting the users shadowbanned column to false
    QSqlQuery query(db.getDatabase());
    query.prepare("UPDATE Users SET shadowbanned = 0 WHERE user_id = :user_id");
    query.bindValue(":user_id", user_id);

    if (!query.exec()) {
        qCritical() << "Failed to unshadowban user:" << query.lastError().text();
        return false;
    }

    return true;
}

bool moderator::putUserInJail(int user_id, int jail_duration, const QString &jail_reason) {
    // Error handling
    if (!isUserValid(user_id)) {
        qWarning() << "User with id" << user_id << "does not exist.";
        return false;
    }

    // Getting current date and time
    QString jail_start_timestamp = QDateTime::currentDateTime().toString(Qt::ISODate);

    QSqlQuery query(db.getDatabase());

    // Inserting user into the jail
    query.prepare("INSERT INTO UserJail (user_id, jail_start_timestamp, jail_duration, jail_reason) "
                  "VALUES (:user_id, :jail_start_timestamp, :jail_duration, :jail_reason)");
    query.bindValue(":user_id", user_id);
    query.bindValue(":jail_start_timestamp", jail_start_timestamp);
    query.bindValue(":jail_duration", jail_duration);
    query.bindValue(":jail_reason", jail_reason);

    if (!query.exec()) {
        qCritical() << "Failed to put user in jail:" << query.lastError().text();
        return false;
    }

    return true;
}

bool moderator::releaseUserFromJail(int user_id) {

    // Error handling
    if (!isUserValid(user_id)) {
        qWarning() << "User with id" << user_id << "does not exist.";
        return false;
    }

    // Removing user from jail
    QSqlQuery query(db.getDatabase());
    query.prepare("DELETE FROM UserJail WHERE user_id = :user_id");
    query.bindValue(":user_id", user_id);

    if (!query.exec()) {
        qCritical() << "Failed to remove user from jail:" << query.lastError().text();
        return false;
    }

    return true;
}

bool moderator::removePost(int post_id, const QString &removal_reason) {
    if (!isPostValid(post_id)) {
        qWarning() << "Post with id" << post_id << "does not exist.";
        return false;
    }

    // Database reference
    QSqlDatabase& db_instance = db.getDatabase();
    QSqlQuery query(db_instance);

    // Starting a transaction to ensure atomicity of post removal and logging
    db_instance.transaction();

    // Removing the post from the PostHashtags table
    query.prepare("DELETE FROM PostHashtags WHERE post_id = :post_id");
    query.bindValue(":post_id", post_id);

    if (!query.exec()) {
        qCritical() << "Failed to remove post from PostHashtags:" << query.lastError().text();
        db_instance.rollback(); // Rollback the transaction on error
        return false;
    }

    // Removing the post from the PostReports table
    query.prepare("DELETE FROM PostReports WHERE post_id = :post_id");
    query.bindValue(":post_id", post_id);

    if (!query.exec()) {
        qCritical() << "Failed to remove post from PostReports:" << query.lastError().text();
        db_instance.rollback(); // Rollback the transaction on error
        return false;
    }

    // Removing the post from the Posts table
    query.prepare("DELETE FROM Posts WHERE post_id = :post_id");
    query.bindValue(":post_id", post_id);

    if (!query.exec()) {
        qCritical() << "Failed to remove post:" << query.lastError().text();
        db_instance.rollback(); // Rollback the transaction on error
        return false;
    }

    // Logging the post removal
    QString removal_timestamp = QDateTime::currentDateTime().toString(Qt::ISODate);
    query.prepare("INSERT INTO RemovedPosts (post_id, moderator_id, removal_reason, removal_timestamp) VALUES (:post_id, :moderator_id, :removal_reason, :removal_timestamp)");
    query.bindValue(":post_id", post_id);
    query.bindValue(":moderator_id", moderator_id);
    query.bindValue(":removal_reason", removal_reason);
    query.bindValue(":removal_timestamp", removal_timestamp);

    if (!query.exec()) {
        qCritical() << "Failed to log post removal:" << query.lastError().text();
        db_instance.rollback(); // Rollback the transaction on error
        return false;
    }

    // Committing the transaction
    db_instance.commit();
    //emit postRemoved(post_id);
    return true;
}


bool moderator::removeUser(int user_id, const QString &removal_reason) {
    if (!isUserValid(user_id)) {
        qWarning() << "User with id" << user_id << "does not exist.";
        return false;
    }

    if (user_id == moderator_id) {
        qWarning() << "A user cannot remove themselves.";
        return false;
    }

    // Database reference
    QSqlDatabase& db_instance = db.getDatabase();
    QSqlQuery query(db_instance);

    // Starting a transaction to ensure atomicity of user removal and logging
    db_instance.transaction();

    // Removing the user from the Users table
    query.prepare("DELETE FROM Users WHERE user_id = :user_id");
    query.bindValue(":user_id", user_id);

    if (!query.exec()) {
        qCritical() << "Failed to remove user:" << query.lastError().text();
        db_instance.rollback(); // Rollback the transaction on error
        return false;
    }

    // Removing the user's data from every table except Converations.
    QStringList tables = {"Moderators", "UserInterests", "Authentication", "Notifications", "UserJail"};
    for (const QString& table : tables) {
        query.prepare(QString("DELETE FROM %1 WHERE user_id = :user_id").arg(table));
        query.bindValue(":user_id", user_id);

        if (!query.exec()) {
            qCritical() << "Failed to remove user data from table" << table << ":" << query.lastError().text();
            db_instance.rollback(); // Rollback the transaction on error
            return false;
        }
    }

    // Handling Conversations Separately.
    query.prepare("DELETE FROM Conversations WHERE user1_id = :user_id OR user2_id = :user_id");
    query.bindValue(":user_id", user_id);

    if (!query.exec()) {
        qCritical() << "Failed to remove user data from table Conversations:" << query.lastError().text();
        db_instance.rollback(); // Rollback the transaction on error
        return false;
    }

    // Messages are also seperate.
    query.prepare("DELETE FROM Messages WHERE sender_id = :user_id");
    query.bindValue(":user_id", user_id);

    if (!query.exec()) {
        qCritical() << "Failed to remove user data from table Messages:" << query.lastError().text();
        db_instance.rollback(); // Rollback the transaction on error
        return false;
    }

    // Removing each post
    query.prepare("SELECT post_id FROM Posts WHERE author_id = :user_id");
    query.bindValue(":user_id", user_id);

    if (!query.exec()) {
        qCritical() << "Failed to fetch user posts:" << query.lastError().text();
        db_instance.rollback(); // Rollback the transaction on error
        return false;
    }

    while (query.next()) {
        int post_id = query.value(0).toInt();
        if (!removePost(post_id, "User removed")) {
            qCritical() << "Failed to remove post with id" << post_id;
            db_instance.rollback(); // Rollback the transaction on error
            return false;
        }
    }

    // Log the user removal
    query.prepare("INSERT INTO RemovedUsers (user_id, moderator_id, removal_reason, removal_timestamp) "
                  "VALUES (:user_id, :moderator_id, :removal_reason, :removal_timestamp)");
    query.bindValue(":user_id", user_id);
    query.bindValue(":moderator_id", moderator_id);
    query.bindValue(":removal_reason", removal_reason);

    // Get the current timestamp and bind it to the query
    QDateTime current_timestamp = QDateTime::currentDateTime();
    QString timestamp_string = current_timestamp.toString("yyyy-MM-dd-hh-mm-ss");
    query.bindValue(":removal_timestamp", timestamp_string);

    if (!query.exec()) {
        qCritical() << "Failed to log user removal:" << query.lastError().text();
        db_instance.rollback(); // Rollback the transaction on error
        return false;
    }

    // Commit the transaction
    db_instance.commit();
    //emit userRemoved(user_id);
    return true;
}

// Remove a report from the database
bool moderator::removeReport(int report_id) {

    // Checking if report exists
    if (!isReportValid(report_id)) {
        qWarning() << "Report with id" << report_id << "does not exist.";
        return false;
    }

    // Database access
    QSqlQuery query(db.getDatabase());

    // Removing report from postReports table
    query.prepare("DELETE FROM PostReports WHERE report_id = :report_id");
    query.bindValue(":report_id", report_id);

    // Query excecution
    if (!query.exec()) {
        qCritical() << "Failed to remove report:" << query.lastError().text();
        return false;
    }

    return true;
}

// Check if a user is valid
bool moderator::isUserValid(int user_id) {
    // Database access
    QSqlQuery query(db.getDatabase());

    // Getting user from the users table
    query.prepare("SELECT user_id FROM Users WHERE user_id = :user_id");
    query.bindValue(":user_id", user_id);

    // Query execution
    if (!query.exec()) {
        qCritical() << "Failed to check user validity:" << query.lastError().text();
        return false;
    }

    return query.next();
}

// Check if a post is valid
bool moderator::isPostValid(int post_id) {

    // Database access
    QSqlQuery query(db.getDatabase());

    // Getting post_id from posts table
    query.prepare("SELECT post_id FROM Posts WHERE post_id = :post_id");
    query.bindValue(":post_id", post_id);

    // Query execution
    if (!query.exec()) {
        qCritical() << "Failed to check post validity:" << query.lastError().text();
        return false;
    }

    return query.next();
}


// Check if a report is valid
bool moderator::isReportValid(int report_id) {
    // Database access
    QSqlQuery query(db.getDatabase());

    // Getting report_id from PostReports table
    query.prepare("SELECT report_id FROM PostReports WHERE report_id = :report_id");
    query.bindValue(":report_id", report_id);

    if (!query.exec()) {
        qCritical() << "Failed to check report validity:" << query.lastError().text();
        return false;
    }

    return query.next();
}

// Get the moderator's ID
int moderator::getModeratorId() const{
    return moderator_id;
}
