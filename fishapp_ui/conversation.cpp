#include "conversation.h"
#include "ui_conversations.h"

conversation::conversation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::conversations) {
        ui->setupUi(this);
    }

conversation::~conversation() {
    delete ui;
}
