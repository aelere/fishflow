#include <iostream>
#include "gtest/gtest.h"
#include "../authentication_engine/user.h"
#include "../authentication_engine/auth_manager.h"
#include "../moderation/moderation_manager.h"
#include "../moderation/moderator.h"

// The fixture for testing class Foo.
class ModAuthTest : public ::testing::Test {
protected:

    db_access myDb;
    moderation_manager modManager;
    auth_manager authManager;

    ModAuthTest(): modManager(myDb), authManager(myDb) {

    }
    virtual ~ModAuthTest() {

    }

    virtual void SetUp() {

    }
    virtual void TearDown() {

    }


};

TEST_F(ModAuthTest, createUserTest) {
    authManager.removeUser(600);
    authManager.removeUser(601);
    authManager.removeUser(602);
    EXPECT_TRUE(authManager.createUser("testuser", "Lasha", "Geladze", "lashageladze1@example.com", "2023-04-04", "testpassword", 600));

    //usernames match, shouldn't create
    EXPECT_FALSE(authManager.createUser("testuser", "Mari", "Gongadze", "marigongadze@example.com", "2023-04-04", "testpassword", 601));

    //emails match, shouldn't create
    EXPECT_FALSE(authManager.createUser("testuser2", "Mari", "Gongadze", "lashageladze1@example.com", "2023-04-04", "testpassword", 602));

    authManager.removeUser(600);
    authManager.removeUser(601);
    authManager.removeUser(602);
}

TEST_F(ModAuthTest, authenticateUpdateUserChangePasswordTest) {
    // Add user to database
    QString username = "testuser";
    QString firstName = "Mari";
    QString lastName = "Gongadze";
    QString email = "marigongadze@example.com";
    QString dateOfRegistration = "2023-04-04";
    QString password = "testpassword";
    ASSERT_TRUE(authManager.createUser(username, firstName, lastName, email, dateOfRegistration, password, 600));

    // Authentication with correct password
    ASSERT_TRUE(authManager.authenticate(username, password));

    // Authentication with incorrect password
    ASSERT_FALSE(authManager.authenticate(username, "wrongpassword"));

    // Authentication with non-existent user
    ASSERT_FALSE(authManager.authenticate("nonexistentuser", "password"));

    // Active user is set to the authenticated user.
    ASSERT_TRUE(authManager.getCurrentUser()->getId() == 600);

    authManager.logout();

    // Changing password
    ASSERT_TRUE(authManager.changePassword(username, "resetpassword"));

    // Login with new password
    ASSERT_TRUE(authManager.authenticate(username, "resetpassword"));

    authManager.logout();

    authManager.removeUser(600);
}

TEST_F(ModAuthTest, setAndCheckModeratorTest){
    QString testUsername = "gela";
    QString testFirstName = "Lasha";
    QString testLastName = "Geladze";
    QString testEmail = "lashageladze1@example.com";
    QString testDate = "2023-04-04";
    QString testPassword = "lasha123";
    int testId = 600;

    // Creating user
    ASSERT_TRUE(authManager.createUser(testUsername,testFirstName,testLastName, testEmail, testDate, testPassword, testId));

    // Assign moderator
    ASSERT_TRUE(modManager.assignModerator(testId));

    // Assert that the moderator exists
    ASSERT_TRUE(modManager.checkModerator(testId));

    // Assert with username
    ASSERT_TRUE(modManager.checkModerator(testUsername));

    // Remove the user from the database
    ASSERT_TRUE(authManager.removeUser(testId));

    // Removed should no longer be a moderator
    ASSERT_FALSE(modManager.checkModerator(testId));
}

TEST_F(ModAuthTest, authenticateModeratorTest) {

    EXPECT_TRUE(authManager.createUser("testuser", "Lasha", "Geladze", "lashageladze1@example.com", "2023-04-04", "testpassword", 600));

    // Make moderator
    modManager.assignModerator(600);

    ASSERT_TRUE(modManager.authenticateModerator("testuser", "testpassword"));

    ASSERT_TRUE(modManager.getCurrentModerator()->getModeratorId() == 600);

    modManager.logout();

    // Authentication with incorrect username
    ASSERT_FALSE(modManager.authenticateModerator("wronguser", "testpassword"));

    // Test authentication with incorrect password
    ASSERT_FALSE(modManager.authenticateModerator("testuser", "wrongpassword"));

    authManager.removeUser(600);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


