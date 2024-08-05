#include <iostream>
#include "gtest/gtest.h"
#include "../authentication_engine/user.h"
#include "../authentication_engine/auth_manager.h"
#include "../messaging/conversation.h"
#include "../messaging/one_conversation.h"
#include "../messaging/one_message.h"

// The fixture for testing class Foo.
class MessagingTest : public ::testing::Test {
protected:

    db_access myDb;
    auth_manager authManager;
    conversation myConversation;

    MessagingTest(): authManager(myDb), myConversation(authManager) {
        //adding users for the tests
        authManager.createUser("testuser1", "first", "last", "email1@example.com", "2023-04-04", "testpassword", 1200);
        authManager.createUser("testuser2", "first", "last", "email2@example.com", "2023-04-04", "testpassword", 1201);
        authManager.createUser("testuser3", "first", "last", "email3@example.com", "2023-04-04", "testpassword", 1202);
        authManager.createUser("testuser4", "first", "last", "email4@example.com", "2023-04-04", "testpassword", 1203);

    }
    virtual ~MessagingTest() {
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

    TEST_F(MessagingTest, createDeleteConversationTest) {
        authManager.authenticate("testuser1", "testpassword");

        //creating first conversation with another user
        EXPECT_TRUE(myConversation.create_conversation(1201));

        //trying to create conversation again will fail
        EXPECT_FALSE(myConversation.create_conversation(1201));

        //creating conversation with different users
        EXPECT_TRUE(myConversation.create_conversation(1202));
        EXPECT_TRUE(myConversation.create_conversation(1203));

        //changing user
        authManager.logout();
        authManager.authenticate("testuser2", "testpassword");

        //false since other user created the conversation already
        EXPECT_FALSE(myConversation.create_conversation(1200));

        //creating conversation with different users
        EXPECT_TRUE(myConversation.create_conversation(1202));
        EXPECT_TRUE(myConversation.create_conversation(1203));

        //deleting conversations
        EXPECT_TRUE(myConversation.delete_conversation(1200));
        EXPECT_TRUE(myConversation.delete_conversation(1202));
        EXPECT_TRUE(myConversation.delete_conversation(1203));
        authManager.logout();
        authManager.authenticate("testuser1", "testpassword");
        EXPECT_TRUE(myConversation.delete_conversation(1202));
        EXPECT_TRUE(myConversation.delete_conversation(1203));

        //will be false since other user deleted conversation
        EXPECT_FALSE(myConversation.delete_conversation(1201));
};

    TEST_F(MessagingTest, sendMessageTest) {
        authManager.authenticate("testuser1", "testpassword");

        //return false since conversation has not been created between the users
        EXPECT_FALSE(myConversation.send_message(1201, "hello"));

        //return true since conversation is created
        myConversation.create_conversation(1201);
        EXPECT_TRUE(myConversation.send_message(1201, "hello"));

        //testing that multiple messages send
        EXPECT_TRUE(myConversation.send_message(1201, "hi"));
        EXPECT_TRUE(myConversation.send_message(1201, "testing"));
        EXPECT_TRUE(myConversation.send_message(1201, "test"));

        //return false since conversation has not been created between this other user
        EXPECT_FALSE(myConversation.send_message(1202, "hello"));

        //return true since conversation is created
        myConversation.create_conversation(1202);
        EXPECT_TRUE(myConversation.send_message(1202, "hello"));

        //testing that multiple messages send
        EXPECT_TRUE(myConversation.send_message(1202, "hi"));
        EXPECT_TRUE(myConversation.send_message(1202, "testing"));
        EXPECT_TRUE(myConversation.send_message(1202, "test"));

        //testing that going switching between conversations works
        EXPECT_TRUE(myConversation.send_message(1201, "hello"));
        EXPECT_TRUE(myConversation.send_message(1202, "test"));
        EXPECT_TRUE(myConversation.send_message(1201, "back"));
        EXPECT_TRUE(myConversation.send_message(1202, "hi"));

        //deleting conversations
        EXPECT_TRUE(myConversation.delete_conversation(1201));
        EXPECT_TRUE(myConversation.delete_conversation(1202));

    }

    TEST_F(MessagingTest, allConversationsTest) {
        authManager.authenticate("testuser1", "testpassword");
        std::vector<one_conversation> all_conversations = myConversation.all_conversations();

        //at first there should be no converstaions
        EXPECT_TRUE(all_conversations.size() == 0);


        myConversation.create_conversation(1201);
        all_conversations = myConversation.all_conversations();
        EXPECT_TRUE(all_conversations.size() == 1);

        myConversation.create_conversation(1202);
        all_conversations = myConversation.all_conversations();
        EXPECT_TRUE(all_conversations.size() == 2);

        myConversation.create_conversation(1203);
        all_conversations = myConversation.all_conversations();
        EXPECT_TRUE(all_conversations.size() == 3);

        //changing user
        authManager.logout();
        authManager.authenticate("testuser2", "testpassword");

        //should start out at 1 since other user started conversation with this user
        all_conversations = myConversation.all_conversations();
        EXPECT_TRUE(all_conversations.size() == 1);

        myConversation.create_conversation(1202);
        all_conversations = myConversation.all_conversations();
        EXPECT_TRUE(all_conversations.size() == 2);

        myConversation.create_conversation(1203);
        all_conversations = myConversation.all_conversations();
        EXPECT_TRUE(all_conversations.size() == 3);

        //deleting conversations
        EXPECT_TRUE(myConversation.delete_conversation(1200));
        EXPECT_TRUE(myConversation.delete_conversation(1202));
        EXPECT_TRUE(myConversation.delete_conversation(1203));
        authManager.logout();
        authManager.authenticate("testuser1", "testpassword");
        EXPECT_TRUE(myConversation.delete_conversation(1202));
        EXPECT_TRUE(myConversation.delete_conversation(1203));

    }

    TEST_F(MessagingTest, allMessagesTest) {
        authManager.authenticate("testuser1", "testpassword");
        std::vector<one_message> all_messages;

        //initially there are 0 messages
        myConversation.create_conversation(1201);
        all_messages = myConversation.all_messages(1201);
        EXPECT_TRUE(all_messages.size() == 0);

        //size should increase as new messages are sent
        myConversation.send_message(1201, "hello");
        all_messages = myConversation.all_messages(1201);
        EXPECT_TRUE(all_messages.size() == 1);

        myConversation.send_message(1201, "hello");
        all_messages = myConversation.all_messages(1201);
        EXPECT_TRUE(all_messages.size() == 2);

        myConversation.send_message(1201, "hello");
        all_messages = myConversation.all_messages(1201);
        EXPECT_TRUE(all_messages.size() == 3);

        //should not change size of messages with different user
        myConversation.create_conversation(1202);
        myConversation.send_message(1202, "hi");
        all_messages = myConversation.all_messages(1201);
        EXPECT_TRUE(all_messages.size() == 3);

        authManager.logout();
        authManager.authenticate("testuser2", "testpassword");

        //should be 3 becuase other user sent messages
        all_messages = myConversation.all_messages(1200);
        EXPECT_TRUE(all_messages.size() == 3);

        //updaes for new message
        myConversation.send_message(1200, "hello");
        all_messages = myConversation.all_messages(1200);
        EXPECT_TRUE(all_messages.size() == 4);

        //deleting conversations
        myConversation.delete_conversation(1200);
        authManager.logout();
        authManager.authenticate("testuser1", "testpassword");
        myConversation.delete_conversation(1202);
    }

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
