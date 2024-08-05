#ifndef USER_PROFILE_H
#define USER_PROFILE_H

#include <QDialog>
#include "../post/one_post.h"
#include "../post/upload.h"
#include "QtWidgets/qmainwindow.h"

namespace Ui {
class user_profile;
}

// Class: user_profile
// ---------------------
// The user_profile UI provides each FishFlow user with their unique
// posts. Through this class, each user has their own profile "feed"
// used to display all their posts. Users can choose to cycle through
// their posts, remove the ones they do not lile, or navigate to other
// parts of the program.

class user_profile : public QDialog
{
    Q_OBJECT

public:
    // Constructor
    explicit user_profile(QMainWindow* mainWindow, QWidget *parent,  auth_manager& userAuthenticaiton);

    // Destructor
    ~user_profile();

private slots:
    // Taking user back to main feed
    void on_feed_clicked();

    // Taking user to messaging
    void on_messages_clicked();

    // Taking user to the same secreen
    void on_profile_clicked();

    // Taking user to their settings
    void on_settings_clicked();

    // Enabling user to log out
    void on_log_out_clicked();


    void on_previous_post_clicked();

    void on_next_post_clicked();

    // Enabling user to delete their posts.
    void on_delete_post_clicked();

    // Method used to load the post data on the UI.
    void load_post();

signals:
    void postRemoved();

private:
    Ui::user_profile *ui;

    // Vector used to store all of the current user's posts.
    std::vector<one_post> myPosts;
    user* currentUser;

    // The index of the displayed post from myPosts.
    int position;
    auth_manager& myManager;

    // Upload object managing post upload/removal
    upload myUpload;
    QMainWindow* mainWindow;
};

#endif // USER_PROFILE_H
