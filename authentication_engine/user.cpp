// user.cpp
#include "user.h"
#include "qsqldatabase.h"
#include "qsqlerror.h"
#include "qsqlquery.h"

user::user(int id, const QString &username, const QString &firstName, const QString &lastName,
           const QString &email, const QString &dateOfRegistration)
    : m_id(id), m_username(username), m_firstName(firstName), m_lastName(lastName),
      m_email(email), m_dateOfRegistration(dateOfRegistration) {}

int user::getId() const {
    return m_id;
}

QString user::getUsername() const {
    return m_username;
}

QString user::getFirstName() const {
    return m_firstName;
}

QString user::getLastName() const {
    return m_lastName;
}

QString user::getEmail() const {
    return m_email;
}

QString user::getDateOfRegistration() const {
    return m_dateOfRegistration;
}

//bool user::addPost(const QString& title, const QString& contents, const QString& timestamp) {
//    QSqlDatabase& db = m_dbAccess.getDatabase();
//    QSqlQuery query(db);

//    // Inserting new values w transaciton
//    db.transaction();

//    // Prepare the query for inserting a new post
//    query.prepare("INSERT INTO Posts (author_id, title, content, timestamp) "
//                  "VALUES (:author_id, :title, :content, :timestamp)");

//    query.bindValue(":author_id", m_id);
//    query.bindValue(":title", title);
//    query.bindValue(":content", contents);
//    query.bindValue(":timestamp", timestamp);

//    // Error handling
//    if (!query.exec()) {
//        qCritical() << "Failed to insert post into Posts table:" << query.lastError().text();
//        db.rollback();
//        return false;
//    }

//    // Commit the transaction
//    db.commit();
//    return true;
//}

int user::getPostCount(){
    QSqlDatabase& db = m_dbAccess.getDatabase();
    QSqlQuery query(db);

    // Prepare the query
    query.prepare("SELECT COUNT(*) FROM Posts WHERE author_id = :author_id");
    query.bindValue(":author_id", m_id);

    // Error handling & execution
    if (!query.exec()) {
        qCritical() << "Failed to retrieve post count for user:" << query.lastError().text();
        return -1;
    }

    // If the query is successful, retrieve the post count
    if (query.next()) {
        int postCount = query.value(0).toInt();
        return postCount;
    }

    // If the query returns no results, return 0 as the post count
    return 0;
}

bool user::reportPost(int postId, const QString &reason, const QString &timestamp) {
    QSqlDatabase& db = m_dbAccess.getDatabase();
    QSqlQuery query(db);

    // Begin a database transaction because we're inserting data
    db.transaction();

    // Insert the report into the PostReports table
    query.prepare("INSERT INTO PostReports (post_id, reporter_id, reason, timestamp) VALUES (:post_id, :reporter_id, :reason, :timestamp)");
    query.bindValue(":post_id", postId);
    query.bindValue(":reporter_id", m_id);
    query.bindValue(":reason", reason);
    query.bindValue(":timestamp", timestamp);

    if (!query.exec()) {
        qCritical() << "Failed to report post in PostReports table:" << query.lastError().text();
        db.rollback();
        return false;
    }

    // Commit the transaction
    db.commit();
    return true;
}

bool user::removePost(int postId) {
    QSqlDatabase& db = m_dbAccess.getDatabase();
    QSqlQuery query(db);

    // Begin a database transaction because we're deleting data
    db.transaction();

    // Delete the post from the PostHashtags table
    query.prepare("DELETE FROM PostHashtags WHERE post_id = :post_id");
    query.bindValue(":post_id", postId);

    if (!query.exec()) {
        qCritical() << "Failed to delete post from PostHashtags table:" << query.lastError().text();
        db.rollback();
        return false;
    }

    // Delete the post from the PostReports table
    query.prepare("DELETE FROM PostReports WHERE post_id = :post_id");
    query.bindValue(":post_id", postId);

    if (!query.exec()) {
        qCritical() << "Failed to delete post from PostReports table:" << query.lastError().text();
        db.rollback();
        return false;
    }

    // Delete the post from the UserFeed table
    query.prepare("DELETE FROM UserFeed WHERE post_id = :post_id");
    query.bindValue(":post_id", postId);

    if (!query.exec()) {
        qCritical() << "Failed to delete post from UserFeed table:" << query.lastError().text();
        db.rollback();
        return false;
    }

    // Delete the post from the Posts table
    query.prepare("DELETE FROM Posts WHERE post_id = :post_id");
    query.bindValue(":post_id", postId);

    if (!query.exec()) {
        qCritical() << "Failed to delete post from Posts table:" << query.lastError().text();
        db.rollback();
        return false;
    }

    // Commit the transaction
    db.commit();
    return true;
}

std::vector<std::string> user::getInterests(){
    QSqlDatabase& db = m_dbAccess.getDatabase();
    QSqlQuery query(db);
    std::vector<std::string> myInterests;

    // Prepare the query
    query.prepare("SELECT name FROM Hashtags "
                  "WHERE hashtag_id IN (select hashtag_id from UserInterests where user_id = :user_id)");
    query.bindValue(":user_id", m_id);

    if (!query.exec()) {
        qCritical() << "Failed to run query:" << query.lastError().text();
        return {};
    }

    while(query.next()){
        QString interest = query.value(0).toString();
        myInterests.push_back(interest.toStdString());
    }

    return myInterests;
}
