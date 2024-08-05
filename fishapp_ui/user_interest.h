#ifndef USER_INTEREST_H
#define USER_INTEREST_H

#include <QDialog>

namespace Ui {
class user_interest;
}

class user_interest : public QDialog
{
    Q_OBJECT

public:
    explicit user_interest(QWidget *parent = nullptr);
    ~user_interest();

private:
    Ui::user_interest *ui;
};

#endif // USER_INTEREST_H
