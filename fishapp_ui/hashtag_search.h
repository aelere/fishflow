#ifndef HASHTAG_SEARCH_H
#define HASHTAG_SEARCH_H

#include <QMainWindow>
#include "../authentication_engine/auth_manager.h"
#include "../feed/user_feed.h"

namespace Ui {
class hashtag_search;
}

// Class: hashtag_search
// ---------------------
// This class represents a window for searching posts by a specific hashtag. It allows the user to
// enter a hashtag in the input field and click the submit button to search for posts containing that hashtag.
// If posts with the given hashtag are found, the hashtag_feed window is opened with the matching posts.

class hashtag_search : public QMainWindow
{
    Q_OBJECT

public:
    // Constructor
    explicit hashtag_search(QMainWindow* mainWindow, QWidget *parent, auth_manager& userAuthenticaiton);
    // Destructor
    ~hashtag_search();

private slots:
    void on_submitButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::hashtag_search *ui;
    auth_manager& myManager;
    user_feed myFeed;
    QMainWindow* mainWindow;
};

#endif // HASHTAG_SEARCH_H
