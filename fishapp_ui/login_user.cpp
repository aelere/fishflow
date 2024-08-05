#include "login_user.h"
#include "main_feed.h"
#include "ui_login_user.h"

login_user::login_user(auth_manager& manager, QMainWindow* mainWindow, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login_user),
    myManager(manager),
    mainWindow(mainWindow) // Store the mainWindow pointer
{
    ui->setupUi(this);
    ui->passwordEdit->setEchoMode(QLineEdit::Password);
}

login_user::~login_user()
{
    delete ui;
}

void login_user::on_cancelButton_clicked()
{
    this->close();
}

void login_user::on_submitButton_clicked()
{
    // Getting user input from UI
    const QString username = ui->usernameEdit->text();
    const QString password = ui->passwordEdit->text();

if (myManager.authenticate(username, password)){
        main_feed* userFeed = new main_feed(mainWindow, myManager); // Pass mainWindow to main_feed
        userFeed->show(); // Show main_feed
        this->close(); // Hide login_user
        mainWindow->hide(); // Hide mainwindow

    }else{
        QMessageBox::critical(this, "Error", "Username or password incorrect.");
    }
}

