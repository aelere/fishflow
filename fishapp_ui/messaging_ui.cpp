//#include "messaging_ui.h"

//MessagingUI::MessagingUI(QWidget *parent) :
//    QWidget(parent),
//    messageDisplay(new QTextEdit(this)),
//    messageInput(new QLineEdit(this)),
//    sendButton(new QPushButton(tr("Send"), this)),
//    mainLayout(new QVBoxLayout(this)),
//    inputLayout(new QHBoxLayout()){
//    messageDisplay->setReadOnly(true);

//    inputLayout->addWidget(messageInput);
//    inputLayout->addWidget(sendButton);

//    mainLayout->addWidget(messageDisplay);
//    mainLayout->addLayout(inputLayout);

//    setLayout(mainLayout);

//    connect(sendButton, &QPushButton::clicked, this, &MessagingUI::onSendButtonClicked);
//    connect(messageInput, &QLineEdit::returnPressed, this, &MessagingUI::onSendButtonClicked);
//}

//MessagingUI::~MessagingUI(){
//}

//void MessagingUI::onSendButtonClicked(){
//    QString message = messageInput->text().trimmed();
//    if (!message.isEmpty()) {
//        messageDisplay->append(message);
//        messageInput->clear();
//    }
//    messageInput->setFocus();
//}

