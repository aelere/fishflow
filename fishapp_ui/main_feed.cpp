#include "main_feed.h"
#include "create_post.h"
#include "hashtag_search.h"
#include "report_submission.h"
#include "ui_main_feed.h"
#include "user_profile.h"
#include "visiting_user_profile.h"
#include "hashtag_feed.h"
#include "hashtag_search.h"


main_feed::main_feed(QMainWindow* mainWindow, auth_manager& userAuthenticaiton, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::main_feed),
    currentUser(userAuthenticaiton.getCurrentUser()),
    myFeed(userAuthenticaiton),
    myManager(userAuthenticaiton),
    mainWindow(mainWindow),
    anonymous(false)
{
    ui->setupUi(this);


    // Setting style sheet
    ui->post_content->setStyleSheet("QLabel {"
                                    "border: 1px solid grey;"
                                    "}");


    // Setting up user info
    ui->username->setText("@" + currentUser->getUsername());
    ui->fullName->setText(currentUser->getFirstName() + " " + currentUser->getLastName());
    myPosts = myFeed.posts_by_hashtag();

    // Sorting posts, ensuring posts exist.
    if(myPosts.size()<1){
        position = -1;
    }
    else{
        position = 0;
        this->load_post();
    }
    ui->post_content->setWordWrap(true);
}

 // The anon constructor.
main_feed::main_feed(bool anonymous, QMainWindow* mainWindow, auth_manager& userAuthenticaiton, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::main_feed),
    currentUser(anonymous ? nullptr : userAuthenticaiton.getCurrentUser()), // Set currentUser to nullptr if anonymous
    myFeed(userAuthenticaiton),
    myManager(userAuthenticaiton),
    mainWindow(mainWindow),
    anonymous(anonymous) // Store the anonymous flag
{
    ui->setupUi(this);

    ui->post_content->setStyleSheet("QLabel {"
                                    "border: 1px solid grey;"
                                    "}");

    // Disabling features based on the anonymous flag
    if (anonymous) {
        ui->username->setText("Anonymous");
        ui->fullName->setText("Anonymous User");
        ui->create_post->setEnabled(false);
        ui->messages->setEnabled(false);
        ui->profile->setEnabled(false);
        ui->go_to_user->setEnabled(false);
        ui->search_hashtag->setEnabled(false);
        ui->report->setEnabled(false);
        //ui->log_out->setEnabled(false);
    } else {
        ui->username->setText("@" + currentUser->getUsername());
        ui->fullName->setText(currentUser->getFirstName() + " " + currentUser->getLastName());
    }

    myPosts = myFeed.all_posts();
    if(myPosts.size()<1){
        position = -1;
    }
    else{
        position = 0;
        this->load_post();
    }
}


main_feed::~main_feed()
{
    delete currentUser;
    delete ui;
}

//takes you to top of feed
void main_feed::on_feed_clicked()
{
    if(position != -1){
        position = 0;
        this->load_post();
    }
}


void main_feed::on_messages_clicked()
{
    //takes you to messages
}


void main_feed::on_profile_clicked()
{
    user_profile* userProfile = new user_profile(mainWindow, this, myManager);
    userProfile->show();
    this->hide();
}



void main_feed::load_post(){

    // Checking if posts exist, and loading post respectively.
    if(position!=-1){
        std::vector<std::string> info = myFeed.findInformation(myPosts.at(position).return_postId());
        std::string hashtags;
        for(int i = 3; i < info.size(); i++){
            hashtags += info.at(i) + " ";
        }

        // Loading post data
        ui->post_fullName->setText(QString::fromStdString(info.at(1) + " " + info.at(2)));
        ui->post_content->setText(myPosts.at(position).return_content());       
        ui->post_hashtag->setText(QString::fromStdString(hashtags));
        ui->post_username->setText("@" + QString::fromStdString(info.at(0)));
        ui->post_timestamp->setText(myPosts.at(position).return_timestamp());
        ui->post_title->setText(myPosts.at(position).return_title());
    }
}


void main_feed::on_log_out_clicked()
{
    // If the current user is not anonymous, log out. Else, just close the window.
    if (!anonymous){
        myManager.logout();
    }
    this->close();
    mainWindow->show();
}


void main_feed::on_create_post_clicked()
{

    // Creating a new create_post dialog passing the auth manager and this instance (main_feed) as parent.
    create_post *createPostDialog = new create_post(myManager, this);
    createPostDialog->show();

    // Connecting the postCreated signal from the create_post dialog to the updateFeed slot in this instance.
    // Triggering the feed to update.
    connect(createPostDialog, &create_post::postCreated, this, &main_feed::updateFeed);

    // Ensuring that the dialog is deleted from memory once it's closed.
    connect(createPostDialog, &create_post::finished, createPostDialog, &create_post::deleteLater);
}


void main_feed::on_search_hashtag_clicked()
{
    // Creating and displaying the new hasttag_search dialog.
    hashtag_search *hashtagSearch = new hashtag_search(mainWindow, this, myManager);
    hashtagSearch->show();
}


void main_feed::on_previous_post_clicked()
{
    // Decrease position by one
    if(position > 0){
        position = position - 1;
        this->load_post();
    }  
}


void main_feed::on_next_post_clicked()
{
    // Increase position by one.
    if(position < myPosts.size() - 1){
        position = position + 1;
        this->load_post();
    }
}


void main_feed::on_report_clicked()
{
    // Creating a new report dialog.
    report_submission *reportDialog = new report_submission(currentUser, myPosts.at(position).return_postId(), this);
    reportDialog->show();
    connect(reportDialog, &report_submission::finished, reportDialog, &report_submission::deleteLater);
}

void main_feed::updateFeed() {

    // Fetch the posts by hashtag.
    myPosts = myFeed.posts_by_hashtag();
    if (myPosts.size() < 1) {
        // Set position to -1 if there are no posts
        position = -1;
    } else {
        // Load posts if there are any.
        position = 0;
        this->load_post();
    }
}

void main_feed::on_go_to_user_clicked()
{

    // Fetching the author of the post.
    int id = myPosts.at(position).return_authorId();

    // If the author is current user, take the user to their respective page.
    if(id == currentUser->getId()){
        user_profile* userProfile = new user_profile(mainWindow, this, myManager);
        userProfile->show();
        this->hide();
    }
    else{
        // If not, take the user to the visiting profile page.
        visiting_user_profile* visitingUserProfile = new visiting_user_profile(mainWindow, this, myManager, id);
        visitingUserProfile->show();
        this->hide();
    }
}

