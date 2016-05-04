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
#include "settings.h"

#include <QDir>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QSettings>
#include <QDebug>

using namespace Core;

ProfileManager::ProfileManager(Settings* settings)
    : m_databaseConnected(false)
    , m_settings(settings)
{

}

void ProfileManager::initDataDir()
{
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    if (dir.exists()) {
        return;
    }

    dir.mkpath(dir.absolutePath());
    dir.mkdir(QLatin1String("profiles"));
}

bool ProfileManager::initProfile(const QString& profileName)
{
    Q_ASSERT(!profileName.isEmpty());

    if (!checkProfileData(profileName))
        createProfile(profileName);

    m_currentProfile = profileName;
    connectToDatabase(profileName);
    setStartingProfile(profileName);

    return m_databaseConnected;
}

bool ProfileManager::checkProfileData(const QString& profileName) const
{
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QLatin1String("/profiles/") + profileName);
    return (dir.exists() && QFile(dir.filePath(profileName)).exists());
}

void ProfileManager::resetProfile()
{
    m_currentProfile = QString();
    m_databaseConnected = false;

    const QString connection = QSqlDatabase::database().connectionName();
    QSqlDatabase().removeDatabase(connection);
}

int ProfileManager::createProfile(const QString& profileName)
{
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QLatin1String("/profiles"));

    if (!dir.mkpath(profileName)) {
        qWarning("Cant' create profile dir");
        return -1;
    }

    dir.cd(profileName);
    QFile(QLatin1String(":/data/data.db")).copy(dir.filePath(QLatin1String("data.db")));
    QFile(dir.filePath(QLatin1String("data.db"))).setPermissions(QFile::ReadUser | QFile::WriteUser);

    return 0;
}

QString ProfileManager::defaultProfile()
{
#ifdef Q_OS_WIN32
    return qgetenv("USERNAME");
#else
    return qgetenv("USER");
#endif
}

bool ProfileManager::removeProfile(const QString& profileName)
{
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QLatin1String("/profiles/") + profileName);

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
    return m_settings->startingProfile;
}

bool ProfileManager::setStartingProfile(const QString& profileName)
{
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QLatin1String("/profiles/") + profileName);
    if (!dir.exists()) {
        qWarning() << "Profile doesn't exist";
        return false;
    }

    m_settings->startingProfile = profileName;

    return true;
}

QStringList ProfileManager::availableProfiles()
{
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QLatin1String("/profiles"));
    return dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
}

void ProfileManager::connectToDatabase(const QString& profileName)
{
    const QString dbFile = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
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
