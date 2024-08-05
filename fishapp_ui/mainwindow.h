#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "../authentication_engine/auth_manager.h"
#include "../moderation/moderation_manager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Class: mainwindow
// ---------------------
// This class represents the main window. It enables the user to create a new account,
// log into an existing account, log in as a moderator, or browse the platform anonymously.
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Constructor that takes dbaccess and mymanager
    MainWindow(QWidget *parent, db_access& dbAccess, auth_manager& myManager, moderation_manager& myModManager);

    // Constructor that only takes myManage
    MainWindow(db_access& dbAccess, auth_manager& myManager,  moderation_manager& myModManager);

    // Display the dialog widget within the stacked widget (resize)
    void displayDialog(QWidget* dialog);

private slots:

    // Slots for each button present on the feed
    void on_createAccount_clicked();

    void on_loginUserButton_clicked();

    void on_loginModeratorButton_clicked();

    void on_anonUserButton_clicked();

private:
    Ui::MainWindow *ui;
    QStackedWidget *stackedWidget;
    db_access& m_dbAccess;
    auth_manager& myManager;
    moderation_manager& myModManager;

};
#endif // MAINWINDOW_H
