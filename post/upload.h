#ifndef UPLOAD_H
#define UPLOAD_H

#include <string>
#include <vector>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlquery.h>
#include <iomanip>
#include <ctime>
#include <sstream>
#include "../authentication_engine/auth_manager.h"
#include "../authentication_engine/user.h"
#include "one_post.h"

// Class: upload
// ---------------------
// The upload class provides functionality for uploading and managing posts in the system.

class upload
{
public:
    upload(auth_manager& userAuthentication);

    // Method to send a post with a title, content, and hashtags
    bool send(std::string title, std::string content, std::vector<std::string> hashtags);

    // Method to find all posts
    std::vector<one_post> findPosts();

    // Method to find all posts by a specific user
    std::vector<one_post> other_findPosts(int user_id);

    // Method to report a post with a reason
    bool report_post(int post_id, std::string reason);

    // Method to remove a post by a moderator with a removal reason
    bool remove_post(int post_id, int moderator_id, std::string removal_reason);

    // Method to delete a post
    bool delete_post(int post_id);

    // Method to find all existing hashtags
    std::vector<std::string> find_hashtags();

    // Method to find information about a post
    std::vector<std::string> findInformation(int postId);

    // Method to get information about another user
    user* getOtherUser(int user_id);

private:
    auth_manager& myAuthentication;
};

#endif // UPLOAD_H
