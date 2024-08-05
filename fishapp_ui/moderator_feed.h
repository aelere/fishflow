#ifndef MODERATOR_FEED_H
#define MODERATOR_FEED_H

#include <QDialog>
#include <QMessageBox>
#include "qmainwindow.h"
#include "../moderation/moderation_manager.h"
#include "../moderation/moderator.h"
#include "../post/one_post.h"

namespace Ui {
class moderator_feed;
}
// Class: moderator_feed
// ---------------------
// The moderator_feed class provides a User Interface for moderators.
// It contains the necessary functionality to navigate and manage reports of posts and users.

class moderator_feed : public QDialog
{
    Q_OBJECT

public:
    explicit moderator_feed(QMainWindow* mainWindow, moderation_manager &userAuthenticaiton, QWidget *parent = nullptr);
    ~moderator_feed();

    void load_report();

private slots:

    // Slots for each button.
    void on_previousButton_clicked();

    void on_nextButton_clicked();

    void on_pushButton_8_clicked();

    void on_banUserButton_clicked();

    void on_removePostButton_clicked();

    void updateFeed();

    void on_shadowbanButton_clicked();

private:
    Ui::moderator_feed *ui;
    moderator* currentModerator;
    int position;
    moderation_manager& myModManager;
    QMainWindow* mainWindow;
    std::vector<std::vector<std::string>> allReports;
};

#endif // MODERATOR_FEED_H
