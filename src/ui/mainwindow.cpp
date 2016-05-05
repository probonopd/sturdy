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

#include "core/application.h"
#include "core/settings.h"
#include "core/profilemanager.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCoreApplication>
#include <QDesktopServices>
#include <QMessageBox>

MainWindow::MainWindow(Core::Application* app, QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_application(app)
    , m_settings(app->settings())
    , m_profileManager(app->profileManager())
{
    ui->setupUi(this);

    restoreGeometry(m_settings->windowGeometry);
    ui->splitter->restoreState(m_settings->verticalSplitterGeometry);

    // Menu

    // - File

    // -- Switch profile
    updateSwitchProfileMenu();

    // -- Quit
    if (QKeySequence(QKeySequence::Quit).isEmpty())
        ui->actionQuit->setShortcut(QStringLiteral("Ctrl+Q"));
    else
        ui->actionQuit->setShortcut(QKeySequence::Quit);

    connect(ui->actionQuit, &QAction::triggered, qApp, QCoreApplication::quit);

    // - Help

    // -- Submit feedback
    connect(ui->actionFeedback, &QAction::triggered, [this]() {
        QDesktopServices::openUrl(QUrl(QStringLiteral("https://github.com/innermous/sturdy/issues")));
    });

    // -- About Qt
    connect(ui->actionAboutQt, &QAction::triggered, [this]() {
        QMessageBox::aboutQt(this);
    });
}

MainWindow::~MainWindow()
{
    m_settings->verticalSplitterGeometry = ui->splitter->saveState();
    m_settings->windowGeometry = saveGeometry();

    delete ui;
}

void MainWindow::updateSwitchProfileMenu()
{
    QMenu* menu = ui->menuSwitchProfile;

    // clear old actions
    qDeleteAll(menu->actions());

    QStringList availableProfiles = m_profileManager->availableProfiles();
    QString currentProfile = m_profileManager->currentProfile();
    for (const auto& profile : availableProfiles) {
        QAction* action = new QAction(profile, menu);
        menu->addAction(action);
        if (profile == m_profileManager->currentProfile())
            action->setEnabled(false);
        else
            connect(action, &QAction::triggered, [=]() {
                requestProfileChange(profile);
            });
    }
}

void MainWindow::requestProfileChange(const QString& profile)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(0,
                                  "",
                                  QStringLiteral("Switch to profile <i>%1</i>?").arg(profile),
                                  QMessageBox::Yes | QMessageBox::Cancel);

    if (reply == QMessageBox::Yes) {
        m_profileManager->closeProfile();
        loadProfile(profile);
        updateSwitchProfileMenu();
    }
}

void MainWindow::loadProfile(const QString& profile)
{
    if (!m_profileManager->initProfile(profile)) {
        QMessageBox::critical(0,
                              QStringLiteral("Something went wrong"),
                              QStringLiteral("Profile failed to load"),
                              QMessageBox::Ok
                              );
        return;
    }
}
