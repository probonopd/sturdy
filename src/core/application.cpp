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
#include "ui/mainwindow.h"

using namespace Core;

Application::Application(QObject* parent)
    : QObject(parent)
{
    m_settings = new Settings(this);
    m_profileManager = new ProfileManager(m_settings);
}

Application::~Application()
{
    delete m_profileManager;
}

void Application::run()
{
    m_profileManager->initDataDir();

    QString profile = m_profileManager->startingProfile();
    if (profile.isEmpty())
        profile = m_profileManager->defaultProfile();

    m_profileManager->initProfile(profile);

    m_mainwindow.reset(new MainWindow(this));
    m_mainwindow->show();
}

Settings* Application::settings() const
{
    return m_settings;
}

ProfileManager *Application::profileManager() const
{
    return m_profileManager;
}
