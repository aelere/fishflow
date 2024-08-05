#ifndef CREATEUSER_H
#define CREATEUSER_H

#include <QDialog>
#include "../authentication_engine/auth_manager.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <QListView>

#include <QStandardItem>


namespace Ui {
class createUser;
}

// Class: createUser
// ------------------------
// This class represents a dialog window for creating a new user account. It allows users to enter
// their username, first name, last name, email, password, and select their interests from a list.
class createUser : public QDialog
{
    Q_OBJECT

public:
    createUser(auth_manager& manager, QWidget *parent);
    ~createUser();

private slots:
    void on_submitButton_clicked();

    void on_cancelButton_clicked();

    // Method that checks email legitimacy
    bool isLegitEmail(const QString &email);

    // Method that checkmarks interests from the combobox
    void on_interestsBox_activated(int index);

private:
    Ui::createUser *ui;

    // Authentication Engine
    auth_manager& myManager;

    //Expression for email format.
    QRegularExpression emailRegex;

    //User interests
    QStringList interests = {
        "tropical_fish",
        "freshwater_fish",
        "marine_fish",
        "aquarium_maintenance",
        "fish_tank_setup",
        "fish_breeding",
        "fish_conservation",
        "aquascaping",
        "fish_photography",
        "aquatic_plants",
        "reef_tank",
        "fish_community",
        "fish_videos",
        "public_aquariums",
        "saltwater_tank"
    };
};

#endif // CREATEUSER_H
