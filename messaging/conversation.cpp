#include "conversation.h"
#include "QtSql/qsqlerror.h"
#include "QtSql/qsqlquery.h"
#include <iomanip>
#include <ctime>
#include <sstream>

conversation::conversation(auth_manager& userAuthentication): myAuthentication(userAuthentication){//db_access& dbAccess, user& userAccess): m_dbAccess(dbAccess), myUser(userAccess){

}

bool conversation::create_conversation(int user2_id){
    QSqlDatabase& db = myAuthentication.m_dbAccess.getDatabase();
    QSqlQuery query(db);

    //checking if post is already created
    query.prepare("SELECT conversation_id "
                  "FROM Conversations where user1_id = :user1_id "
                  "and user2_id = :user2_id");
    query.bindValue(":user1_id", myAuthentication.currentUser->getId());
    query.bindValue(":user2_id", user2_id);
    if (!query.exec()) {
        qCritical() << "Failed to execute query:" << query.lastError().text();
        return false;
    }
    if(query.next()){
        qCritical() << "Conversation already created";
        return false;
    }
    query.prepare("SELECT conversation_id "
                  "FROM Conversations where "
                  "user1_id = :user2_id "
                  "and user2_id = :user1_id");
    query.bindValue(":user1_id", myAuthentication.currentUser->getId());
    query.bindValue(":user2_id", user2_id);
    if (!query.exec()) {
        qCritical() << "Failed to execute query:" << query.lastError().text();
        return false;
    }
    if(query.next()){
        qCritical() << "Conversation already created";
        return false;
    }

    //start transaction
    db.transaction();
    query.prepare("INSERT INTO Conversations (user1_id, user2_id) "
                  "VALUES (:user1_id, :user2_id)");
    query.bindValue(":user1_id", myAuthentication.currentUser->getId());
    query.bindValue(":user2_id", user2_id);
    if (!query.exec()) {
        qCritical() << "Failed to insert into Conversations:" << query.lastError().text();
        db.rollback();
        return false;
    }
    db.commit();
    return true;
}

bool conversation::delete_conversation(int user2_id){
    QSqlDatabase& db = myAuthentication.m_dbAccess.getDatabase();
    QSqlQuery query(db);

    //finding conversation id
    int conversation_id = -1;
    query.prepare("SELECT conversation_id "
                  "FROM Conversations where user1_id = :user1_id "
                  "and user2_id = :user2_id");
    query.bindValue(":user1_id", myAuthentication.currentUser->getId());
    query.bindValue(":user2_id", user2_id);
    if (!query.exec()) {
        qCritical() << "Failed to execute query:" << query.lastError().text();
        return false;
    }
    if(query.next()){
        conversation_id = query.value(0).toInt();
    }

    query.prepare("SELECT conversation_id "
                  "FROM Conversations where "
                  "user1_id = :user2_id "
                  "and user2_id = :user1_id");
    query.bindValue(":user1_id", myAuthentication.currentUser->getId());
    query.bindValue(":user2_id", user2_id);
    if (!query.exec()) {
        qCritical() << "Failed to execute query:" << query.lastError().text();
        return false;
    }

    if(query.next()){
        conversation_id = query.value(0).toInt();
    }
    if(conversation_id < 0){
        qCritical() << "No conversation exists";
        return false;
    }

    db.transaction();
    query.prepare("DELETE FROM Conversations WHERE "
                  "conversation_id = :conversation_id");
    query.bindValue(":conversation_id", conversation_id);
    if (!query.exec()) {
        qCritical() << "Failed to delete from Conversation:" << query.lastError().text();
        db.rollback();
        return false;
    }

    db.transaction();
    query.prepare("DELETE FROM Messages WHERE "
                  "conversation_id = :conversation_id");
    query.bindValue(":conversation_id", conversation_id);
    if (!query.exec()) {
        qCritical() << "Failed to delete from Messages:" << query.lastError().text();
        db.rollback();
        return false;
    }
    db.commit();
    return true;
}

std::vector<one_conversation> conversation::all_conversations(){
    QSqlDatabase& db = myAuthentication.m_dbAccess.getDatabase();
    QSqlQuery query(db);
    std::vector<one_conversation> holder;

    //finding conversations
    query.prepare("SELECT conversation_id, user1_id, user2_id "
                  "FROM Conversations "
                  "WHERE user1_id = :user_id "
                  "OR user2_id = :user_id");
    query.bindValue(":user_id", myAuthentication.currentUser->getId());
    if (!query.exec()) {
        qCritical() << "Failed to execute query:" << query.lastError().text();
        return {};
    }
    bool checker = false;
    while(query.next()){
        checker = true;
          int conversation_id = query.value(0).toInt();
          int user1_id = query.value(1).toInt();
          int user2_id = query.value(2).toInt();
          one_conversation temp = one_conversation(conversation_id, user1_id, user2_id);
          holder.push_back(temp);
    }
    if(!checker){return {};}
    return holder;
}

std::vector<one_message> conversation::all_messages(int user2_id){
    QSqlDatabase& db = myAuthentication.m_dbAccess.getDatabase();
    QSqlQuery query(db);
    std::vector<one_message> holder;
    int conversation_id;

    //finding conversation_id
    query.prepare("SELECT conversation_id "
                  "FROM Conversations where user1_id = :user1_id "
                  "and user2_id = :user2_id");
    query.bindValue(":user1_id", myAuthentication.currentUser->getId());
    query.bindValue(":user2_id", user2_id);
    bool conversationChecker = false;
    if (!query.exec()) {
        qCritical() << "Failed to execute query:" << query.lastError().text();
        return {};
    }
    if(query.next()){
        conversation_id = query.value(0).toInt();
    }
    else{
        conversationChecker = true;
    }
    query.prepare("SELECT conversation_id "
                  "FROM Conversations where "
                  "user1_id = :user2_id "
                  "and user2_id = :user1_id");
    query.bindValue(":user1_id", myAuthentication.currentUser->getId());
    query.bindValue(":user2_id", user2_id);
    if (!query.exec()) {
        qCritical() << "Failed to execute query:" << query.lastError().text();
        return {};
    }
    if(query.next()){
        conversation_id = query.value(0).toInt();
    }
    else if(conversationChecker){
        qCritical() << "No conversation available";
        return {};
    }

    //finding messages with given conversation
    query.prepare("SELECT message_id, conversation_id, sender_id, content "
                  "FROM Messages "
                  "WHERE conversation_id = :conversation_id");
    query.bindValue(":conversation_id", conversation_id);
    if (!query.exec()) {
        qCritical() << "Failed to execute query:" << query.lastError().text();
        return {};
    }
    bool checker = false;
    while(query.next()){
        checker = true;
        int message_id = query.value(0).toInt();
        int conversation_id = query.value(1).toInt();
        int sender_id = query.value(2).toInt();
        QString content = query.value(3).toString();
        QString timestamp = query.value(4).toString();
        one_message temp = one_message(message_id,
                                       conversation_id,
                                       sender_id,
                                       content.toStdString(),
                                       timestamp.toStdString());
        holder.push_back(temp);
    }
    if(!checker){return {};}
    return holder;
}

bool conversation::send_message(int user2_id, std::string content){
    QSqlDatabase& db = myAuthentication.m_dbAccess.getDatabase();
    QSqlQuery query(db);
    int conversation_id;

    //finding conversation_id
    query.prepare("SELECT conversation_id "
                  "FROM Conversations where user1_id = :user1_id "
                  "and user2_id = :user2_id");
    query.bindValue(":user1_id", myAuthentication.currentUser->getId());
    query.bindValue(":user2_id", user2_id);
    bool conversationChecker = false;
    if (!query.exec()) {
        qCritical() << "Failed to execute query:" << query.lastError().text();
        return false;
    }
    if(query.next()){
        conversation_id = query.value(0).toInt();
    }
    else{
        conversationChecker = true;
    }

    query.prepare("SELECT conversation_id "
                  "FROM Conversations where "
                  "user1_id = :user2_id "
                  "and user2_id = :user1_id");
    query.bindValue(":user1_id", myAuthentication.currentUser->getId());
    query.bindValue(":user2_id", user2_id);
    if (!query.exec()) {
        qCritical() << "Failed to execute query:" << query.lastError().text();
        return false;
    }
    if(query.next()){
        conversation_id = query.value(0).toInt();
    }
    else if(conversationChecker){
        qCritical() << "No conversation available";
        return false;
    }

    //starting transaction
    db.transaction();
    query.prepare("INSERT INTO Messages (conversation_id, sender_id, content, timestamp) "
                  "VALUES (:conversation_id, :sender_id, :content, :timestamp)");
    query.bindValue(":conversation_id", conversation_id);
    query.bindValue(":sender_id", myAuthentication.currentUser->getId());
    query.bindValue(":content", QString::fromStdString(content));
    //finding time for timestamp
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d-%H-%M-%S");
    auto str = oss.str();
    query.bindValue(":timestamp", QString::fromStdString(str));
    if (!query.exec()) {
        qCritical() << "Failed to insert into Messages" << query.lastError().text();
        db.rollback();
        return false;
    }
    db.commit();
    return true;
}
