#include "create_post.h"
#include "qlistview.h"
#include "qstandarditemmodel.h"


create_post::create_post(auth_manager &myManager,  QWidget *parent):
    QDialog(parent),
    ui(new Ui::create_post),
    myManager(&myManager),
    myUpload(myManager)
{
    ui->setupUi(this);
    // Initializing the model for interestsBox
    QStandardItemModel *model = new QStandardItemModel(this);

    //Populating interestsBox with checkable interests
    foreach (QString interest, interests) {
        QStandardItem *item = new QStandardItem(interest);
        item->setCheckable(true);
        item->setCheckState(Qt::Unchecked);
        model->appendRow(item);
    }

    // Setting the model for interestsBox
    if (model != nullptr) {
        ui->interestsBox->setEditable(true);
        ui->interestsBox->setView(new QListView());
        ui->interestsBox->setModel(model);
    } else {
        qDebug() << "Model pointer is null!";
    }

    // User UI info update
    int posts = myManager.getCurrentUser()->getPostCount();
    QString postString = QString::number(posts);
    ui->usernameLabel->setText("@" + myManager.getCurrentUser()->getUsername());
    ui->nameLabel->setText(myManager.getCurrentUser()->getFirstName() + " " +
                           myManager.getCurrentUser()->getLastName());
    ui->totalPostsLabel->setText("Total Posts: " + postString);
}

create_post::~create_post()
{
    delete ui;
}

void create_post::on_postButton_clicked()
{

    // Retrieving the entered post details
    const QString title = ui->titleEdit->text();
    const QString contents = ui->textEdit->toPlainText();

    // Getting the current date and time
    QDateTime currentDateTime = QDateTime::currentDateTime();
    const QString formattedDateTime = currentDateTime.toString("yyyy-MM-dd-hh-mm-ss");

    // Fetching selected interests
    QStandardItemModel *model = qobject_cast<QStandardItemModel*>(ui->interestsBox->model());
    std::vector<std::string> myHashtags((std::vector<std::string>()));

    for (int i = 0; i < model->rowCount(); i++) {
        QStandardItem *item = model->item(i);
        if (item->checkState() == Qt::Checked) {
            QString interestName = item->text();
            myHashtags.push_back(interestName.toStdString());
        }
    }

    // Sending the post using the upload class
    if (myUpload.send(title.toStdString(), contents.toStdString(), myHashtags)){
        emit postCreated();
    }

    this->close();
}

void create_post::on_cancelButton_clicked()
{
    this->close();
}

void create_post::on_interestsBox_activated(int index)
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


void create_post::on_addButton_clicked()
{
    // Retrieving the model of interestsBox
    QStandardItemModel *model = qobject_cast<QStandardItemModel*>(ui->interestsBox->model());

    // Retrieving the new hashtag from the text edit
    QString newHasthag = ui->customEdit->toPlainText();

    // Create a new item with the new hashtag and set it as checked
    QStandardItem *item = new QStandardItem(newHasthag);

    item->setCheckState(Qt::Checked);
    model->appendRow(item);
}

