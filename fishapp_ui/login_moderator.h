#ifndef LOGIN_MODERATOR_H
#define LOGIN_MODERATOR_H

#include <QDialog>
#include "../moderation/moderation_manager.h"
#include "qmainwindow.h"
#include <QMessageBox>
#include <QRegularExpression>

namespace Ui {
class login_moderator;
}
// Class: login_moderator
// ---------------------
// This class represents a window for moderator authentication. It allows the moderator to
// enter their username & password in the input fields & log in.
// If the authentication is successful, the moderator_feed window is opened.
// If the authentication fails, an error message is shown.
class login_moderator : public QDialog
{
    Q_OBJECT

public:
    // The constructor for the login_moderator class.
    explicit login_moderator(moderation_manager& manager, QMainWindow* mainWindow, QWidget *parent = nullptr);

    // Destructor
    ~login_moderator();

private slots:
    // This method is triggered when the cancel button is clicked, closing the login window.
    void on_cancelButton_clicked();

    // This method is triggered when the submit button is clicked. It attempts to authenticate the entered
    // username and password. Success - moderator feed show up.
    // If the authentication fails, an error message is shown.
    void on_submitButton_clicked();

private:
    Ui::login_moderator *ui;
    moderation_manager& myManager;
    QMainWindow* mainWindow;
};

#endif // LOGIN_MODERATOR_H
