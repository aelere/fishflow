#ifndef CHANGE_PROFILE_H
#define CHANGE_PROFILE_H

#include <QMainWindow>
#include "../authentication_engine/auth_manager.h"
#include "QtCore/qregularexpression.h"

namespace Ui {
class change_profile;
}

// Class: change_profile
// ---------------------
// This class represents the profile change window.
// It allows the user to modify their profile information, including
// username, first name, last name, email, and password. The class
// communicates with the auth_manager to update the user information
// and handles user interactions through slots. Includes
// helper methods to validate email addresses and manage user interests.

class change_profile : public QMainWindow
{
    Q_OBJECT

public:
    // Constructor for the change_profile class
    explicit change_profile(QMainWindow* mainWindow, QWidget *parent, auth_manager& manager);

    // Destructor for the change_profile class
    ~change_profile();

private slots:
    // Slot triggered when the cancel button is clicked
    void on_cancelButton_clicked();

    // Slot triggered when the submit button is clicked
    void on_submitButton_clicked();

    // Method that checks the legitimacy of an email address
    bool isLegitEmail(const QString &email);

    // Slot triggered when an item in the interests box is activated
    void on_interestsBox_activated(int index);

private:
    Ui::change_profile *ui;
    user* currentUser;
    QMainWindow* mainWindow;

    // Authentication Engine
    auth_manager& myManager;

    // Regular expression for email format
    QRegularExpression emailRegex;

    // User interests
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

#endif // CHANGE_PROFILE_H
