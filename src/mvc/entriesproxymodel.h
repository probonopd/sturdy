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

#ifndef ENTRIESPROXYMODEL_H
#define ENTRIESPROXYMODEL_H

#include <QIdentityProxyModel>

namespace Mvc
{

class EntriesProxyModel : public QIdentityProxyModel
{
    Q_OBJECT
public:
    explicit EntriesProxyModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& idx = QModelIndex()) const override;
    Qt::ItemFlags flags(const QModelIndex& idx) const override;
    QVariant data(const QModelIndex &idx, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& idx, const QVariant& value, int role = Qt::EditRole) override;
    QModelIndex index(int row, int column, const QModelIndex& parent=QModelIndex()) const override;
    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;

    void changeNotebook(int id);
    void setCurrentEntry(const QModelIndex& idx, const QModelIndex& pidx = QModelIndex());

signals:
    void entrySelected(int id);
    void entryRemoved(int id);

private:
    enum Field {
        Id = 0,
        Title,
        Content,
        Timestamp,
        Notebook_id,
        Count
    };

    int m_currentNotebookId;
};

}

#endif // ENTRIESPROXYMODEL_H
