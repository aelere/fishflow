#include "auth_manager.h"

auth_manager::auth_manager(db_access& dbAccess)
    : m_dbAccess(dbAccess) {
}

bool auth_manager::authenticate(const QString& username, const QString& password) {
    QSqlDatabase& db = m_dbAccess.getDatabase();
    QSqlQuery query(db);

    //Fetching the password hash from the database to compare it to entry.
    query.prepare("SELECT password_hash FROM Authentication WHERE user_id = (SELECT user_id FROM Users WHERE username = :username)");
    query.bindValue(":username", username);

    //error handling
    if (!query.exec()) {
        qCritical() << "Failed to execute query:" << query.lastError().text();
        return false;
    }

    //query only has one output
    if (query.next()) {
        //stored password hash
        QString storedPasswordHash = query.value(0).toString();

        //converting user-given password to a password hash
        QString passwordHash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex();

        updateUser(username);

        return storedPasswordHash == passwordHash;  //if they match, return false. Else, return true.
    } else {
        // No matching user found
        return false;
    }
}

bool auth_manager::createUser(const QString& username, const QString& first_name, const QString& last_name,
                              const QString& email, const QString date_of_registration, const QString& password, int id) {
    QSqlDatabase& db = m_dbAccess.getDatabase();
    QSqlQuery query(db);

    // Begin a database transaction because inserting new values
    db.transaction();

    // Prepare the query based on whether id is provided
    if (id >= 0) {
        query.prepare("INSERT INTO Users (user_id, username, first_name, last_name, email, date_of_registration) "
                      "VALUES (:user_id, :username, :first_name, :last_name, :email, :date_of_registration)");
        query.bindValue(":user_id", id);
    } else {
        query.prepare("INSERT INTO Users (username, first_name, last_name, email, date_of_registration) "
                      "VALUES (:username, :first_name, :last_name, :email, :date_of_registration)");
    }

    query.bindValue(":username", username);
    query.bindValue(":first_name", first_name);
    query.bindValue(":last_name", last_name);
    query.bindValue(":email", email);
    query.bindValue(":date_of_registration", date_of_registration);

    // Error handling
    if (!query.exec()) {
        qCritical() << "Failed to insert user into Users table:" << query.lastError().text();
        db.rollback();
        return false;
    }

    int userId = id >= 0 ? id : query.lastInsertId().toInt(); // Get the ID of the newly inserted user

    // Hash the password and insert it into the Authentication table
    QString passwordHash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex();

    query.prepare("INSERT INTO Authentication (user_id, password_hash) VALUES (:user_id, :password_hash)");
    query.bindValue(":user_id", userId);
    query.bindValue(":password_hash", passwordHash);

    // Error handling
    if (!query.exec()) {
        qCritical() << "Failed to insert user into Authentication table:" << query.lastError().text();
        db.rollback();
        return false;
    }

    // Commit the transaction
    db.commit();
    return true;
}


bool auth_manager::updateUser(const QString& username) {
    QSqlDatabase& db = m_dbAccess.getDatabase();
    QSqlQuery query(db);

    // Query user information from the Users table using the given username
    query.prepare("SELECT user_id, username, first_name, last_name, email, date_of_registration "
                  "FROM Users WHERE username = :username");
    query.bindValue(":username", username);

    //error handling
    if (!query.exec()) {
        qCritical() << "Failed to execute query:" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        //getting user info to update the currentUser variable
        int id = query.value(0).toInt();
        QString username = query.value(1).toString();
        QString firstName = query.value(2).toString();
        QString lastName = query.value(3).toString();
        QString email = query.value(4).toString();
        QString dateOfRegistration = query.value(5).toString();

        QDateTime currentDateTime = QDateTime::currentDateTime();
        const QString formattedDateTime = currentDateTime.toString("yyyy-MM-dd-hh-mm-ss");

        QSqlQuery query2(db);

        query2.prepare("UPDATE Authentication SET last_login_timestamp = :formattedDateTime"
                      " WHERE user_id = :id");


        query2.bindValue(":formattedDateTime", formattedDateTime);
        query2.bindValue(":id", id);

        if (!query2.exec()) {
            qCritical() << "Failed to execute query:" << query2.lastError().text();
        }

        // Update the currentUser instance variable with the queried user data
        currentUser = new user(id, username, firstName, lastName, email, dateOfRegistration);
        return true;
    } else {
        // No matching user found
        return false;
    }
}


bool auth_manager::changePassword(const QString &username, const QString& newPassword) {
    QSqlDatabase& db = m_dbAccess.getDatabase();
    QSqlQuery query(db);

    //Hash the new password
    QString newPasswordHash = QCryptographicHash::hash(newPassword.toUtf8(), QCryptographicHash::Sha256).toHex();

    //Update the password_hash in the database
    query.prepare("UPDATE Authentication SET password_hash = :newPasswordHash WHERE user_id = (SELECT user_id FROM Users WHERE username = :username)");
    query.bindValue(":newPasswordHash", newPasswordHash);
    query.bindValue(":username", username);

    //Error handling
    if (!query.exec()) {
        qCritical() << "Failed to update password" << query.lastError().text();
        db.rollback();
        return false;
    }

    //True if change successful
    return true;
}

bool auth_manager::addUserInterest(int id, QString interest){

    QSqlDatabase& db = m_dbAccess.getDatabase();
    QSqlQuery query(db);

    if (!db.transaction()) {
        return false;
    }

    // Get the ID of the interest from the Hashtags table
    query.prepare("SELECT hashtag_id FROM Hashtags WHERE name = :name");
    query.bindValue(":name", interest);
    if (!query.exec() || !query.first()) {
        db.rollback();
        return false;
    }
    int hashtagId = query.value("hashtag_id").toInt();

    // Insert the user-interest relationship into the UserInterests table
    query.prepare("INSERT INTO UserInterests (user_id, hashtag_id) VALUES (:userId, :hashtagId)");
    query.bindValue(":userId", id);
    query.bindValue(":hashtagId", hashtagId);
    if (!query.exec()) {
        db.rollback();
        return false;
    }

    if (!db.commit()) {
        db.rollback();  //Rollback if any type of failure.
        return false;
    }

    return true;
}

bool auth_manager::changeUserInformation(const QString& username, const QString& first_name, const QString& last_name,
                      const QString& email, int id){
    QSqlDatabase& db = m_dbAccess.getDatabase();
    QSqlQuery query(db);

    QString usernameFinal = username;
    QString first_nameFinal = first_name;
    QString last_nameFinal = last_name;
    QString emailFinal = email;

    query.prepare("SELECT username, first_name, last_name, email "
                  "FROM Users WHERE user_id = :user_id");
    query.bindValue(":user_id", id);

    //error handling
    if (!query.exec()) {
        qCritical() << "Failed to execute query:" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        if(usernameFinal == nullptr){
            usernameFinal = query.value(0).toString();
        }
        if(first_nameFinal == nullptr){
            first_nameFinal = query.value(1).toString();
        }
        if(last_nameFinal == nullptr){
            last_nameFinal = query.value(2).toString();
        }
        if(emailFinal == nullptr){
            emailFinal = query.value(3).toString();
        }
    }
    else {
        qCritical() << "No such user:";
        return false;
    }

    db.transaction();

    query.prepare("UPDATE Users Set "
                  "username = :username, "
                  "first_name = :first_name, "
                  "last_name = :last_name, "
                  "email = :email "
                  "WHERE user_id = :user_id");
    query.bindValue(":username", usernameFinal);
    query.bindValue(":first_name", first_nameFinal);
    query.bindValue(":last_name", last_nameFinal);
    query.bindValue(":email", emailFinal);
    query.bindValue(":user_id", id);

    if (!query.exec()) {
        qCritical() << "Failed to update Users:" << query.lastError().text();
        return false;
    }

    db.commit();
    this->updateUser(usernameFinal);
    return true;
}

bool auth_manager::clearIntersts(int user_id){
    QSqlDatabase& db = m_dbAccess.getDatabase();
    QSqlQuery query(db);

    db.transaction();

    query.prepare("DELETE FROM UserInterests WHERE user_id = :user_id");
    query.bindValue(":user_id", user_id);

    if (!query.exec()) {
        qCritical() << "Failed to delete user from UserInterests table:" << query.lastError().text();
        db.rollback();
        return false;
    }

    db.commit();
}


bool auth_manager::removeUser(int user_id) {
    QSqlDatabase& db = m_dbAccess.getDatabase();
    QSqlQuery query(db);

    // Begin a database transaction because we're deleting data
    db.transaction();

    // Delete the user from the Users table
    query.prepare("DELETE FROM Users WHERE user_id = :user_id");
    query.bindValue(":user_id", user_id);

    if (!query.exec()) {
        qCritical() << "Failed to delete user from Users table:" << query.lastError().text();
        db.rollback();
        return false;
    }

    // Delete the user from the Authentication table
    query.prepare("DELETE FROM Authentication WHERE user_id = :user_id");
    query.bindValue(":user_id", user_id);

    if (!query.exec()) {
        qCritical() << "Failed to delete user from Authentication table:" << query.lastError().text();
        db.rollback();
        return false;
    }

    // Delete the user from the UserInterests table
    query.prepare("DELETE FROM UserInterests WHERE user_id = :user_id");
    query.bindValue(":user_id", user_id);

    if (!query.exec()) {
        qCritical() << "Failed to delete user from UserInterests table:" << query.lastError().text();
        db.rollback();
        return false;
    }

    // Delete the user from the Notifications table
    query.prepare("DELETE FROM Notifications WHERE user_id = :user_id");
    query.bindValue(":user_id", user_id);

    if (!query.exec()) {
        qCritical() << "Failed to delete user from Notifications table:" << query.lastError().text();
        db.rollback();
        return false;
    }

    // Delete the user from the Posts table
    query.prepare("DELETE FROM Posts WHERE author_id = :user_id");
    query.bindValue(":user_id", user_id);

    if (!query.exec()) {
        qCritical() << "Failed to delete user from Posts table:" << query.lastError().text();
        db.rollback();
        return false;
    }

    // Delete the user from the RemovedPosts table
    query.prepare("DELETE FROM RemovedPosts WHERE moderator_id = :user_id");
    query.bindValue(":user_id", user_id);

    if (!query.exec()) {
        qCritical() << "Failed to delete user from RemovedPosts table:" << query.lastError().text();
        db.rollback();
        return false;
    }

    // Delete the user from the UserJail table
    query.prepare("DELETE FROM UserJail WHERE user_id = :user_id");
    query.bindValue(":user_id", user_id);

    if (!query.exec()) {
        qCritical() << "Failed to delete user from UserJail table:" << query.lastError().text();
        db.rollback();
        return false;
    }

    // Delete the user from the Moderators table
    query.prepare("DELETE FROM Moderators WHERE user_id = :user_id");
    query.bindValue(":user_id", user_id);

    if (!query.exec()) {
        qCritical() << "Failed to delete user from Moderators table:" << query.lastError().text();
        db.rollback();
        return false;
    }

    // Commit the transaction
    db.commit();
    return true;
}

int auth_manager::getUserId(QString username){
    QSqlDatabase& db = m_dbAccess.getDatabase();
    QSqlQuery query(db);

    query.prepare("SELECT user_id FROM Users WHERE username = :username");
    query.bindValue(":username", username);

    if (query.exec() && query.first()) {
        return query.value("user_id").toInt(); // Making sure the query retrieves at least one row.
    }

    return -1;  // Return -1 if nonexistent.
}

void auth_manager::logout(){
    if (currentUser){
        delete currentUser; // Current User is removed since that is the indicator of logout.
        currentUser = nullptr;
    }
}

//bool auth_manager::isBanned(const QString& username) {

//    QSqlDatabase& db = m_dbAccess.getDatabase();
//    QSqlQuery query(db);

//    query.prepare("SELECT Users.user_id, RemovedUsers.removed_user_id "
//                  "FROM Users "
//                  "LEFT JOIN RemovedUsers ON Users.user_id = RemovedUsers.user_id "
//                  "WHERE Users.username = :username");
//    query.bindValue(":username", username);

//    //error handling
//    if (!query.exec()) {
//        qCritical() << "Failed to execute query:" << query.lastError().text();
//        return false;
//    }

//    if (query.next()) {
//        return !query.value(1).isNull();
//    } else {
//        // No matching user found
//        return false;
//    }
//}

user* auth_manager::getCurrentUser(){
    return currentUser;
}
