#include <iostream>
#include "gtest/gtest.h"
#include "../authentication_engine/user.h"
#include "../authentication_engine/auth_manager.h"
#include "../post/one_post.h"
#include "../post/upload.h"
#include "../feed/user_feed.h"

class FeedTest : public ::testing::Test {
protected:

    db_access myDb;
    auth_manager authManager;
    user_feed myFeed;
    upload myUpload;

    FeedTest(): authManager(myDb), myFeed(authManager), myUpload(authManager) {
        //adding users for the tests
        authManager.createUser("testuser1", "first", "last", "email1@example.com", "2023-04-04", "testpassword", 1200);
        authManager.createUser("testuser2", "first", "last", "email2@example.com", "2023-04-04", "testpassword", 1201);
        authManager.createUser("testuser3", "first", "last", "email3@example.com", "2023-04-04", "testpassword", 1202);
        authManager.createUser("testuser4", "first", "last", "email4@example.com", "2023-04-04", "testpassword", 1203);

    }
    virtual ~FeedTest() {
        authManager.removeUser(1200);
        authManager.removeUser(1201);
        authManager.removeUser(1202);
        authManager.removeUser(1203);
    }

    virtual void SetUp() {

    }
    virtual void TearDown() {

    }
};

TEST_F(FeedTest, allPosts) {
    //logging in with a test user
    authManager.createUser("testuser1", "first", "last", "email1@example.com", "2023-04-04", "testpassword", 1200);
    authManager.createUser("testuser2", "first", "last", "email1@example.com", "2023-04-04", "testpassword", 1201);
    authManager.authenticate("testuser1", "testpassword");

    //finding the initial number of posts
    std::vector<one_post> temp = myFeed.all_posts();
    int initial = temp.size();

    //adding a post
    std::vector<std::string> post_hashtags;
    post_hashtags.push_back("fish");
    myUpload.send("title1", "content1", post_hashtags);

    //checking that the method still returns the right number of posts
    temp = myFeed.all_posts();
    EXPECT_TRUE(temp.size() == (initial + 1));

    //adding another post
    myUpload.send("title1", "content1", post_hashtags);

    //checking that the method still returns the right number of posts
    temp = myFeed.all_posts();
    EXPECT_TRUE(temp.size() == (initial + 2));

    //deleteing test user
    authManager.removeUser(1200);
    authManager.removeUser(1201);
}



TEST_F(FeedTest, sort) {
    //logging in with a test user
    authManager.createUser("testuser1", "first", "last", "email1@example.com", "2023-04-04", "testpassword", 1200);
    authManager.createUser("testuser2", "first", "last", "email1@example.com", "2023-04-04", "testpassword", 1201);
    authManager.authenticate("testuser1", "testpassword");

    //no two posts can be created at the exact same second so that does not need to be tested
    //creating 7 posts where the name of the object is the order it should be sorted into
    one_post first = one_post(12345, 12345, "title", "content", "2023-05-14-19-04-31");
    one_post second = one_post(12345, 12345, "title", "content", "2023-05-14-19-04-30");
    one_post third = one_post(12345, 12345, "title", "content", "2023-05-14-19-03-30");
    one_post fourth = one_post(12345, 12345, "title", "content", "2023-05-14-18-03-30");
    one_post fifth = one_post(12345, 12345, "title", "content", "2023-05-13-18-03-30");
    one_post sixth = one_post(12345, 12345, "title", "content", "2023-04-13-18-03-30");
    one_post seventh = one_post(12345, 12345, "title", "content", "2022-04-13-18-03-30");
    std::vector<one_post> temp;
    temp.push_back(third);
    temp.push_back(seventh);
    temp.push_back(sixth);
    temp.push_back(first);
    temp.push_back(fourth);
    temp.push_back(second);
    temp.push_back(fifth);

    //checking that sort properly sorts all the posts
    std::vector<one_post> final = myFeed.sort(temp);
    EXPECT_TRUE(final.at(0).return_timestamp() == first.return_timestamp());
    EXPECT_TRUE(final.at(1).return_timestamp() == second.return_timestamp());
    EXPECT_TRUE(final.at(2).return_timestamp() == third.return_timestamp());
    EXPECT_TRUE(final.at(3).return_timestamp() == fourth.return_timestamp());
    EXPECT_TRUE(final.at(4).return_timestamp() == fifth.return_timestamp());
    EXPECT_TRUE(final.at(5).return_timestamp() == sixth.return_timestamp());
    EXPECT_TRUE(final.at(6).return_timestamp() == seventh.return_timestamp());

    //deleteing test user
    authManager.removeUser(1200);
    authManager.removeUser(1201);
}

TEST_F(FeedTest, allHashtags) {
    //logging in with a test user
    authManager.createUser("testuser1", "first", "last", "email1@example.com", "2023-04-04", "testpassword", 1200);
    authManager.createUser("testuser2", "first", "last", "email1@example.com", "2023-04-04", "testpassword", 1201);
    authManager.authenticate("testuser1", "testpassword");
    srand(time(NULL));

    //finding initial number of hashtags
    std::vector<std::string> temp = myFeed.all_hashtags();
    int initial = temp.size();

    //creating a random string
    char Alphabets[26] = {'a','b','c','d','e','f','g','h',
       'i','j','k','l','m','n','o','p','q',
       'r','s','t','u','v','w','x','y','z'};
      std::string RandString;
      int k=0;
      while(k<10) {
        int temp = rand() % 26;
        RandString += Alphabets[temp];
        k++;}

    //adding a post with a new random hashtag
    std::vector<std::string> post_hashtags;
    post_hashtags.push_back(RandString);
    myUpload.send("title1", "content1", post_hashtags);

    //the number of hashtags should have increased by one
    temp = myFeed.all_hashtags();
    EXPECT_TRUE(temp.size() == (initial + 1));

    //adding another post with the same hashtag
    myUpload.send("title2", "content2", post_hashtags);

    //the number of hashtags should not have increased
    temp = myFeed.all_hashtags();
    EXPECT_TRUE(temp.size() == (initial + 1));

    //creating another random string
      std::string RandString2;
      int i=0;
      while(i<10) {
        int temp = rand() % 26;
        RandString2 += Alphabets[temp];
        i++;}

    //adding a different post with a new hashtag
    std::vector<std::string> post_hashtags2;
    post_hashtags2.push_back(RandString2);
    myUpload.send("title1", "content1", post_hashtags2);

    //the number of hashtags should have increased by two since initial was initialized
    temp = myFeed.all_hashtags();
    EXPECT_TRUE(temp.size() == (initial + 2));

    //deleteing test user
    authManager.removeUser(1200);
    authManager.removeUser(1201);
}

TEST_F(FeedTest, postBySpecificHashtag) {
    //logging in with a test user
    authManager.createUser("testuser1", "first", "last", "email1@example.com", "2023-04-04", "testpassword", 1200);
    authManager.createUser("testuser2", "first", "last", "email2@example.com", "2023-04-04", "testpassword", 1201);
    authManager.authenticate("testuser1", "testpassword");
    srand(time(NULL));

    //testing that a hashtag with no posts returns no posts
    std::vector<one_post> temp = myFeed.post_by_specific_hashtag("iwefhiuwehbfgiuwebfg");
    EXPECT_TRUE(temp.size() == 0);

    //adding a post with a random hastag
    char Alphabets[26] = {'a','b','c','d','e','f','g','h',
       'i','j','k','l','m','n','o','p','q',
       'r','s','t','u','v','w','x','y','z'};
      std::string RandString;
      int k=0;
      while(k<10) {
        int temp = rand() % 26;
        RandString += Alphabets[temp];
        k++;}
    std::vector<std::string> post_hashtags;
    post_hashtags.push_back(RandString);
    myUpload.send("title1", "content1", post_hashtags);

    //testing that all posts with the random hashtag are returned
    temp = myFeed.post_by_specific_hashtag(QString::fromStdString(RandString));
    EXPECT_TRUE(temp.size() == 1);

    //adding another post with the random hashtag hashtag
    myUpload.send("title2", "content2", post_hashtags);

    //testing that all posts with the random hashtag are returned
    temp = myFeed.post_by_specific_hashtag(QString::fromStdString(RandString));
    EXPECT_TRUE(temp.size() == 2);

    //adding a post with another random hastag
      std::string RandString2;
      int i=0;
      while(i<10) {
        int temp = rand() % 26;
        RandString2 += Alphabets[temp];
        i++;}
    std::vector<std::string> post_hashtags2;
    post_hashtags2.push_back(RandString2);
    myUpload.send("title3", "content3", post_hashtags2);

    //testing that the method works with another hashtag
    temp = myFeed.post_by_specific_hashtag(QString::fromStdString(RandString2));
    EXPECT_TRUE(temp.size() == 1);

    //logging into another user
    authManager.logout();
    authManager.authenticate("testuser2", "testpassword");

    //testing that other users find the same posts with given hashtags
    temp = myFeed.post_by_specific_hashtag(QString::fromStdString(RandString));
    EXPECT_TRUE(temp.size() == 2);
    temp = myFeed.post_by_specific_hashtag(QString::fromStdString(RandString2));
    EXPECT_TRUE(temp.size() == 1);

    //deleteing test user
    authManager.removeUser(1200);
    authManager.removeUser(1201);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
