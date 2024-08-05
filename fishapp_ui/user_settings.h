#ifndef USER_SETTINGS_H
#define USER_SETTINGS_H

#include <QMainWindow>
#include "../authentication_engine/user.h"
#include "../authentication_engine/auth_manager.h"

namespace Ui {
class user_settings;
}
// Class: user_settings
// ---------------------
// The user_settings class enables the user to change their
// information on the platform. Additional features (such as
// switching to anonymous user, for example) are to be implemented.
class user_settings : public QMainWindow
{
    Q_OBJECT

public:
    explicit user_settings(QMainWindow* mainWindow, QWidget *parent,  auth_manager& userAuthenticaiton);
    ~user_settings();

private slots:
    void on_profile_clicked();

    void on_info_clicked();

    void on_my_feed_clicked();

    void on_anonymous_clicked();

    void on_feed_status_clicked();

    void on_messaging_clicked();

    void on_new_messaging_clicked();

private:
    Ui::user_settings *ui;
    user* currentUser;
    auth_manager& myManager;
    QMainWindow* mainWindow;
};

#endif // USER_SETTINGS_H
