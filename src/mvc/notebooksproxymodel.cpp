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
#include <QFont>
#include <QSqlQuery>

using namespace Mvc;

NotebooksProxyModel::NotebooksProxyModel(QObject* parent)
    : QIdentityProxyModel(parent)
{

}

int NotebooksProxyModel::rowCount(const QModelIndex& idx) const
{
    return QIdentityProxyModel::rowCount(idx) + 1;
}

QVariant NotebooksProxyModel::data(const QModelIndex& idx, int role) const
{
    if (idx.row() == rowCount() - 1) {
        switch (role)
        {
        case Qt::DisplayRole:
            return QStringLiteral("Add new notebook");

        case Qt::FontRole:
        {
            QFont font;
            font.setItalic(true);
            return font;
        }

        default: return QVariant();
        }
    }

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

Qt::ItemFlags NotebooksProxyModel::flags(const QModelIndex& idx) const
{
    return QIdentityProxyModel::flags(idx) | Qt::ItemIsEditable;
}

bool NotebooksProxyModel::setData(const QModelIndex& idx, const QVariant& value, int role)
{
    if (value.toString().trimmed().isEmpty())
        return false;

    if (idx.row() != rowCount() - 1)
        return QIdentityProxyModel::setData(index(idx.row(), Field::Title), value, role);

    // Handle "Add new notebook" scenario
    int newrow = rowCount() - 1;

    // Check uniqueness
    QStringList lstTitles;
    for (int row = 0; row < newrow; ++row)
        lstTitles << data(QIdentityProxyModel::index(row, Field::Title)).toString();
    if (lstTitles.contains(value.toString()))
        return false;
    else {
        sourceModel()->insertRow(newrow);
        return QIdentityProxyModel::setData(QIdentityProxyModel::index(newrow, Field::Title), value);
    }
}

QModelIndex NotebooksProxyModel::index(int row, int column, const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    return createIndex(row, column);
}

bool NotebooksProxyModel::removeRows(int row, int count, const QModelIndex &parent)
{
    for (int i = row; i < row + count; ++i) {
        int id = QIdentityProxyModel::data(index(row, Field::Id)).toInt();

        // Cascade removing depending entries
        QSqlDatabase db = QSqlDatabase::database();
        QSqlQuery query(db);
        query.prepare(QStringLiteral("DELETE FROM ENTRIES WHERE notebook_id = ?"));
        query.addBindValue(id);
        query.exec();
    }

    return QIdentityProxyModel::removeRows(row, count, parent);
}

void NotebooksProxyModel::setCurrentNotebook(const QModelIndex& idx, const QModelIndex& pidx)
{
    Q_UNUSED (pidx)

    // Don't do anything for virtual row
    if (idx.row() == QIdentityProxyModel::rowCount())
        return;

    int id = QIdentityProxyModel::data(index(idx.row(), Field::Id)).toInt();
    emit notebookChanged(id);
}
