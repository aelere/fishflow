#ifndef MESSAGING_H
#define MESSAGING_H

#include <QDialog>
#include <QLineEdit>
#include "../messaging/one_message.h"
#include "ui_messaging.h" // Generated from the XML file using the Qt Designer

class messaging : public QDialog, private Ui::messaging {
    Q_OBJECT

public:
    explicit messaging(QWidget *parent = nullptr);
    ~messaging();

private slots:
    void on_sendButton_clicked();
    void on_cancelButton_clicked();

private:
    QLineEdit *input;
    std::vector<one_message> messages;
};

#endif // MESSAGING_UI_H






