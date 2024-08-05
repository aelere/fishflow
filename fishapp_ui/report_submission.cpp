#include "report_submission.h"
#include "ui_report_submission.h"

report_submission::report_submission(user* user, int postId, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::report_submission),
    m_user(user),
    m_postId(postId)
{
    ui->setupUi(this);
}

report_submission::~report_submission()
{
    delete ui;
}


void report_submission::on_submitButtom_clicked()
{
    // Fetch text from ui entry
    QString reason = ui->textEdit->toPlainText();

    // Get current date and time
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString timestamp = currentDateTime.toString("yyyy-MM-dd-hh-mm-ss");

    if (m_user->reportPost(m_postId, reason, timestamp)) {
        // If report successful, make this popup
        QMessageBox::information(this, "Report submitted", "Your report has been submitted successfully.");
    } else {
        // Else make this popup
        QMessageBox::critical(this, "Report submission failed", "An error occurred while submitting your report. Please try again later.");
    }

    this->close();

}

void report_submission::on_cancelButton_clicked()
{
    this->close();
}

