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
    : QAbstractListModel(parent)
    , m_tableModel(new QSqlTableModel)
{
    m_tableModel->setTable(QStringLiteral("notebooks"));
    m_tableModel->setEditStrategy(QSqlTableModel::OnFieldChange);
    m_tableModel->select();

    connect(m_tableModel, &QSqlTableModel::rowsRemoved, [this]() {
        qDebug() << "Data changed";
    });
}

NotebooksModel::~NotebooksModel()
{
    delete m_tableModel;
}

int NotebooksModel::rowCount(const QModelIndex &index) const
{
    QModelIndex i = m_tableModel->index(index.row(), Field::Id);
    return m_tableModel->rowCount(i);
}

Qt::ItemFlags NotebooksModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

QVariant NotebooksModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
    {
        QModelIndex idx = m_tableModel->index(index.row(), Field::Title);
        return m_tableModel->data(idx).toString();
    }

    case Qt::EditRole:
    {
        QModelIndex idx = m_tableModel->index(index.row(), Field::Title);
        return m_tableModel->data(idx).toString();
    }

    case Qt::ToolTipRole:
    {
        QModelIndex idx = m_tableModel->index(index.row(), Field::Description);
        return m_tableModel->data(idx).toString();
    }

    default:
        return QVariant();
    }
}

bool NotebooksModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    QModelIndex idx = m_tableModel->index(index.row(), Field::Title);
    return m_tableModel->setData(idx, value, role);
}
