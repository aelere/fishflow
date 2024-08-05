#include "visit_user.h"
#include "ui_visit_user.h"

visit_user::visit_user(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::visit_user)
{
    ui->setupUi(this);
}

visit_user::~visit_user()
{
    delete ui;
}
