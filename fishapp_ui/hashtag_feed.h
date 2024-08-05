#ifndef HASHTAG_FEED_H
#define HASHTAG_FEED_H

#include <QMainWindow>
#include <QDialog>
#include "../post/one_post.h"
#include "../feed/user_feed.h"
#include "../authentication_engine/auth_manager.h"
#include "../authentication_engine/user.h"
#include "qmainwindow.h"

namespace Ui {
class hashtag_feed;
}

// Class: hashtag_feed
// ------------------------
// This class represents a hashtag feed window that displays posts related to a specific hashtag.
// It extends QMainWindow.
class hashtag_feed : public QMainWindow
{
    Q_OBJECT

public:
    explicit hashtag_feed(QMainWindow* mainWindow, QWidget *parent,  auth_manager& userAuthenticaiton, std::vector<one_post> posts);
    ~hashtag_feed();

private slots:

    void on_feed_clicked();

    void load_post();

    void on_log_out_clicked();

    void on_previous_post_clicked();

    void on_next_post_clicked();

    void on_report_clicked();

    void on_go_clicked();

private:
    Ui::hashtag_feed *ui;
    std::vector<one_post> myPosts;      // Vector of one_post objects representing the posts related to the hashtag
    user* currentUser;                  // Pointer to the current user object
    user_feed myFeed;                   // user_feed object for managing user's feed
    int position;                       // Current position/index of the displayed post in the myPosts vector
    auth_manager& myManager;            // Reference to the auth_manager object for user authentication and management
    QMainWindow* mainWindow;
};

#endif // HASHTAG_FEED_H
