#include "messaging.h"

messaging::messaging(QWidget *parent) : QDialog(parent) {
    setupUi(this);
    input = findChild<QLineEdit *>("lineEdit");
}

messaging::~messaging() {
}

void messaging::on_sendButton_clicked() {
    std::string content = input->text().toStdString();
    // You can modify the parameters below to fit your requirements
    one_message new_message(1, 1, 1, content, "timestamp");
    messages.push_back(new_message);
    input->clear();
}

void messaging::on_cancelButton_clicked() {
    input->clear();
}
