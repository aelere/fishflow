#include "upload.h"


upload::upload(auth_manager& userAuthentication): myAuthentication(userAuthentication){// db_access& dbAccess, user& userAccess): m_dbAccess(dbAccess), myUser(userAccess){
}


bool upload::send(std::string title, std::string content, std::vector<std::string> hashtags){
    QSqlDatabase& db = myAuthentication.m_dbAccess.getDatabase();
    QSqlQuery query(db);

    //start transaction
    db.transaction();
    query.prepare("INSERT INTO Posts (author_id, title, content, timestamp) "
                  "VALUES (:author_id, :title, :content, :timestamp)");

    query.bindValue(":author_id", myAuthentication.currentUser->getId());
    query.bindValue(":title", QString::fromStdString(title));
    query.bindValue(":content", QString::fromStdString(content));


    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d-%H-%M-%S");
    auto str = oss.str();
    query.bindValue(":timestamp", QString::fromStdString(str));

    if (!query.exec()) {
        qCritical() << "Failed to insert user into Posts table:" << query.lastError().text();
        db.rollback();
        return false;
    }

    int post_id = query.lastInsertId().toInt();

    for(int i = 0; i < hashtags.size(); i++){
        //converting hastag to lower case
        std::string name;
        char arr[hashtags.at(i).length()+1];
        strcpy(arr,hashtags.at(i).c_str());
        for(int x = 0; x < strlen(arr); x++){
            if(64 < arr[x] & arr[x] < 91){
                name+= (arr[x]+32);
            }
            else{
                name+= (arr[x]);
            }
        }
        //checking if hashtag is already created
        query.prepare("SELECT hashtag_id "
                      "from Hashtags where name = :name");
        query.bindValue(":name", QString::fromStdString(name));
        if (!query.exec()) {
            qCritical() << "Failed to execute query:" << query.lastError().text();
            db.rollback();
            return false;
        }
        int hashtag_id;
        if(query.next()){ //if hashtag is already created just get id
            hashtag_id = query.value(0).toInt();
        }
        else{ //else create hashtag id and insert into Hashtags table
            query.prepare("INSERT INTO Hashtags (name) "
                          "VALUES (:name)");
            query.bindValue(":name", QString::fromStdString(name));
            if (!query.exec()) {
                qCritical() << "Failed to insert into Hashtags:" << query.lastError().text();
                db.rollback();
                return false;
            }

            hashtag_id = query.lastInsertId().toInt();

        }
        //inserting into PostHashtags
        query.prepare("INSERT or REPLACE INTO PostHashtags (post_id, hashtag_id) "
                      "VALUES (:post_id, :hashtag_id)");
        query.bindValue(":post_id", post_id);
        query.bindValue(":hashtag_id", hashtag_id);

        if (!query.exec()) {
            qCritical() << "Failed to insert into PostHashtags:" << query.lastError().text();
            db.rollback();
            return false;
        }
    }

    //Commit Transaction
    db.commit();
    return true;
}

std::vector<one_post> upload::findPosts(){
    QSqlDatabase& db = myAuthentication.m_dbAccess.getDatabase();
    QSqlQuery query(db);
    std::vector<one_post> holder;

    //finding all post_ids from user
    query.prepare("SELECT post_id, author_id, title, content, timestamp "
                  "FROM Posts where author_id = :author_id");
    query.bindValue(":author_id", myAuthentication.currentUser->getId());

    if (!query.exec()) {
        qCritical() << "Failed to execute query:" << query.lastError().text();
        return {};
    }

    bool check = false;
    while(query.next()){
        check = true;
        int post_id = query.value(0).toInt();
        int author_id = query.value(1).toInt();
        QString title = query.value(2).toString();
        QString content = query.value(3).toString();
        QString timestamp = query.value(4).toString();
        one_post temp = one_post(post_id, author_id, title, content, timestamp);
        holder.push_back(temp);
    }

    //returns null if no posts were found
    if(!check){return {};}
    return holder;
}

std::vector<one_post> upload::other_findPosts(int user_id){
    QSqlDatabase& db = myAuthentication.m_dbAccess.getDatabase();
    QSqlQuery query(db);
    std::vector<one_post> holder;

    //finding all post_ids from user
    query.prepare("SELECT post_id, author_id, title, content, timestamp "
                  "FROM Posts where author_id = :author_id");
    query.bindValue(":author_id", user_id);

    if (!query.exec()) {
        qCritical() << "Failed to execute query:" << query.lastError().text();
        return {};
    }

    bool check = false;
    while(query.next()){
        check = true;
        int post_id = query.value(0).toInt();
        int author_id = query.value(1).toInt();
        QString title = query.value(2).toString();
        QString content = query.value(3).toString();
        QString timestamp = query.value(4).toString();
        one_post temp = one_post(post_id, author_id, title, content, timestamp);
        holder.push_back(temp);
    }

    //returns null if no posts were found
    if(!check){return {};}
    return holder;
}

bool upload::report_post(int post_id, std::string reason){
    QSqlDatabase& db = myAuthentication.m_dbAccess.getDatabase();
    QSqlQuery query(db);

    //checking that the post was not already reported by that user
    query.prepare("SELECT report_id "
                  "FROM PostReports "
                  "where post_id = :post_id "
                  "and reporter_id = :reporter_id");
    query.bindValue(":post_id", post_id);
    query.bindValue(":reporter_id", myAuthentication.currentUser->getId());
    if (!query.exec()) {
        qCritical() << "Failed to execute query:" << query.lastError().text();
        return false;
    }
    if(query.next()){
        qCritical() << "Post has already been reported by user";
        return false;
    }


    //starting transaction
    db.transaction();
    query.prepare("INSERT INTO PostReports (report_id, post_id, reporter_id, reason, timestamp) "
                  "VALUES (:report_id, :post_id, :reporter_id, :reason, :timestamp)");
    query.bindValue(":post_id", post_id);
    query.bindValue(":reporter_id", myAuthentication.currentUser->getId());
    query.bindValue(":reason", QString::fromStdString(reason));
    //finding time for timestamp
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d-%H-%M-%S");
    auto str = oss.str();
    query.bindValue(":timestamp", QString::fromStdString(str));

    //Error handling
    if (!query.exec()) {
        qCritical() << "Failed to insert user into PostReports table:" << query.lastError().text();
        db.rollback();
        return false;
    }

    //Commit transaction
    db.commit();
    return true;
}

bool upload::remove_post(int post_id, int moderator_id, std::string removal_reason){
    QSqlDatabase& db = myAuthentication.m_dbAccess.getDatabase();
    QSqlQuery query(db);

    //checking that the post exists
    query.prepare("SELECT post_id FROM Posts "
                  "where post_id = :post_id");
    query.bindValue(":post_id", post_id);
    if (!query.exec()) { //error handling
        qCritical() << "Failed to execute query:" << query.lastError().text();
        return false;
    }
    if(!query.next()){
        qCritical() << "Post is not available";
        return false;
    }

    db.transaction();
    //adding post to RemovedPosts
    query.prepare("INSERT INTO RemovedPosts (post_id, moderator_id, removal_reason, removal_timestamp) "
                  "VALUES (:post_id, :moderator_id, :removal_reason, :removal_timestamp)");
    query.bindValue(":post_id", post_id);
    query.bindValue(":moderator_id", moderator_id);
    query.bindValue(":removal_reason", QString::fromStdString(removal_reason));
    //finding time for timestamp
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d-%H-%M-%S");
    auto str = oss.str();
    query.bindValue(":removal_timestamp", QString::fromStdString(str));

    if (!query.exec()) { //error handling
        qCritical() << "Failed to insert into query:" << query.lastError().text();
        db.rollback();
        return false;
    }


    //remove post from Posts
    query.prepare("DELETE FROM Posts "
                  "where post_id = :post_id");
    query.bindValue(":post_id", post_id);

    //error handling
    if (!query.exec()) {
        qCritical() << "Failed to delete post from Posts table:" << query.lastError().text();
        db.rollback();
        return false;
    }
    db.commit();
    return true;
}

bool upload::delete_post(int post_id){
    QSqlDatabase& db = myAuthentication.m_dbAccess.getDatabase();
    QSqlQuery query(db);

    //checking that the post exists
    query.prepare("SELECT post_id FROM Posts "
                  "where post_id = :post_id");
    query.bindValue(":post_id", post_id);
    if (!query.exec()) { //error handling
        qCritical() << "Failed to execute query:" << query.lastError().text();
//        db.rollback();
        return false;
    }
    if(!query.next()){
        qCritical() << "Post is not available";
        return false;
    }

    //starting transaction
    db.transaction();

    query.prepare("DELETE FROM Posts "
                  "where post_id = :post_id");
    query.bindValue(":post_id", post_id);

    //error handling
    if (!query.exec()) {
        qCritical() << "Failed to delete post from Posts table:" << query.lastError().text();
        db.rollback();
        return false;
    }

    query.prepare("DELETE FROM PostHashtags "
                  "where post_id = :post_id");
    query.bindValue(":post_id", post_id);
    if (!query.exec()) {
        qCritical() << "Failed to delete post from PostHashtags table:" << query.lastError().text();
        db.rollback();
        return false;
    }

    db.commit();
    return true;
}

std::vector<std::string> upload::find_hashtags(){
    QSqlDatabase& db = myAuthentication.m_dbAccess.getDatabase();
    QSqlQuery query(db);
    std::vector<std::string> holder;

    //finding posts;
    query.prepare("SELECT name FROM Hashtags");
    if (!query.exec()) { //error handling
        qCritical() << "Failed to execute query:" << query.lastError().text();
        return {};
    }
    while(query.next()){
        holder.push_back(query.value(0).toString().toStdString());
    }
    return holder;
}

//returns the authors username, first name, last name, and all hashtags
std::vector<std::string> upload::findInformation(int postId){
    QSqlDatabase& db = myAuthentication.m_dbAccess.getDatabase();
    QSqlQuery query(db);
    std::vector<std::string> temp;

    // Retrieve username, first name, and last name of the post author
    query.prepare("SELECT username, first_name, last_name "
                  "FROM Users "
                  "WHERE user_id = (SELECT author_id FROM Posts WHERE post_id = :post_id)");
    query.bindValue(":post_id", postId);
    if (!query.exec()){
        qCritical() << "Failed to execute query:" << query.lastError().text();
        return {};
    }
    while(query.next()){
        QString username = query.value(0).toString();
        QString first_name = query.value(1).toString();
        QString last_name = query.value(2).toString();
        temp.push_back(username.toStdString());
        temp.push_back(first_name.toStdString());
        temp.push_back(last_name.toStdString());
    }

    // Retrieve the hashtag IDs associated with the post
    std::vector<int> hashtag_ids;
    query.prepare("SELECT hashtag_id "
                  "FROM PostHashtags "
                  "WHERE post_id = :post_id");
    query.bindValue(":post_id", postId);
    if (!query.exec()){
        qCritical() << "Failed to execute query:" << query.lastError().text();
        return {};
    }
    while(query.next()){
        int hashtag_id = query.value(0).toInt();
        hashtag_ids.push_back(hashtag_id);
    }
    if(hashtag_ids.size() != 0){
        for(int i = 0; i < hashtag_ids.size(); i++){
            // Retrieve the names of hashtags based on their IDs
            query.prepare("SELECT name "
                          "FROM Hashtags "
                          "WHERE hashtag_id = :hashtag_id");
            query.bindValue(":hashtag_id", hashtag_ids.at(i));
            if (!query.exec()){
                qCritical() << "Failed to execute query:" << query.lastError().text();
                return {};
            }
            query.next();
            QString name = query.value(0).toString();
            temp.push_back(name.toStdString());
        }
    }
    return temp;
}

user* upload::getOtherUser(int user_id){
    QSqlDatabase& db = myAuthentication.m_dbAccess.getDatabase();
    QSqlQuery query(db);

    query.prepare("SELECT username, first_name, last_name, email, date_of_registration "
                  "from Users where user_id = :user_id");
    query.bindValue(":user_id", user_id);

    if (!query.exec()){
        qCritical() << "Failed to exert query:" << query.lastError().text();
        return NULL;
    }

    query.next();
    QString username = query.value(0).toString();
    QString first_name = query.value(1).toString();
    QString last_name = query.value(2).toString();
    QString email = query.value(3).toString();
    QString date = query.value(4).toString();

    static user temp = user(user_id, username, first_name, last_name, email, date);

    return &temp;
}
