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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>

class QSettings;

namespace Core
{

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject* parent = nullptr);
    ~Settings();

    // Settings groups
    // -Start
    QString startingProfile;

    // - State
    QByteArray windowGeometry;
    QByteArray verticalSplitterGeometry;
    QByteArray selectionSplitterGeometry;


    // - Display options
    bool showOpenEntries;
    bool showTags;

    // - Synchronization options
    bool autoSave;
    uint autoSaveInterval;

public slots:
    void load();
    void save();

signals:
    void updated();

private:
    QSettings* m_settings;
    QSettings* m_ini;
};

}

#endif // SETTINGS_H
