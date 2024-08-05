#include "moderator_feed.h"
#include "ui_moderator_feed.h"

// In moderator_feed.cpp, in the moderator_feed constructor
moderator_feed::moderator_feed(QMainWindow* mainWindow, moderation_manager &modAuthenticaiton, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::moderator_feed),
    currentModerator(modAuthenticaiton.getCurrentModerator()),
    myModManager(modAuthenticaiton),
    mainWindow(mainWindow)
{
    ui->setupUi(this);

    // Load all reports
    allReports = myModManager.all_reports();
    if (allReports.size() < 1) {
        position = -1;
    } else {
        position = 0;
    }
    this->load_report();

    // Display admin info
    ui->modFullLabel->setText(currentModerator->getFirstName() + " " + currentModerator->getLastName());
    ui->modIDLabel->setText("Admin #" + QString::number(currentModerator->getModeratorId()));
    ui->modUsernameLabel->setText("@" + currentModerator->getUsername());
}


moderator_feed::~moderator_feed()
{
    delete ui;
}

void moderator_feed::load_report() {

    // Checking if position is valid.
    if (position >= 0 && position < allReports.size()) {
        // Fetching report data
        std::vector<std::string> reportData = allReports.at(position);

        // Updating UI Labels
        ui->reportIDLabel->setText("Report ID : " + QString::fromStdString(reportData[0]));
        ui->reporterIDLabel->setText("Reporter #" + QString::fromStdString(reportData[1]));
        int postID = stoi(reportData[2]);
        ui->postIDLabel->setText(QString::fromStdString(reportData[2]));
        ui->postLabel->setText(myModManager.getPostContent(postID));
        ui->reasonLabel->setText("Reason: " + QString::fromStdString(reportData[3]));
        ui->timeLabel->setText(QString::fromStdString(reportData[4]));
        ui->userIDLabel->setText(QString::fromStdString(reportData[5]));
        ui->post_title->setText(QString::fromStdString(reportData[6]));
    } else {
        // Handling cases when there are no reports or position is out of bounds
        QMessageBox::information(this, "No Reports", "There are no reports to review.");
    }
}

void moderator_feed::on_previousButton_clicked()
{
    // Going to previous report
    if (position > 0) {
        position = position - 1;
        this->load_report();
    }
}


void moderator_feed::on_nextButton_clicked()
{
    // Going to next report
    if (position < allReports.size() - 1) {
        position = position + 1;
        this->load_report();
    }
}


void moderator_feed::on_pushButton_8_clicked()
{

    // Log out button
    myModManager.logout();
    this->close();
    mainWindow->show();
}


void moderator_feed::on_banUserButton_clicked()
{

    // If there are no reprots, display error box
    if (allReports.size() == 0){
        QMessageBox::warning(this, "Ban User", "No more reports");
        return;
    }

    // Retrieve user id and ban reason.
    int user_id = ui->userIDLabel->text().toInt();
    QString ban_reason = ui->decisionEdit->toPlainText();


    // Remove the user.
    if (myModManager.getCurrentModerator()->removeUser(user_id, ban_reason)) {
        QMessageBox::information(this, "Ban User", "The user has been banned successfully.");
        updateFeed();
    } else {
        QMessageBox::warning(this, "Ban User", "Failed to ban the user.");
        return;
    }

    allReports = myModManager.all_reports();

    // Check if there are more reports available
    if (position < allReports.size() - 1) {
        // Load next report
        position++;
        load_report();
    } else {
        // No more reports available, display a message or close the moderator_feed window
        QMessageBox::information(this, "No More Reports", "There are no more reports to review.");

        position = -1;

        // this->close();
    }
}


void moderator_feed::on_removePostButton_clicked()
{
    // Check if there are no reprts to remove
    if (allReports.size() == 0){
        QMessageBox::warning(this, "Remove Post", "No more reports");
        return;
    }

    if (myModManager.getCurrentModerator()->removePost(ui->postIDLabel->text().toInt(), ui->decisionEdit->toPlainText())) {
        QMessageBox::information(this, "Remove Post", "The post has been removed successfully.");
        updateFeed(); // Update the allReports vector with the latest data
    } else {
        QMessageBox::warning(this, "Remove Post", "Failed to remove the post.");
        return;
    }

    // Check if there are more reports available
    if (position < allReports.size() - 1) {
        // Increment position to load next report
        position++;
        // Load next report
        load_report();
    } else {
        // No more reports available, display a message or close the moderator_feed window
        QMessageBox::information(this, "No More Reports", "There are no more reports to review.");
        position = -1;
        // this->close();
    }
}

void moderator_feed::updateFeed() {
    allReports = myModManager.all_reports();

    if (allReports.size() < 1) {
        position = -1;
    } else if (position >= allReports.size()) {
        // Adjust position to the last report if it goes out of bounds
        position = allReports.size() - 1;
    }

    if (position == -1) {
        ui->reportIDLabel->setText("Report ID : ");
        ui->reporterIDLabel->setText("Reporter #");
        ui->postLabel->setText("NO REPORTS");
        ui->reasonLabel->setText("");
        ui->timeLabel->setText("");
        ui->userIDLabel->setText("User #");
        ui->post_title->setText("NO REPORTS");
    } else {
        this->load_report();
    }
}

void moderator_feed::on_shadowbanButton_clicked()
{
    if (allReports.size() == 0){
        QMessageBox::warning(this, "Shadowban User", "No more reports");
        return;
    }

    int user_id = ui->userIDLabel->text().toInt();

    if (myModManager.getCurrentModerator()->shadowBanUser(user_id)) {
        QMessageBox::information(this, "Success", "User successfully shadowbanned.");

        int report_id = ui->reportIDLabel->text().split(" ")[3].toInt();
        myModManager.getCurrentModerator()->removeReport(report_id);

        // Update the reports and UI
        updateFeed();
    } else {
        QMessageBox::warning(this, "Error", "Failed to shadowban user.");
    }


    // Check if there are more reports available
    if (position < allReports.size() - 1) {
        // Increment position to load next report
        position++;
        // Load next report
        load_report();
    } else {
        // No more reports available, display a message or close the moderator_feed window
        QMessageBox::information(this, "No More Reports", "There are no more reports to review.");
        position = -1;
        // this->close();
    }
}




