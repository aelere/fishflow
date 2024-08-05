#include "review_reports.h"
#include "ui_review_reports.h"

review_reports::review_reports(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::review_reports)
{
    ui->setupUi(this);
}

review_reports::~review_reports()
{
    delete ui;
}
