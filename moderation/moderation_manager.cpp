#include "moderation_manager.h"

moderation_manager::moderation_manager(db_access& dbAccess) : m_dbAccess(dbAccess)
{
    currentModerator = nullptr;
}

bool moderation_manager::assignModerator(int user_id) {

    // Database Reference
    QSqlDatabase& db = m_dbAccess.getDatabase();
    QSqlQuery query(db);

    // Inserting the user id into the moderators table
    query.prepare("INSERT INTO Moderators (user_id) VALUES (:user_id)");
    query.bindValue(":user_id", user_id);

    if (!query.exec()) {
        qCritical() << "Failed to assign user as moderator:" << query.lastError().text();
        return false;
    }

    return true;
}

bool moderation_manager::checkModerator(int user_id){    
    // Database Reference
    QSqlDatabase& db = m_dbAccess.getDatabase();
    QSqlQuery query(db);

    // Fetching the moderator id from the moderator
    // table and comparing it to user ids in the user table
    query.prepare("SELECT moderator_id FROM Moderators WHERE user_id = :user_id;");
    query.bindValue(":user_id", user_id);

    if (!query.exec()) {
        qCritical() << "Failed to retrieve moderator with given id: " << query.lastError().text();
        return false;
    }
    return query.next();
}

std::vector<std::vector<std::string>> moderation_manager::all_reports() {

    // Database reference
    QSqlDatabase& db = m_dbAccess.getDatabase();
    QSqlQuery query(db);

    // Vector storing all reports
    std::vector<std::vector<std::string>> allReports;

    if (!query.exec("SELECT pr.report_id, pr.reporter_id, pr.post_id, pr.reason, pr.timestamp, p.author_id, p.title "
                    "FROM PostReports pr "
                    "JOIN Posts p ON pr.post_id = p.post_id ")) {
        qCritical() << "Failed to execute query:" << query.lastError().text();
        return {};
    }

    // Iterating over the result set and adding each report to the `allReports` vector.
    while(query.next()){

        // Fetching data for the current report
        int report_id = query.value(0).toInt();
        int reporter_id = query.value(1).toInt();
        int post_id = query.value(2).toInt();
        QString reason = query.value(3).toString();
        QString timestamp = query.value(4).toString();
        int user_id = query.value(5).toInt();
        QString title = query.value(6).toString();


        // Creating a vector to store the data for the current report.
        std::vector<std::string> reportData;
        reportData.push_back(std::to_string(report_id));
        reportData.push_back(std::to_string(reporter_id));
        reportData.push_back(std::to_string(post_id));
        reportData.push_back(reason.toStdString());
        reportData.push_back(timestamp.toStdString());
        reportData.push_back(std::to_string(user_id));
        reportData.push_back(title.toStdString());

        allReports.push_back(reportData);
    }

    return allReports;
}

bool moderation_manager::checkModerator(QString username){
    QSqlDatabase& db = m_dbAccess.getDatabase();
    QSqlQuery query(db);

    // Fetching the moderator id from the moderator
    // table and comparing it to user ids in the user table
    query.prepare("SELECT moderator_id FROM Moderators WHERE user_id = (SELECT user_id FROM Users WHERE username = :username)");
    query.bindValue(":username", username);

    if (!query.exec()) {
        qCritical() << "Failed to retrieve moderator with given id: " << query.lastError().text();
        return false;
    }
    return query.next();
}

bool moderation_manager::authenticateModerator(QString username, QString password){
    QSqlDatabase& db = m_dbAccess.getDatabase();
    QSqlQuery query(db);


    // Fetching the password hash from the database to compare it to entry.
    query.prepare("SELECT password_hash FROM Authentication WHERE user_id = (SELECT user_id FROM Users WHERE username = :username)");
    query.bindValue(":username", username);

    // Error handling
    if (!query.exec()) {
        qCritical() << "Failed to execute query:" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        // Storing password hash
        QString storedPasswordHash = query.value(0).toString();

        // Converting user-given password to a password hash
        QString passwordHash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex();

        if (storedPasswordHash == passwordHash && checkModerator(username)){
            updateModerator(username);
        }else{
            return false;
        }
    } else {
        // No matching user found
        return false;
    }

    return true;
}

bool moderation_manager::updateModerator(QString username){

    // Database reference.
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
        // Getting user info to update the currentUser variable
        int id = query.value(0).toInt();
        QString username = query.value(1).toString();
        QString firstName = query.value(2).toString();
        QString lastName = query.value(3).toString();
        QString email = query.value(4).toString();
        QString dateOfRegistration = query.value(5).toString();

        // Update the currentUser instance variable with the queried user data
        currentModerator = new moderator(id, username, firstName, lastName,
                                         email, dateOfRegistration, m_dbAccess);
        return true;
    } else {
        // No matching user found
        return false;
    }
}

void moderation_manager::logout(){
    // If currentModerator exists, we enable the user to logout.
    if (currentModerator){
        delete currentModerator;
        currentModerator = nullptr;
    }
}

QString moderation_manager::getPostContent(int post_id){

    // Database reference
    QSqlDatabase db = m_dbAccess.getDatabase();
    QSqlQuery query(db);

    // Fetching postID
    query.prepare("SELECT content FROM Posts WHERE post_id = :post_id");
    query.bindValue(":post_id", post_id);

    // Error handling
    if (!query.exec()) {
        qCritical() << "Failed to execute query:" << query.lastError().text();
        return QString();
    }

    // If Query has output, convert it to String.
    if (query.next()) {
        QString postContent = query.value(0).toString();
        return postContent;
    } else {
        // No matching post found
        return QString();
    }
}

moderator* moderation_manager::getCurrentModerator(){
    return currentModerator;
}

