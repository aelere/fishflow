#ifndef CONVERSATION_H
#define CONVERSATION_H

#include <QDialog>

namespace Ui {
class conversation;
}

class conversation : public QDialog
{
    Q_OBJECT

public:
    explicit conversation(QWidget *parent = nullptr);
    ~conversation();

private:
    Ui::conversation *ui;
};

#endif // CONVERSATION_H
