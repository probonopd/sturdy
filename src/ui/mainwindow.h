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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QSqlTableModel;

namespace Core {
class Application;
class Settings;
class ProfileManager;
}

namespace Ui {
class MainWindow;
}

namespace Mvc {
class NotebooksProxyModel;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Core::Application* app, QWidget* parent = nullptr);
    ~MainWindow();

    void loadProfile(const QString& profile);

private slots:
    void updateSwitchProfileMenu();

private:
    void requestProfileChange(const QString& profile);
    void closeProfile();

    Ui::MainWindow *ui;

    Core::Application* m_application;
    Core::Settings* m_settings;
    Core::ProfileManager* m_profileManager;

    QSqlTableModel* m_nbModel;
    QScopedPointer<Mvc::NotebooksProxyModel> m_nbProxyModel;
};

#endif // MAINWINDOW_H
