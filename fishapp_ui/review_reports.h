#ifndef REVIEW_REPORTS_H
#define REVIEW_REPORTS_H

#include <QDialog>

namespace Ui {
class review_reports;
}

class review_reports : public QDialog
{
    Q_OBJECT

public:
    explicit review_reports(QWidget *parent = nullptr);
    ~review_reports();

private:
    Ui::review_reports *ui;
};

#endif // REVIEW_REPORTS_H
