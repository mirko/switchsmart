//
// rfm12-ASK-for-linux QML GUI
//
// Copyright (C) 2011 Simon Lang <slangdev@googlemail.com>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//

#ifndef LISTMODEL_H
#define LISTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QVariant>

#include <xmlrpc-c/base.h>
#include <xmlrpc-c/client.h>

#include "helperfunctions.h"
#include "listitem.h"
#include "powersocket.h"

/**
  * List model that contains all our power sockets. Used by our QML GUI.
  */
class ListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ListModel(ListItem *prototype, xmlrpc_env *envP, xmlrpc_client **clientPP,
                       QObject *parent = 0);
    ~ListModel();
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    void appendRow(ListItem *item);
    void appendRows(const QList<ListItem *> &items);
    void insertRow(int row, ListItem *item);
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    ListItem *takeRow(int row);
    ListItem *find(const QString &id) const;
    QModelIndex indexOfItem(const ListItem *item) const;
    void clear();

    /**
      * Refreshes this list of power sockets with a list from the given server address.
      * This function is called by our GUI when the refresh button or an item is clicked.
      */
    Q_INVOKABLE void refresh(QString serverURL);

    /**
      * (Remotely) sets the power socket with the given ID to the given value.
      * This function is called by our GUI when an item is clicked.
      */
    Q_INVOKABLE void control(QString powerSocketId, int value);

private slots:
    void handleItemChange();

private:
    ListItem *m_prototype;

    // local list containing all the power sockets
    QList<ListItem *> m_list;

    xmlrpc_env *m_envP;
    xmlrpc_client **m_clientPP;
    QString m_serverURL;

    void refreshPowerSockets(QString serverURL) throw (QString);

};

#endif // LISTMODEL_H
