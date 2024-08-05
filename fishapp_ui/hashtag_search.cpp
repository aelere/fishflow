#include "hashtag_search.h"
#include "hashtag_feed.h"
#include "ui_hashtag_search.h"
#include <QDialog>
#include <QMessageBox>


hashtag_search::hashtag_search(QMainWindow* mainWindow, QWidget *parent, auth_manager& userAuthenticaiton) :
    QMainWindow(parent),
    ui(new Ui::hashtag_search),
    myManager(userAuthenticaiton),
    myFeed(userAuthenticaiton),
    mainWindow(mainWindow)
{
    ui->setupUi(this);
}

hashtag_search::~hashtag_search()
{
    delete ui;
}

void hashtag_search::on_submitButton_clicked()
{

    // Getting the text entered in the input field
    const QString in = ui->input->text();

    // Checking if the input is not null
    if(in != nullptr){
        std::vector<one_post> temp = myFeed.post_by_specific_hashtag(in); // Retrieving posts matching the specified hashtag

        // Checking if there are matching posts
        if(temp.size() != 0){

            // Creating a new hashtag_feed window with the matching posts
            hashtag_feed* hashtagFeed = new hashtag_feed(mainWindow, this, myManager, temp);

            // Showing the hashtag_feed window
            hashtagFeed->show();

            this->close(); // Closing the current hashtag_search window
        }
        else{
            QMessageBox::information(this, in, "No post with given hashtag."); // Displaying a message box indicating no matching posts were found
            this->close(); // Closing the current hashtag_search window
        }
    }
    else{
        QMessageBox::information(this, in, "No post with given hashtag."); // Displaying a message box indicating no matching posts were found
        this->close(); // Closing the current hashtag_search window
    }
}


void hashtag_search::on_cancelButton_clicked()
{
    this->close();
}

