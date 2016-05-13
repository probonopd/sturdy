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

#include "entrymanager.h"

#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>

using namespace Core;

EntryManager::~EntryManager()
{
    clear();
}

void EntryManager::close(int entryId)
{
    Entry* entry = getEntry(entryId);
    if (!entry)
        return;

    delete entry;
    m_entries.erase(entryId);
}

void EntryManager::clear()
{
    for (auto entry : m_entries) {
        save(entry.first);
        delete entry.second;
    }

    m_entries.clear();
}

bool EntryManager::save(int entryId) const
{
    Entry* entry = m_entries.at(entryId);
    if (!entry->isModified)
        return true;

    entry->timestamp = QDateTime().toTime_t();
    entry->isModified = false;

    QSqlDatabase db = QSqlDatabase::database();

    QSqlQuery query(db);
    query.prepare(QStringLiteral("UPDATE entries SET content = ?, timestamp = ? WHERE id= ?"));
    query.addBindValue(entry->content);
    query.addBindValue(entry->timestamp);
    query.addBindValue(entryId);
    return query.exec();
}

bool EntryManager::load(int entryId)
{
    QSqlDatabase db = QSqlDatabase::database();

    QSqlQuery query(db);
    query.prepare(QStringLiteral("SELECT id, content, timestamp FROM entries WHERE id= ?"));
    query.addBindValue(entryId);
    if (!query.exec())
        return false;

    query.next();

    Entry* entry = new Entry;
    entry->id = query.value(0).toInt();
    entry->content = query.value(1).toString();
    entry->timestamp = query.value(2).toUInt();

    m_entries.insert(std::make_pair<>(entry->id, entry));

    return true;
}

Entry* EntryManager::getEntry(int entryId)
{
    auto it = m_entries.find(entryId);
    if (it == m_entries.end())
        if (!load(entryId))
            return nullptr;

    return m_entries.at(entryId);
}
