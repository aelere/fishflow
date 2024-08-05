#ifndef USER_FEED_H
#define USER_FEED_H
#include "../authentication_engine/auth_manager.h"
#include "../post/one_post.h"


// Class: report_submission
// ---------------------
// The report_submission class provides a submission form to users.
// It enables users to write the reason why they think the post should
// be removed, which gets sent to the moderator_feed for mods to review.
class user_feed
{
public:
    user_feed(auth_manager& userAuthentication);//db_access& dbAccess, user& userAccess);

    // Method returning all the posts unordered in a vector.
    std::vector<one_post> all_posts();

    // Method returning all the posts sorted by hashtag relevancy first, then date.
    std::vector<one_post> posts_by_hashtag();

    // Method sorting the posts with respect to date.
    std::vector<one_post> sort(std::vector<one_post> initial);

    // Method returning information about the post from the database
    std::vector<std::string> findInformation(int postId);

    // Method retrieving all existent hashtags (unordered)
    std::vector<std::string> all_hashtags();

    // Method for retrieving all posts that use a specific hashtag
    std::vector<one_post> post_by_specific_hashtag(QString hashtag);




private:
    // Method that compares two posts to determine which should come first.
    bool compare(one_post first, one_post second);

    // Methodss to extract specific components (year, month, date, hours, minutes, seconds) from a post's timestamp
    bool compare_date(one_post first, one_post second);
    int year(one_post data);
    int month(one_post data);
    int date(one_post data);
    int hours(one_post data);
    int minutes(one_post data);
    int seconds(one_post data);

    std::vector<one_post> filterShadowbannedPosts(const std::vector<one_post>& allPosts);
    bool isUserShadowbanned(int user_id);

//    db_access& m_dbAccess;
//    user& myUser;
    auth_manager& myAuthentication;

};

#endif // USER_FEED_H
