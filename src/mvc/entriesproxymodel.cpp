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

#include "entriesproxymodel.h"

#include <QFont>
#include <QIcon>

using namespace Mvc;

EntriesProxyModel::EntriesProxyModel(QObject* parent)
    : QIdentityProxyModel(parent)
    , m_currentNotebookId(-1)
{
}

int EntriesProxyModel::rowCount(const QModelIndex& idx) const
{
    if (m_currentNotebookId < 0)
        return 0;
    return QIdentityProxyModel::rowCount(idx) + 1;
}

QVariant EntriesProxyModel::data(const QModelIndex& idx, int role) const
{
    if (!idx.isValid())
        return QVariant();

    if (idx.row() == rowCount() - 1) {
        switch (role)
        {
        case Qt::DisplayRole:
            return QStringLiteral("Add new note");

        case Qt::FontRole:
        {
            QFont font;
            font.setItalic(true);
            return font;
        }

        case Qt::DecorationRole:
            return QIcon(QStringLiteral(":/icons/icons/add_black_18dp.png"));

        default:
            return QVariant();
        }
    }

    switch (role)
    {
    case Qt::DisplayRole:
        return QIdentityProxyModel::data(index(idx.row(), Field::Title), role);
    case Qt::EditRole:
        return QIdentityProxyModel::data(index(idx.row(), Field::Title), role);
    default:
        return sourceModel()->data(idx, role);
    }
}

bool EntriesProxyModel::setData(const QModelIndex& idx, const QVariant& value, int role)
{
    if (value.toString().trimmed().isEmpty())
        return false;

    if (idx.row() != rowCount() - 1)
        return QIdentityProxyModel::setData(index(idx.row(), Field::Title), value, role);

    // Handle "Add new note" scenario
    int newrow = rowCount() - 1;

    // Check uniqueness
    QStringList lstTitles;
    for (int row = 0; row < newrow; ++row)
        lstTitles << data(QIdentityProxyModel::index(row, Field::Title)).toString();
    if (lstTitles.contains(value.toString()))
        return false;
    else {
        QIdentityProxyModel::insertRow(newrow);

        QIdentityProxyModel::setData(QIdentityProxyModel::index(newrow, Field::Title), value);
        QIdentityProxyModel::setData(QIdentityProxyModel::index(newrow, Field::Notebook_id), m_currentNotebookId);

        return true;
    }
}

Qt::ItemFlags EntriesProxyModel::flags(const QModelIndex& idx) const
{
    return QIdentityProxyModel::flags(idx) | Qt::ItemIsEditable;
}

QModelIndex EntriesProxyModel::index(int row, int column, const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    return createIndex(row, column);
}

bool EntriesProxyModel::removeRows(int row, int count, const QModelIndex &parent)
{
    for (int i = row; i < row + count; ++i) {
        int id = QIdentityProxyModel::data(index(row, Field::Id)).toInt();
        emit entryRemoved(id);
    }

    return QIdentityProxyModel::removeRows(row, count, parent);
}

void EntriesProxyModel::changeNotebook(int id)
{
    m_currentNotebookId = id;
}

void EntriesProxyModel::setCurrentEntry(const QModelIndex& idx, const QModelIndex& pidx)
{
    Q_UNUSED (pidx)

    // Don't do anything to virtual row
    if (idx.row() == QIdentityProxyModel::rowCount())
        return;

    int id = QIdentityProxyModel::data(index(idx.row(), Field::Id)).toInt();
    emit entrySelected(id);
}
