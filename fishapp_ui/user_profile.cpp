#include "user_profile.h"
#include "ui_user_profile.h"
#include "main_feed.h"
#include "mainwindow.h"
#include "user_settings.h"



user_profile::user_profile(QMainWindow* mainWindow, QWidget *parent,  auth_manager& userAuthenticaiton) :
    QDialog(parent),
    ui(new Ui::user_profile),
    currentUser(userAuthenticaiton.getCurrentUser()),
    myManager(userAuthenticaiton),
    myUpload(myManager),
    mainWindow(mainWindow)
{
    ui->setupUi(this);

    // Displaying user data on the UI
    ui->user_username->setText("@" + currentUser->getUsername());

    ui->user_fullname->setText(currentUser->getFirstName() + " " + currentUser->getLastName());

    std::vector<std::string> interests = currentUser->getInterests();

    std::string all_interests = "My Interests: ";

    for(int i = 0; i < interests.size(); i++){
        all_interests = all_interests + interests.at(i) + "; ";
    }

    ui->user_interests->setText(QString::fromStdString(all_interests));

    ui->user_interests->setWordWrap(true);

    // Retrieving all posts from the user
    myPosts = myUpload.findPosts();
    if(myPosts.size()<1){
        position = -1;
    }
    else{
        position = 0;
        this->load_post();
    }
    ui->post_content->setWordWrap(true);
}

user_profile::~user_profile()
{
    delete ui;
}

void user_profile::on_feed_clicked()
{
    // Going back to feed
    main_feed* userFeed = new main_feed(mainWindow, myManager);
    userFeed->show();
    this->close();
}


void user_profile::on_messages_clicked()
{
    //takes you to messages
}


void user_profile::on_profile_clicked()
{
    // Reloading user profile
    user_profile* userProfile = new user_profile(mainWindow, this, myManager);
    userProfile->show();
    this->close();
}


void user_profile::on_settings_clicked()
{
    // Going to settigns
    user_settings* userSettings = new user_settings(mainWindow, this, myManager);
    userSettings->show();
    this->close();
}


void user_profile::on_log_out_clicked()
{
    // Logging out to main window
    myManager.logout();
    this->close();
    mainWindow->show();
}


void user_profile::on_previous_post_clicked()
{
    // Changing position to previous post
    if(position > 0){
        position = position - 1;
        this->load_post();
    }
}


void user_profile::on_next_post_clicked()
{
    // Changing position to next post
    if(position < myPosts.size() -1){
        position = position + 1;
        this->load_post();
    }    
}


void user_profile::on_delete_post_clicked()
{
    // If current postition is valid, remove the post
    if(position > -1){
        myUpload.delete_post(myPosts.at(position).return_postId());
        myPosts = myUpload.findPosts();

        // Change position accordingly
        if(myPosts.size() == 0){
            position = -1;
        }
        else{
            position = 0;
        }
        this->load_post();
    }
}

void user_profile::load_post(){

    // Check if position is valid
    if(position!=-1){
        // Retrieving post information
        std::vector<std::string> info = myUpload.findInformation(myPosts.at(position).return_postId());
        std::string hashtags;
        for(int i = 3; i < info.size(); i++){
            hashtags = hashtags + info.at(i) + " ";
        }
        ui->post_fullname->setText(QString::fromStdString(info.at(1) + " " + info.at(2)));
        ui->post_content->setText(myPosts.at(position).return_content());
        ui->post_hashtags->setText(QString::fromStdString(hashtags));
        ui->post_username->setText(QString::fromStdString(info.at(0)));
        ui->post_timestamp->setText(myPosts.at(position).return_timestamp());
        ui->post_title->setText(myPosts.at(position).return_title());
    }
}

