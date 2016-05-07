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

#include "notebooksmodel.h"

#include <QSqlQuery>
#include <QDebug>

using namespace Mvc;

NotebooksModel::NotebooksModel(QObject* parent)
    : QSqlTableModel(parent)
{
    setTable(QStringLiteral("notebooks"));
    setEditStrategy(QSqlTableModel::OnFieldChange);
    select();
}

int NotebooksModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 1;
}

Qt::ItemFlags NotebooksModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

QVariant NotebooksModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch(role)
    {
    case Qt::DisplayRole:
        return QSqlTableModel::data(createIndex(index.row(), Field::Title), role);

    case Qt::EditRole:
        return QSqlTableModel::data(createIndex(index.row(), Field::Title), role);

    case Qt::ToolTipRole:
        return QSqlTableModel::data(createIndex(index.row(), Field::Description), role).toString();

    default:
        return QVariant();
    }
}

bool NotebooksModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return QSqlTableModel::setData(createIndex(index.row(), Field::Title), value, role);
}
