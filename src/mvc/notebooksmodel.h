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

#ifndef NOTEBOOKSMODEL_H
#define NOTEBOOKSMODEL_H

#include <QAbstractListModel>
#include <QSqlTableModel>

namespace Mvc
{

class NotebooksModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit NotebooksModel(QObject* parent = nullptr);
    ~NotebooksModel();

    int rowCount(const QModelIndex& index) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

private:
    enum Field {
        Id = 0,
        Title = 1,
        Description = 2,
        Count = 3
    };

    QSqlTableModel* m_tableModel;
};

}

#endif // NOTEBOOKSMODEL_H
