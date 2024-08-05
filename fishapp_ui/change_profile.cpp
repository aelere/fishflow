#include "change_profile.h"
#include "QtGui/qstandarditemmodel.h"
#include "QtWidgets/qlistview.h"
#include "ui_change_profile.h"
#include "../authentication_engine/auth_manager.h"
#include "user_settings.h"
#include <QMessageBox>

change_profile::change_profile(QMainWindow* mainWindow, QWidget *parent,  auth_manager& manager) :
    QMainWindow(parent),
    ui(new Ui::change_profile),
    currentUser(manager.getCurrentUser()),
    mainWindow(mainWindow),
    myManager(manager)
{
    ui->setupUi(this);

    // Email expression.
    emailRegex = *new QRegularExpression("^[\\w-]+(\\.[\\w-]+)*@[\\w-]+(\\.[\\w-]+)*(\\.[a-zA-Z]{2,})$");


    // Initializing item model for interest box.
    QStandardItemModel *model = new QStandardItemModel(this);

    // Pushing interests into the box.
    foreach (QString interest, interests) {
        QStandardItem *item = new QStandardItem(interest);
        item->setCheckable(true);
        item->setCheckState(Qt::Unchecked);
        model->appendRow(item);
    }

    // Setting the model for the list view
    if (model != nullptr) {
        ui->interestsBox->setEditable(true);
        ui->interestsBox->setView(new QListView());
        ui->interestsBox->setModel(model);
    } else {
        qDebug() << "Model pointer is null!";
    }
}

change_profile::~change_profile()
{
    delete ui;
}

void change_profile::on_cancelButton_clicked()
{
    // Creating and showing new user_settings window.
    user_settings* userSettings = new user_settings(mainWindow, this, myManager);
    userSettings->show();
    this->close();
}


void change_profile::on_submitButton_clicked()
{
    // Pulling the entered data from the UI
    const QString username = ui->userNameEdit->text();
    const QString first = ui->firstNameEdit->text();
    const QString last = ui->lastNameEdit->text();
    const QString email = ui->emailEdit->text();
    const QString password = ui->passwordEdit->text();

    // Checking if email is valid.
    if (!isLegitEmail(email) && email != nullptr) {
        QMessageBox::critical(this, "Error", "Invalid email format.");
        return;
    }

    // Creating the user
    if (myManager.changeUserInformation(username, first, last, email, currentUser->getId())) {
        QStandardItemModel *model = qobject_cast<QStandardItemModel*>(ui->interestsBox->model());

        // Checking which items are checked
        QStringList selectedInterests;
        bool interestsSelected = false; // Flag to track if any interest is selected
        for (int i = 0; i < model->rowCount(); i++) {
            QStandardItem *item = model->item(i);
            if (item->checkState() == Qt::Checked) {
                QString interestName = item->text();
                selectedInterests.append(interestName);
                interestsSelected = true;
            }
        }

        // Getting the user ID
        int userId = myManager.getUserId(username);

        // Clearing existing interests for the current user only if interests are selected
        if (interestsSelected) {
            myManager.clearIntersts(currentUser->getId());
        }

        // Adding selected interests to the user's profile
        foreach(QString interest, selectedInterests){
            if (!myManager.addUserInterest(userId, interest)){
                QMessageBox::critical(this, "Error", "Unable to add interest.");
                return;
            }
        }

        // Changing the password if provided
        if(password != nullptr){
            if( username == nullptr ){
                myManager.changePassword(currentUser->getUsername(), password);
            }
            else{
                myManager.changePassword(username, password);
            }
        }

        // Creating a new user_settings window and showing it
        user_settings* userSettings = new user_settings(mainWindow, this, myManager);
        userSettings->show();
        this->close();
    } else {
        QMessageBox::critical(this, "Error", "Unable to create account.");
    }
}


bool change_profile::isLegitEmail(const QString &email){
    // If input matches the format, returns true. Else false.
    if (emailRegex.match(email).hasMatch()){
        return true;
    } else {
        return false;
    }
}


void change_profile::on_interestsBox_activated(int index)
{
    // Retrieving selected item model index
    QModelIndex modelIndex = ui->interestsBox->model()->index(index, 0);

    // Casting the object to change checked state.
    QStandardItemModel *model = qobject_cast<QStandardItemModel*>(ui->interestsBox->model());

    // Retrieving the selected object.
    QStandardItem *item = model->itemFromIndex(modelIndex);

    // Geting checkstate
    Qt::CheckState state = item->checkState();

    // Changing the change to its opposite/
    item->setCheckState(state == Qt::Checked ? Qt::Unchecked : Qt::Checked);
}

