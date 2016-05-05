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

#ifndef PROFILEMANAGER_H
#define PROFILEMANAGER_H

#include <QString>

namespace Core
{

class Settings;
class ProfileManager
{
public:
    explicit ProfileManager(Settings* settings);

    void initDataDir();
    bool checkProfileData(const QString& profileName) const;
    bool initProfile(const QString& profileName);
    void closeProfile();

    // Return 0: success, -1: can't create dir
    int createProfile(const QString& profileName);
    bool removeProfile(const QString& profileName);

    QString currentProfile();
    QString defaultProfile();
    QString startingProfile();
    bool setStartingProfile(const QString& profileName);

    QStringList availableProfiles();

    bool isDatabaseConnected();

private:
    void connectToDatabase(const QString& profileName);
    bool m_databaseConnected;

    QString m_currentProfile;

    Settings* m_settings;
};

};

#endif // PROFILEMANAGER_H
