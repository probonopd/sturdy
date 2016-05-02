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

#include "settings.h"

#include <QSettings>

namespace
{
// Configuration groups
const char GroupState[] = "state";
const char GroupDisplay[] = "display";
const char GroupSync[] = "sync";
}

using namespace Core;

Settings::Settings(QObject* parent)
    : QObject(parent)
    , m_settings(new QSettings(this))
{
    load();
}

Settings::~Settings()
{
    save();
}

void Settings::load()
{
    m_settings->beginGroup(GroupState);
    windowGeometry = m_settings->value(QStringLiteral("windowGeometry")).toByteArray();
    verticalSplitterGeometry = m_settings->value(QStringLiteral("verticalSplitterGeometry")).toByteArray();
    m_settings->endGroup();

    m_settings->beginGroup(GroupDisplay);
    showOpenEntries = m_settings->value(QStringLiteral("showOpenEntries"), false).toBool();
    showTags = m_settings->value(QStringLiteral("showTags"), false).toBool();
    m_settings->endGroup();

    m_settings->beginGroup(GroupSync);
    autoSave = m_settings->value(QStringLiteral("autoSave"), false).toBool();
    autoSaveInterval = m_settings->value(QStringLiteral("autoSaveInterval"), 0).toUInt();
    m_settings->endGroup();
}

void Settings::save()
{
    m_settings->beginGroup(GroupState);
    m_settings->setValue(QStringLiteral("windowGeometry"), windowGeometry);
    m_settings->setValue(QStringLiteral("verticalSplitterGeometry"), verticalSplitterGeometry);
    m_settings->endGroup();

    m_settings->beginGroup(GroupDisplay);
    m_settings->setValue(QStringLiteral("showOpenEntries"), showOpenEntries);
    m_settings->setValue(QStringLiteral("showTags"), showTags);
    m_settings->endGroup();

    m_settings->beginGroup(GroupSync);
    m_settings->setValue(QStringLiteral("autoSave"), autoSave);
    m_settings->setValue(QStringLiteral("autoSaveInterval"), autoSaveInterval);
    m_settings->endGroup();

    m_settings->sync();

    emit updated();
}
