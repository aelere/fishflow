#ifndef LOGIN_USER_H
#define LOGIN_USER_H


#include <QDialog>
#include "../authentication_engine/auth_manager.h"
#include "qmainwindow.h"
#include <QMessageBox>
#include <QRegularExpression>

namespace Ui {
class login_user;
}

// Class: login_user
// ---------------------
// This class represents a window for logging in as a user. It allows the user to
// enter their username and password in the input fields and click the submit button to log in.
// If login successful, the main_feed window is opened.
// If the authentication fails, an error message is shown.
class login_user : public QDialog
{
    Q_OBJECT

public:
    // Constructor
    explicit login_user(auth_manager& manager, QMainWindow* mainWindow, QWidget *parent = nullptr);

    // Destructor
    ~login_user();

private slots:

    // Slot for clicking cancel button. The window closes, going back to mainwindow.
    void on_cancelButton_clicked();

    // This method is triggered when the submit button is clicked. It attempts to authenticate the entered
    // username and password.
    void on_submitButton_clicked();

private:
    Ui::login_user *ui;
    auth_manager& myManager;
    QMainWindow* mainWindow;
};

#endif // LOGIN_USER_H
