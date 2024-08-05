#include "user_feed.h"
#include <set>


user_feed::user_feed(auth_manager& userAuthentication): myAuthentication(userAuthentication){//db_access& dbAccess, user& userAccess): m_dbAccess(dbAccess), myUser(userAccess){
}

//returns all posts sorted by timestamp
//for posts that were posted on the same day,
//all posts that were part of user interest is prioritized
std::vector<one_post> user_feed::all_posts(){
    QSqlDatabase& db = myAuthentication.m_dbAccess.getDatabase();
    QSqlQuery query(db);

    if (!query.exec("SELECT post_id, author_id, title, content, timestamp FROM Posts")) {
        qCritical() << "Failed to execute query:" << query.lastError().text();
        return std::vector<one_post>();
    }

    std::vector<one_post> allPosts;

    while (query.next()) {
        int post_id = query.value(0).toInt();
        int author_id = query.value(1).toInt();
        QString title = query.value(2).toString();
        QString content = query.value(3).toString();
        QString timestamp = query.value(4).toString();

        one_post temp = one_post(post_id, author_id, title, content, timestamp);
        allPosts.push_back(temp);
    }

    return filterShadowbannedPosts(allPosts);
}

std::vector<one_post> user_feed::posts_by_hashtag() {
    QSqlDatabase& db = myAuthentication.m_dbAccess.getDatabase();
    QSqlQuery query(db);
    // Finding hashtags
    std::vector<int> hashtag_ids;
    query.prepare("SELECT hashtag_id "
                  "From UserInterests where user_id = :user_id");
    query.bindValue(":user_id", myAuthentication.currentUser->getId()); //Making sure hashtags match with user
                                                                        // preferences
    if (!query.exec()) {
        qCritical() << "Failed to execute query:" << query.lastError().text();
        return {};
    }
    while (query.next()) {
        hashtag_ids.push_back(query.value(0).toInt());
    }

    // Finding posts with matching hashtags
    std::vector<one_post> matching_posts;
    std::set<int> matching_post_ids;
    for (int i = 0; i < hashtag_ids.size(); i++) {
        query.prepare("select e.post_id, d.author_id, d.title, d.content, d.timestamp "
                      "from Posts as d, PostHashtags as e "
                      "where e.post_id = d.post_id "
                      "and e.post_id = d.post_id "
                      "and e.hashtag_id = :hashtag_id");
        query.bindValue(":hashtag_id", hashtag_ids.at(i));
        if (!query.exec()) {
            qCritical() << "Failed to exert query:" << query.lastError().text();
            return {};
        }
        while (query.next()) {

            // Creating a temp post.
            int post_id = query.value(0).toInt();
            int author_id = query.value(1).toInt();
            QString title = query.value(2).toString();
            QString content = query.value(3).toString();
            QString timestamp = query.value(4).toString();
            one_post temp = one_post(post_id, author_id, title, content, timestamp);
            matching_posts.push_back(temp);
            matching_post_ids.insert(post_id);
        }
    }

    // Finding all posts
    std::vector<one_post> all_posts;
    query.prepare("select post_id, author_id, title, content, timestamp from Posts");
    if (!query.exec()) {
        qCritical() << "Failed to execute query:" << query.lastError().text();
        return {};
    }
    while (query.next()) {
        int post_id = query.value(0).toInt();
        if (matching_post_ids.find(post_id) == matching_post_ids.end()) {
            int author_id = query.value(1).toInt();
            QString title = query.value(2).toString();
            QString content = query.value(3).toString();
            QString timestamp = query.value(4).toString();
            one_post temp = one_post(post_id, author_id, title, content, timestamp);
            all_posts.push_back(temp);
        }
    }

    // Sorting and matching all posts
    std::vector<one_post> sorted_matching_posts = sort(matching_posts);
    std::vector<one_post> sorted_all_posts = sort(all_posts);

    // Merging for final output
    std::vector<one_post> combined_posts;
    combined_posts.insert(combined_posts.end(), sorted_matching_posts.begin(), sorted_matching_posts.end());
    combined_posts.insert(combined_posts.end(), sorted_all_posts.begin(), sorted_all_posts.end());

    return filterShadowbannedPosts(combined_posts);
}

//returns the authors username, first name, last name, and all hashtags
std::vector<std::string> user_feed::findInformation(int postId){
    QSqlDatabase& db = myAuthentication.m_dbAccess.getDatabase();
    QSqlQuery query(db);
    std::vector<std::string> temp;

    // Retrieve username, first name, and last name of the author of the post
    query.prepare("SELECT username, first_name, last_name "
                  "FROM Users "
                  "WHERE user_id = (SELECT author_id FROM Posts WHERE post_id = :post_id);");
    query.bindValue(":post_id", postId);

    if (!query.exec()) {
        qCritical() << "Failed to execute query:" << query.lastError().text();
        return {};
    }

    if (query.next()) {
        QString username = query.value(0).toString();
        QString first_name = query.value(1).toString();
        QString last_name = query.value(2).toString();
        temp.push_back(username.toStdString());
        temp.push_back(first_name.toStdString());
        temp.push_back(last_name.toStdString());
    } else {
        qDebug() << "No results found.";
    }

    std::vector<int> hashtag_ids;

    // Retrieve the hashtag IDs associated with the post
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

    // Retrieve the names of hashtags based on their IDs and add them to the vector
    if(hashtag_ids.size() != 0){
        for(int i = 0; i < hashtag_ids.size(); i++){
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


std::vector<one_post> user_feed::sort(std::vector<one_post> initial){
    std::vector<one_post> final;
    while(initial.size() > 0){
        if(initial.size() == 1){
            final.push_back(initial.at(0));
            break;
        }
        int j = 0;
        one_post temp = initial.at(0);
        for(int i = 1; i < initial.size(); i++){
            if(compare(initial.at(i), temp)){
                temp = initial.at(i);
                j = i;
            }
        }
        final.push_back(initial.at(j));
        initial.erase(initial.begin()+j);
    }
    return final;
}


//returns true if first is posted more recently than second
bool user_feed::compare(one_post first, one_post second){
    if(year(first) == year(second)){
        if(month(first) == month(second)){
            if(date(first) == date(second)){
                if(hours(first) == hours(second)){
                    if(minutes(first) == minutes(second)){
                        return seconds(first) >= seconds(second);
                    }
                    return minutes(first) > minutes(second);
                }
                return hours(first) > hours(second);
            }
            return date(first) > date(second);
        }
        return month(first) > month(second);
    }
    return year(first) > year(second);
}

//returns true if first one is posted more recently than second
//or if they were post on same date
bool user_feed::compare_date(one_post first, one_post second){
    if(year(first) == year(second)){
        if(month(first) == month(second)){
            if(date(first) == date(second)){
                return true;
            }
            return date(first) > date(second);
        }
        return month(first) > month(second);
    }
    return year(first) > year(second);
}

std::vector<std::string> user_feed::all_hashtags(){
    QSqlDatabase& db = myAuthentication.m_dbAccess.getDatabase();
    QSqlQuery query(db);
    std::vector<std::string> hashtags;

    // Retrieve all hashtag names from the database
    query.prepare("SELECT name FROM Hashtags");
    if (!query.exec()){
        qCritical() << "Failed to execute query:" << query.lastError().text();
        return {};
    }

    // Iterate over the query results and add each hashtag name to the vector
    while(query.next()){
        QString name = query.value(0).toString();
        hashtags.push_back(name.toStdString());
    }

    return hashtags;
}


std::vector<one_post> user_feed::post_by_specific_hashtag(QString hashtag){
    QSqlDatabase& db = myAuthentication.m_dbAccess.getDatabase();
    QSqlQuery query(db);
    std::vector<one_post> posts;

    // Converting the hashtag to lowercase
    std::string temp = hashtag.toStdString();
    std::string name;
    char arr[temp.length()+1];
    strcpy(arr, temp.c_str());
    for(int x = 0; x < strlen(arr); x++){
        if(64 < arr[x] & arr[x] < 91){
            name+= (arr[x]+32);
        }
        else{
            name+= (arr[x]);
        }
    }

    std::vector<int> post_id;

    // Retrieving post IDs that match the specified hashtag from the database
    query.prepare("SELECT post_id FROM PostHashtags WHERE hashtag_id = "
                  "(SELECT hashtag_id FROM Hashtags WHERE name = :name)");
    query.bindValue(":name", QString::fromStdString(name));
    if (!query.exec()){
        qCritical() << "Failed to execute query:" << query.lastError().text();
        return {};
    }
    while(query.next()){
        int post = query.value(0).toInt();
        post_id.push_back(post);
    }

    // Retrieving post details based on the retrieved post IDs
    for(int i = 0; i < post_id.size(); i++){
        query.prepare("SELECT post_id, author_id, title, content, timestamp FROM Posts "
                      "WHERE post_id = :post_id");
        query.bindValue(":post_id", post_id.at(i));
        if (!query.exec()){
            qCritical() << "Failed to execute query:" << query.lastError().text();
            return {};
        }
        while(query.next()){
            int post_id = query.value(0).toInt();
            int author_id = query.value(1).toInt();
            QString title = query.value(2).toString();
            QString content = query.value(3).toString();
            QString timestamp = query.value(4).toString();
            one_post temp2 = one_post(post_id, author_id, title, content, timestamp);
            posts.push_back(temp2);
        }
    }

    // Filter out shadowbanned posts from the result
    return filterShadowbannedPosts(posts);
}

bool user_feed::isUserShadowbanned(int user_id) {
    QSqlDatabase& db = myAuthentication.m_dbAccess.getDatabase();
    QSqlQuery query(db);

    query.prepare("SELECT shadowbanned FROM Users WHERE user_id = :user_id");
    query.bindValue(":user_id", user_id);

    if (!query.exec()) {
        qCritical() << "Failed to check shadowbanned status:" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        return query.value(0).toBool();
    }

    return false;
}


// Functions to extract specific components (year, month, date, hours, minutes, seconds) from a post's timestamp
std::vector<one_post> user_feed::filterShadowbannedPosts(const std::vector<one_post>& allPosts) {
    std::vector<one_post> nonShadowbannedPosts;

    for (const one_post& post : allPosts) {
        int author_id = post.return_authorId();

        if (!isUserShadowbanned(author_id)) {
            nonShadowbannedPosts.push_back(post);
        }
    }

    return nonShadowbannedPosts;
}


int user_feed::year(one_post data){
    return stoi(data.return_timestamp().toStdString().substr(0,4));
}

int user_feed::month(one_post data){
    return stoi(data.return_timestamp().toStdString().substr(5,2));
}

int user_feed::date(one_post data){
    return stoi(data.return_timestamp().toStdString().substr(8,2));
}

int user_feed::hours(one_post data){
    return stoi(data.return_timestamp().toStdString().substr(11,2));
}

int user_feed::minutes(one_post data){
    return stoi(data.return_timestamp().toStdString().substr(14,2));
}

int user_feed::seconds(one_post data){
    return stoi(data.return_timestamp().toStdString().substr(17,2));
}
