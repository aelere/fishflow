#ifndef VISIT_USER_H
#define VISIT_USER_H

#include <QDialog>

namespace Ui {
class visit_user;
}

class visit_user : public QDialog
{
    Q_OBJECT

public:
    explicit visit_user(QWidget *parent = nullptr);
    ~visit_user();

private:
    Ui::visit_user *ui;
};

#endif // VISIT_USER_H
