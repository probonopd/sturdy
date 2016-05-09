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

#include "notebooksproxymodel.h"

#include <QSqlTableModel>

using namespace Mvc;

NotebooksProxyModel::NotebooksProxyModel(QObject* parent)
    : QIdentityProxyModel(parent)
{

}

int NotebooksProxyModel::rowCount(const QModelIndex &idx) const
{
    return QIdentityProxyModel::rowCount(idx);
}

QVariant NotebooksProxyModel::data(const QModelIndex &idx, int role) const
{
    switch (role)
    {
    case Qt::DisplayRole:
        return QIdentityProxyModel::data(index(idx.row(), Field::Title), role);
    case Qt::EditRole:
        return QIdentityProxyModel::data(index(idx.row(), Field::Title));
    case Qt::ToolTipRole:
        return QIdentityProxyModel::data(index(idx.row(), Field::Description));
    default:
        return QIdentityProxyModel::data(idx, role);
    }
}

Qt::ItemFlags NotebooksProxyModel::flags(const QModelIndex &idx) const
{
    return QAbstractItemModel::flags(idx) | Qt::ItemIsEditable;
}

bool NotebooksProxyModel::setData(const QModelIndex &idx, const QVariant &value, int role)
{
    return QIdentityProxyModel::setData(index(idx.row(), Field::Title), value, role);
}
