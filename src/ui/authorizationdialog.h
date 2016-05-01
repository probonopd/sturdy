#ifndef AUTHORIZATIONDIALOG_H
#define AUTHORIZATIONDIALOG_H

#include <QDialog>

namespace Core
{
class ProfileManager;
}

namespace Ui {
class AuthorizationDialog;
}

class AuthorizationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AuthorizationDialog(Core::ProfileManager* mgr, QWidget* parent = nullptr);
    ~AuthorizationDialog();

private slots:
    void logIn();
    void signUp();

private:
    Ui::AuthorizationDialog* ui;
    Core::ProfileManager* m_manager;
};

#endif // AUTHORIZATIONDIALOG_H


