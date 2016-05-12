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

#ifndef ENTRYMANAGER_H
#define ENTRYMANAGER_H

#include <unordered_map>

#include <QString>

namespace Core
{

struct Entry
{
    int id;
    QString content;
    uint timestamp;

    bool isStateChanged { false };
};

class EntryManager
{
public:
    EntryManager() = default;
    ~EntryManager();

    Entry* getEntry(int entryId) const;

    void close(int entryId);
    void clear();

    bool save(int entryId) const;
    bool load(int entryId) const;

private:
    std::unordered_map<int, Entry*> m_entries;
};

}

#endif // ENTRYMANAGER_H
