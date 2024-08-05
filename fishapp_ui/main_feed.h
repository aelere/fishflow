#ifndef MAIN_FEED_H
#define MAIN_FEED_H

#include <QDialog>
#include "../post/one_post.h"
#include "../feed/user_feed.h"
#include "../authentication_engine/auth_manager.h"
#include "../authentication_engine/user.h"
#include "qmainwindow.h"


namespace Ui {
class main_feed;
}

// Class: main_feed
// ---------------------
// This class represents the main feed window where users can interact with FishFlow.
// It includes functionalities to view posts, go to user's profile, create a post, search by hashtag,
// navigate through posts, report a post, and log out. It also supports anonymous usage, where certain
// features are disabled.
class main_feed : public QDialog
{
    Q_OBJECT

public:
   // Constructor
   explicit main_feed(QMainWindow* mainWindow, auth_manager &userAuthenticaiton, QWidget *parent = nullptr);
   // Destructor
   explicit main_feed(bool anonymous, QMainWindow* mainWindow, auth_manager& userAuthenticaiton, QWidget *parent = nullptr);
    ~main_feed();

private slots:

    // These methods are triggered when the corresponding buttons are clicked. They provide functionalities
    // such as viewing the feed, checking messages, going to user's profile, logging out, creating a post,
    // searching by hashtag, navigating through posts, reporting a post, and going to a user's profile.
    void on_feed_clicked();

    void on_messages_clicked();

    void on_profile_clicked();

    void load_post();

    void on_log_out_clicked();

    void on_create_post_clicked();

    void on_search_hashtag_clicked();

    void on_previous_post_clicked();

    void on_next_post_clicked();

    void on_report_clicked();

    void updateFeed();

    void on_go_to_user_clicked();

private:
    Ui::main_feed *ui;
    std::vector<one_post> myPosts;
    user* currentUser;
    user_feed myFeed;
    int position;
    auth_manager& myManager;
    QMainWindow* mainWindow;
    bool anonymous;
};

#endif // MAIN_FEED_H
