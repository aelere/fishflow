#include "user_interest.h"
#include "ui_user_interest.h"

user_interest::user_interest(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::user_interest)
{
    ui->setupUi(this);
}

user_interest::~user_interest()
{
    delete ui;
}
