#include "hashtag_feed.h"
#include "ui_hashtag_feed.h"
#include "main_feed.h"
#include "report_submission.h"
#include "user_profile.h"
#include "visiting_user_profile.h"

hashtag_feed::hashtag_feed(QMainWindow* mainWindow, QWidget *parent,  auth_manager& userAuthenticaiton, std::vector<one_post> posts) :
    QMainWindow(parent),
    ui(new Ui::hashtag_feed),
    currentUser(userAuthenticaiton.getCurrentUser()),
    myFeed(userAuthenticaiton),
    myManager(userAuthenticaiton),
    mainWindow(mainWindow)
{
    ui->setupUi(this);

    // Setting style sheet for the post_content QLabel
    ui->post_content->setStyleSheet("QLabel {"
                                    "border: 1px solid grey;"
                                    "}");

    // Setting the username and full name labels with the current user's information
    ui->username->setText("@" + currentUser->getUsername());
    ui->fullName->setText(currentUser->getFirstName() + " " + currentUser->getLastName());

    // Initializing the myPosts vector with the provided posts
    myPosts = posts;
    if(myPosts.size()<1){
        position = -1;  // No posts available, setting position to -1
    }
    else{
        position = 0;   // Setting position to the first post
        this->load_post();
    }
    // Enabling word wrapping for the post_content QLabel
    ui->post_content->setWordWrap(true);
}

hashtag_feed::~hashtag_feed()
{
    delete ui;
}


// Takes you to top of feed
void hashtag_feed::on_feed_clicked()
{
//    main_feed* userFeed = new main_feed(mainWindow, myManager);
//    userFeed->show();
    this->close();
}


void hashtag_feed::load_post(){
    // Loading the current post to the UI
    if(position!=-1){
        std::vector<std::string> info = myFeed.findInformation(myPosts.at(position).return_postId());
        std::string hashtags;
        for(int i = 3; i < info.size(); i++){
            hashtags += info.at(i) + " ";
        }
        ui->post_fullName_3->setText(QString::fromStdString(info.at(1) + " " + info.at(2)));
        ui->post_content->setText(myPosts.at(position).return_content());
        ui->post_hashtag_3->setText(QString::fromStdString(hashtags));
        ui->post_username_3->setText("@" + QString::fromStdString(info.at(0)));
        ui->post_timestamp->setText(myPosts.at(position).return_timestamp());
        ui->post_title->setText(myPosts.at(position).return_title());
    }
}

void hashtag_feed::on_log_out_clicked()
{
    this->close();          // Close the current hashtag_feed window
    mainWindow->show();     // Show the main window
}


void hashtag_feed::on_previous_post_clicked()
{
    if(position > 0){
        position = position - 1;    // Decrease the position by 1
        this->load_post();          // Load
    }
}

void hashtag_feed::on_next_post_clicked()
{
    if(position < myPosts.size() - 1){
        position = position + 1; //Increase the position by 1
        this->load_post();  // Load
    }
}


void hashtag_feed::on_report_clicked()
{
    report_submission *reportDialog = new report_submission(currentUser, myPosts.at(position).return_postId(), this);
    reportDialog->show();

    // Connect the finished signal of the report submission dialog to the deleteLater slot
    connect(reportDialog, &report_submission::finished, reportDialog, &report_submission::deleteLater);
}


void hashtag_feed::on_go_clicked()
{
//    int id = myPosts.at(position).return_authorId();

//    if(id == currentUser->getId()){
//        user_profile* userProfile = new user_profile(mainWindow, this, myManager);
//        userProfile->show();
//        this->hide();
//    }
//    else{
//        visiting_user_profile* visitingUserProfile = new visiting_user_profile(mainWindow, this, myManager, id);
//        visitingUserProfile->show();
//        this->hide();
//    }
}

