/***********************************************************************
**
** Sturdy - note-taking app
** Copyright (C) 2016 Vladislav Tronko <innermous@gmail.com>
**
** Sturdy is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with Sturdy. If not, see <http://www.gnu.org/licenses/>.
**
***********************************************************************/

#include "core/profilemanager.h"

#include "authorizationdialog.h"
#include "ui_authorizationdialog.h"

#include <QMessageBox>

AuthorizationDialog::AuthorizationDialog(Core::ProfileManager* mgr, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::AuthorizationDialog)
    , m_manager(mgr)
{
    m_manager->initDataDir();

    ui->setupUi(this);
    ui->cmbProfiles->addItems(m_manager->availableProfiles());

    connect(ui->logInBtn, &QPushButton::pressed, this, &AuthorizationDialog::logIn);
    connect(ui->signUpBtn, &QPushButton::pressed, this, &AuthorizationDialog::signUp);
}

AuthorizationDialog::~AuthorizationDialog()
{
    delete ui;
}

void AuthorizationDialog::logIn()
{
    const QString profileName = ui->cmbProfiles->currentText();
    if (profileName.isEmpty()) return;

    if (m_manager->initProfile(profileName)) {
        accept();
    } else {
        QMessageBox::information(0,
                                 QStringLiteral("Error"),
                                 QStringLiteral("Can't initialize profile"),
                                 QMessageBox::Ok);
    }
}

void AuthorizationDialog::signUp()
{
    const QString profileName = ui->newProfileName->text();
    if (profileName.isEmpty()) return;

    int result = m_manager->createProfile(profileName);
    switch (result)
    {
    case 0:
        QMessageBox::information(
                    0,
                    QStringLiteral("Success"),
                    QStringLiteral("Profile %1 was successfully created").arg(profileName),
                    QMessageBox::Ok);
        ui->tabWidget->setCurrentIndex(0);
        ui->cmbProfiles->addItem(profileName);
        break;

    case -1:
        QMessageBox::information(
                    0,
                    QStringLiteral("Can't create profile"),
                    QStringLiteral("Profile with such name already exists"),
                    QMessageBox::Ok);
        break;

    case -2:
        QMessageBox::critical(
                    0,
                    QStringLiteral("Something went wrong"),
                    QStringLiteral("Can't create profile"),
                    QMessageBox::Ok);
        break;

    default: break;
    }
}
