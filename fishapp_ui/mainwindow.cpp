#include "mainwindow.h"
#include "login_moderator.h"
#include "login_user.h"
#include "ui_mainwindow.h"
#include "main_feed.h"
#include "createuser.h"

MainWindow::MainWindow(QWidget *parent, db_access& dbAccess, auth_manager& manager, moderation_manager& modManager)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_dbAccess(dbAccess)
    , myManager(manager)
    , myModManager(modManager)
{
    ui->setupUi(this);

    // Initialize the stacked widget
    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(ui->centralwidget);
    setCentralWidget(stackedWidget);
}

MainWindow::MainWindow(db_access& dbAccess, auth_manager& myManager, moderation_manager& modManager)
    : ui(new Ui::MainWindow)
    , m_dbAccess(dbAccess)
    , myManager(myManager)
    , myModManager(modManager)
{
    ui->setupUi(this);

    // Initialize the stacked widget
    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(ui->centralwidget);
    setCentralWidget(stackedWidget);
}

void MainWindow::displayDialog(QWidget *dialog) {
    int index = stackedWidget->addWidget(dialog);
    stackedWidget->setCurrentIndex(index);
    resize(dialog->sizeHint());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_createAccount_clicked()
{
    // Go to createAccount
    createUser *createUserDialog = new createUser(myManager, this);
    displayDialog(createUserDialog);
    connect(createUserDialog, &createUser::finished, createUserDialog, &createUser::deleteLater);

}

void MainWindow::on_loginUserButton_clicked()
{
    //Go to loginUser
    login_user *loginUserDialog = new login_user(myManager, this);
    displayDialog(loginUserDialog);
    connect(loginUserDialog, &login_user::finished, loginUserDialog, &login_user::deleteLater);

}

void MainWindow::on_loginModeratorButton_clicked()
{
    // Go to modLogin
    login_moderator *loginModDialog = new login_moderator(myModManager, this);
    displayDialog(loginModDialog);
    connect(loginModDialog, &login_moderator::finished, loginModDialog, &login_moderator::deleteLater);
}

void MainWindow::on_anonUserButton_clicked()
{
    main_feed* userFeed = new main_feed(true, this, myManager); // Pass mainWindow to main_feed
    userFeed->show(); // Show main_feed
    this->hide();
}

