#include "visiting_user_profile.h"
#include "main_feed.h"
#include "report_submission.h"
#include "ui_visiting_user_profile.h"
#include "user_profile.h"

visiting_user_profile::visiting_user_profile(QMainWindow* mainWindow, QWidget *parent,  auth_manager& userAuthenticaiton, int other_user_id) :
    QMainWindow(parent),
    ui(new Ui::visiting_user_profile),
    currentUser(userAuthenticaiton.getCurrentUser()),
    myManager(userAuthenticaiton),
    myUpload(myManager),
    mainWindow(mainWindow),
    otherUser(myUpload.getOtherUser(other_user_id))
{
    ui->setupUi(this);

    // Fetching and displaying user info on UI
    ui->user_username->setText("@" + otherUser->getUsername());

    ui->user_fullname->setText(otherUser->getFirstName() + " " + otherUser->getLastName());

    std::vector<std::string> interests = otherUser->getInterests();

    // Displaying and iterating through all interests
    std::string all_interests = "My Interests: ";

    for(int i = 0; i < interests.size(); i++){
        all_interests = all_interests + interests.at(i) + " ";
    }

    ui->user_interests->setText(QString::fromStdString(all_interests));

    // Fetching posts from the current user.
    otherPosts = myUpload.other_findPosts(other_user_id);

    // Displaying fosts
    if(otherPosts.size()<1){
        position = -1;
    }
    else{
        position = 0;
        this->load_post();
    }
    ui->post_content->setWordWrap(true);
}

visiting_user_profile::~visiting_user_profile()
{
    delete ui;
}

void visiting_user_profile::on_report_post_clicked()
{
    // Taking user to report screen
    report_submission *reportDialog = new report_submission(currentUser, otherPosts.at(position).return_postId(), this);
    reportDialog->show();
    connect(reportDialog, &report_submission::finished, reportDialog, &report_submission::deleteLater);
}


void visiting_user_profile::on_previous_post_clicked()
{
    // Taking user to previous post if applicable
    if(position > 0){
        position = position - 1;
        this->load_post();
    }
}


void visiting_user_profile::on_next_post_clicked()
{
    // Taking user to next post if applicable
    if(position < otherPosts.size() -1){
        position = position + 1;
    }
    this->load_post();
}


void visiting_user_profile::on_log_out_clicked()
{
    // Logging out
    myManager.logout();
    this->close();
    mainWindow->show();
}


void visiting_user_profile::on_conversation_clicked()
{
    //creates conversation if doesn't exit
    //takes you to messages
}


void visiting_user_profile::on_messages_clicked()
{
    //takes you to messages
}


void visiting_user_profile::on_feed_clicked()
{
    main_feed* userFeed = new main_feed(mainWindow, myManager);
    userFeed->show();
    this->close();
}


void visiting_user_profile::on_profile_clicked()
{
    user_profile* userProfile = new user_profile(mainWindow, this, myManager);
        userProfile->show();
        this->close();
}

void visiting_user_profile::load_post(){
    if(position!=-1){
            // Fetching post data
            std::vector<std::string> info = myUpload.findInformation(otherPosts.at(position).return_postId());
            std::string hashtags;

            // Displaying all the hashtags for posts
            for(int i = 3; i < info.size(); i++){
                hashtags = hashtags + info.at(i) + " ";
            }

            // Displaying all hashtags
            ui->post_fullname->setText(QString::fromStdString(info.at(1) + " " + info.at(2)));
            ui->post_content->setText(otherPosts.at(position).return_content());
            ui->post_hashtags->setText(QString::fromStdString(hashtags));
            ui->post_username->setText(QString::fromStdString(info.at(0)));
            ui->post_timestamp->setText(otherPosts.at(position).return_timestamp());
            ui->post_title->setText(otherPosts.at(position).return_title());
        }
}
