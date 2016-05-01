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

#include "profilemanager.h"

#include <QDir>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QSettings>
#include <QDebug>

using namespace Core;

ProfileManager::ProfileManager()
    : m_databaseConnected(false)
{

}

void ProfileManager::initConfigDir()
{
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));

    if (dir.exists() && QFile(dir.filePath(QLatin1String("profiles/profiles.ini"))).exists()) {
        return;
    }

    if (!dir.exists()) {
        dir.mkpath(dir.absolutePath());
    }

    // Create empty profiles directory
    dir.mkdir(QLatin1String("profiles"));
    dir.cd(QLatin1String("profiles"));

    QFile(dir.filePath(QLatin1String("profiles.ini"))).remove();
    QFile(QLatin1String(":/data/profiles.ini")).copy(dir.filePath(QLatin1String("profiles.ini")));
    QFile(dir.filePath(QLatin1String("profiles.ini"))).setPermissions(QFile::ReadUser | QFile::WriteUser);
}

bool ProfileManager::initProfile(const QString& profileName)
{
    Q_ASSERT(!profileName.isEmpty());

    QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + QLatin1String("/profiles/") + profileName);
    if (!dir.exists()) {
        qWarning() << "Profile doesn't exist";
        return false;
    }

    m_currentProfile = profileName;
    connectToDatabase(profileName);

    return m_databaseConnected;
}

int ProfileManager::createProfile(const QString& profileName)
{
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + QLatin1String("/profiles"));

    if (QDir(dir.absolutePath() + QLatin1Char('/') + profileName).exists()) {
        qWarning("Profile dir exists");
        return -1;
    }
    if (!dir.mkdir(profileName)) {
        qWarning("Cant' create profile dir");
        return -2;
    }

    dir.cd(profileName);
    QFile(QLatin1String(":/data/data.db")).copy(dir.filePath(QLatin1String("data.db")));
    QFile(dir.filePath(QLatin1String("data.db"))).setPermissions(QFile::ReadUser | QFile::WriteUser);

    return 0;
}

bool ProfileManager::removeProfile(const QString& profileName)
{
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + QLatin1String("/profiles/") + profileName);

    if (!dir.exists()) {
        return false;
    }

    dir.removeRecursively();

    return true;
}

QString ProfileManager::currentProfile()
{
    return m_currentProfile;
}

QString ProfileManager::startingProfile()
{
    QSettings settings(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + QLatin1String("/profiles/profiles.ini"));
    return settings.value("Profiles/startProfile").toString();
}

bool ProfileManager::setStartingProfile(const QString& profileName)
{
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + QLatin1String("/profiles/") + profileName);
    if (!dir.exists()) {
        qWarning() << "Profile doesn't exist";
        return false;
    }

    QSettings settings(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + QLatin1String("/profiles/profiles.ini"));
    settings.setValue("Profiles/startProfile", profileName);
    return true;
}

QStringList ProfileManager::availableProfiles()
{
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + QLatin1String("/profiles"));
    return dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
}

void ProfileManager::connectToDatabase(const QString& profileName)
{
    const QString dbFile = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)
            + QLatin1String("/profiles/") + profileName + QLatin1String("/data.db");

    // Reconnect
    if (m_databaseConnected) {
        QSqlDatabase::removeDatabase(QSqlDatabase::database().connectionName());
        m_databaseConnected = false;
    }

    QSqlDatabase db = QSqlDatabase::addDatabase(QLatin1String("QSQLITE"));
    db.setDatabaseName(dbFile);

    if (!QFile::exists(dbFile)) {
        qWarning("Can't find SQLite db file. Copying and using default.");

        QFile(":/data/data.db").copy(dbFile);
        QFile(dbFile).setPermissions(QFile::ReadUser | QFile::WriteUser);
        db.setDatabaseName(dbFile);
    }

    if (!db.open()) {
        qWarning("Cannot open SQLite database!");
        return;
    }

    m_databaseConnected = true;
}

bool ProfileManager::isDatabaseConnected()
{
    return m_databaseConnected;
}
