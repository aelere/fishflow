#ifndef VISITING_USER_PROFILE_H
#define VISITING_USER_PROFILE_H

#include <QMainWindow>
#include "../post/one_post.h"
#include "../authentication_engine/user.h"
#include "../authentication_engine/auth_manager.h"
#include "../post/upload.h"

namespace Ui {
class visiting_user_profile;
}
// Class: visiting_user_profile
// ---------------------
// The visiting_user_profile UI enables Users to see other people's profiles.
// Very similar to user profile, but instead of removing posts, the user can report them.


class visiting_user_profile : public QMainWindow
{
    Q_OBJECT

public:
    explicit visiting_user_profile(QMainWindow* mainWindow, QWidget *parent,  auth_manager& userAuthenticaiton, int other_user_id);
    ~visiting_user_profile();

private slots:
    void on_report_post_clicked();

    void on_previous_post_clicked();

    void on_next_post_clicked();

    void on_log_out_clicked();

    void on_conversation_clicked();

    void on_messages_clicked();

    void on_feed_clicked();

    void on_profile_clicked();

    void load_post();

private:
    Ui::visiting_user_profile *ui;
    std::vector<one_post> otherPosts;
    user* currentUser;
    int position;
    auth_manager& myManager;
    upload myUpload;
    QMainWindow* mainWindow;
    user* otherUser;
};

#endif // VISITING_USER_PROFILE_H
