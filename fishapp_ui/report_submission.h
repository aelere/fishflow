#ifndef REPORT_SUBMISSION_H
#define REPORT_SUBMISSION_H

#include <QDialog>
#include <QMessageBox>
#include "../authentication_engine/user.h"

namespace Ui {
class report_submission;
}

// Class: report_submission
// ---------------------
// The report_submission class provides a submission form to users
// It enables users to write the reason why they think the post should
// be removed, which gets sent to the moderator_feed for mods to review.

class report_submission : public QDialog
{
    Q_OBJECT

public:
    // Constructor
    explicit report_submission(user* user, int postId, QWidget *parent = nullptr); // Update the constructor

    // Destructor
    ~report_submission();

private slots: // Slots
    void on_submitButtom_clicked();

    void on_cancelButton_clicked();

private:
    Ui::report_submission *ui;
    user* m_user;
    int m_postId;
};

#endif // REPORT_SUBMISSION_H
