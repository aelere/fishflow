#include "user_settings.h"
#include "change_profile.h"
#include "ui_user_settings.h"
#include "user_profile.h"

user_settings::user_settings(QMainWindow* mainWindow, QWidget *parent,  auth_manager& userAuthenticaiton) :
    QMainWindow(parent),
    ui(new Ui::user_settings),
    currentUser(userAuthenticaiton.getCurrentUser()),
    myManager(userAuthenticaiton),
    mainWindow(mainWindow)
{
    ui->setupUi(this);
}

user_settings::~user_settings()
{
    delete ui;
}

void user_settings::on_profile_clicked()
{
    //  Taking user back to user profile
    user_profile* userProfile = new user_profile(mainWindow, this, myManager);
    userProfile->show();
    this->close();
}

void user_settings::on_info_clicked()
{
    // Taking user to change profile screen
    change_profile* changeProfile = new change_profile(mainWindow, this, myManager);
    changeProfile->show();
    this->close();
}


void user_settings::on_my_feed_clicked()
{
    //toggles how feed is displayed
}


void user_settings::on_anonymous_clicked()
{
    //goes to anonymous mode
}


void user_settings::on_feed_status_clicked()
{
    //turns on/off feed
}


void user_settings::on_messaging_clicked()
{
    //turns on/off messaging
}


void user_settings::on_new_messaging_clicked()
{
    //enables/disables new messages
}

