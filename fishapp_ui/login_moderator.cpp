#include "login_moderator.h"
#include "moderator_feed.h"
#include "ui_login_moderator.h"

login_moderator::login_moderator(moderation_manager& manager, QMainWindow* mainWindow, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login_moderator),
    myManager(manager),
    mainWindow(mainWindow)
{
    ui->setupUi(this);
    ui->passwordEdit->setEchoMode(QLineEdit::Password);
}

login_moderator::~login_moderator()
{
    delete ui;
}

void login_moderator::on_cancelButton_clicked()
{
    this->close();
}

void login_moderator::on_submitButton_clicked()
{
    const QString username = ui->usernameEdit->text();
    const QString password = ui->passwordEdit->text();

    // If login works, authenticate and close. Else, critical box.
    if (myManager.authenticateModerator(username, password)) {
        moderator_feed* moderatorFeed = new moderator_feed(mainWindow, myManager);
        moderatorFeed->show();
        this->close();
        mainWindow->hide(); // Hide mainwindow
    } else {
        QMessageBox::critical(this, "Error", "Username or password incorrect.");
    }
}

