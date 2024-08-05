#include "createuser.h"
#include "ui_createuser.h"


createUser::createUser(auth_manager& manager, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::createUser),
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

createUser::~createUser()
{
    delete ui;
}

void createUser::on_submitButton_clicked()
{


    // Pulling the entered data from the UI
    const QString username = ui->userNameEdit->text();
    const QString first = ui->firstNameEdit->text();
    const QString last = ui->lastNameEdit->text();
    const QString email = ui->emailEdit->text();
    const QString password = ui->passwordEdit->text();

    // Check if any input fields are empty
    if (username.isEmpty() || first.isEmpty() || last.isEmpty() || email.isEmpty() || password.isEmpty()) {
        QMessageBox::critical(this, "Error", "All fields must be filled in.");
        return;
    }


    // Getting curent date.
    QDateTime currentDateTime = QDateTime::currentDateTime();
    const QString formattedDateTime = currentDateTime.toString("yyyy-MM-dd-hh-mm-ss");

    // Checking if email is valid.
    if (!isLegitEmail(email)) {
        QMessageBox::critical(this, "Error", "Invalid email format.");
        return;
    }

    // Creating the user
    if (myManager.createUser(username, first, last, email, formattedDateTime, password, -1)) {
        QStandardItemModel *model = qobject_cast<QStandardItemModel*>(ui->interestsBox->model());
        //If worked, we add the user to the database.

        // Checking which items are checked
        QStringList selectedInterests;
        for (int i = 0; i < model->rowCount(); i++) {
            QStandardItem *item = model->item(i);
            if (item->checkState() == Qt::Checked) {
                QString interestName = item->text();
                selectedInterests.append(interestName);
            }
        }

        int userId = myManager.getUserId(username);

        // Getting UserInterests in the database.
        foreach(QString interest, selectedInterests){
            if (!myManager.addUserInterest(userId, interest)){
                QMessageBox::critical(this, "Error", "Unable to add interest.");
                return;
            }
        }
        this->close();
    } else {
        QMessageBox::critical(this, "Error", "Unable to create account.");
    }
}

bool createUser::isLegitEmail(const QString &email){
    // If input matches the format, returns true. Else false.
    if (emailRegex.match(email).hasMatch()){
        return true;
    } else {
        return false;
    }
}
void createUser::on_cancelButton_clicked()
{
    this->close();
}

void createUser::on_interestsBox_activated(int index)
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





