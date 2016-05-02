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

#include "application.h"
#include "settings.h"
#include "profilemanager.h"
#include "ui/authorizationdialog.h"

using namespace Core;

Application::Application(QObject* parent)
    : QObject(parent)
{
    m_settings = new Settings(this);
    m_profileManager = new ProfileManager();
}

Application::~Application()
{
    delete m_profileManager;
}

void Application::switchProfile()
{
    if (!m_profileManager->startingProfile().isEmpty())
    {
        // launch mainwindow
        return;
    }

    AuthorizationDialog* dialog = new AuthorizationDialog(m_profileManager);

    if (dialog->exec()) {
        // launch mainwindow
    }

    dialog->deleteLater();
}

void Application::run()
{
    switchProfile();
}
