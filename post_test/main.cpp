#include <iostream>
#include "gtest/gtest.h"
#include "../authentication_engine/user.h"
#include "../authentication_engine/auth_manager.h"
#include "../post/one_post.h"
#include "../post/upload.h"

// The fixture for testing class Foo.
class PostTest : public ::testing::Test {
    protected:

    db_access myDb;
    auth_manager authManager;
    upload myUpload;

    PostTest(): authManager(myDb), myUpload(authManager) {

    }
    virtual ~PostTest() {

    }

    virtual void SetUp() {

    }
    virtual void TearDown() {

    }
    };

    TEST_F(PostTest, sendPost) {
        authManager.createUser("testuser1", "first", "last", "email1@example.com", "2023-04-04", "testpassword", 1200);
        authManager.createUser("testuser2", "first", "last", "email2@example.com", "2023-04-04", "testpassword", 1201);
        authManager.authenticate("testuser1", "testpassword");

        //checking that post is sending properly and hashtag
        //is made to lowercase
        std::vector<std::string> post_hashtags;
        post_hashtags.push_back("FISHY");
        post_hashtags.push_back("CoLd WaTeR");
        EXPECT_TRUE(myUpload.send("title1", "content1", post_hashtags));

        //on new post with same hashtags, uses old hashtag_ids
        std::vector<std::string> post_hashtags2;
        post_hashtags2.push_back("fishy");
        post_hashtags2.push_back("cold water");
        post_hashtags2.push_back("warm water");
        EXPECT_TRUE(myUpload.send("title2", "content2", post_hashtags2));
        if(true){

        }
        authManager.removeUser(1200);
        authManager.removeUser(1201);
    }

    TEST_F(PostTest, findPosts) {
        authManager.createUser("testuser1", "first", "last", "email1@example.com", "2023-04-04", "testpassword", 1200);
        authManager.createUser("testuser2", "first", "last", "email2@example.com", "2023-04-04", "testpassword", 1201);
        authManager.authenticate("testuser1", "testpassword");
        std::vector<std::string> post_hashtags;
        post_hashtags.push_back("FISHY");
        post_hashtags.push_back("CoLd WaTeR");

        //starts at 0
        std::vector<one_post> all_posts;
        all_posts = myUpload.findPosts();
        EXPECT_TRUE(all_posts.size() == 0);

        //increases as posts are added
        myUpload.send("title1", "content1", post_hashtags);
        all_posts = myUpload.findPosts();
        EXPECT_TRUE(all_posts.size() == 1);

        //increases as posts are added
        myUpload.send("title1", "content1", post_hashtags);
        all_posts = myUpload.findPosts();
        EXPECT_TRUE(all_posts.size() == 2);

        authManager.logout();
        authManager.authenticate("testuser2", "testpassword");
        //starts at 0 for different
        std::vector<one_post> all_posts2;
        all_posts2 = myUpload.findPosts();
        EXPECT_TRUE(all_posts2.size() == 0);

        //increases as posts are added
        myUpload.send("title1", "content1", post_hashtags);
        all_posts2 = myUpload.findPosts();
        EXPECT_TRUE(all_posts2.size() == 1);

        //increases as posts are added
        myUpload.send("title1", "content1", post_hashtags);
        all_posts2 = myUpload.findPosts();
        EXPECT_TRUE(all_posts2.size() == 2);


        authManager.removeUser(1200);
        authManager.removeUser(1201);
    }

    TEST_F(PostTest, reportPost) {
        authManager.createUser("testuser1", "first", "last", "email1@example.com", "2023-04-04", "testpassword", 1200);
        authManager.createUser("testuser2", "first", "last", "email2@example.com", "2023-04-04", "testpassword", 1201);
        authManager.authenticate("testuser1", "testpassword");

        std::vector<std::string> post_hashtags;
        post_hashtags.push_back("FISHY");
        post_hashtags.push_back("CoLd WaTeR");

        myUpload.send("title1", "content1", post_hashtags);


        authManager.removeUser(1200);
        authManager.removeUser(1201);
    }

    TEST_F(PostTest, removePost) {
        authManager.createUser("testuser1", "first", "last", "email1@example.com", "2023-04-04", "testpassword", 1200);
        authManager.createUser("testuser2", "first", "last", "email2@example.com", "2023-04-04", "testpassword", 1201);
        authManager.authenticate("testuser1", "testpassword");

        authManager.removeUser(1200);
        authManager.removeUser(1201);
    }

    TEST_F(PostTest, deletePost) {
        authManager.createUser("testuser1", "first", "last", "email1@example.com", "2023-04-04", "testpassword", 1200);
        authManager.createUser("testuser2", "first", "last", "email2@example.com", "2023-04-04", "testpassword", 1201);
        authManager.authenticate("testuser1", "testpassword");

        authManager.removeUser(1200);
        authManager.removeUser(1201);
    }

    TEST_F(PostTest, findHashtags) {
        authManager.createUser("testuser1", "first", "last", "email1@example.com", "2023-04-04", "testpassword", 1200);
        authManager.createUser("testuser2", "first", "last", "email2@example.com", "2023-04-04", "testpassword", 1201);
        authManager.authenticate("testuser1", "testpassword");

        authManager.removeUser(1200);
        authManager.removeUser(1201);
    }

    int main(int argc, char **argv) {
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    }
