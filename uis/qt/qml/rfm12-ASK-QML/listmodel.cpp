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

#include "listmodel.h"

ListModel::ListModel(ListItem *prototype, xmlrpc_env *envP, xmlrpc_client **clientPP,
                     QObject *parent) :
    QAbstractListModel(parent), m_prototype(prototype), m_envP(envP), m_clientPP(clientPP)
{
    setRoleNames(m_prototype->roleNames());
}

int ListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_list.size();
}

QVariant ListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_list.size())
        return QVariant();

    return m_list.at(index.row())->data(role);
}

ListModel::~ListModel()
{
    delete m_prototype;
    clear();
}

void ListModel::appendRow(ListItem *item)
{
    appendRows(QList<ListItem *>() << item);
}

void ListModel::appendRows(const QList<ListItem *> &items)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount() + items.size() - 1);
    foreach (ListItem *item, items)
    {
        connect(item, SIGNAL(dataChanged()), SLOT(handleItemChange()));
        m_list.append(item);
    }
    endInsertRows();
}

void ListModel::insertRow(int row, ListItem *item)
{
    beginInsertRows(QModelIndex(), row, row);
    connect(item, SIGNAL(dataChanged()), SLOT(handleItemChange()));
    m_list.insert(row, item);
    endInsertRows();
}

void ListModel::handleItemChange()
{
    ListItem *item = static_cast<ListItem *>(sender());
    QModelIndex index = indexOfItem(item);
    if (index.isValid())
        emit dataChanged(index, index);
}

ListItem *ListModel::find(const QString &id) const
{
    foreach (ListItem *item, m_list)
    {
        if (item->id() == id)
            return item;
    }

    return 0;
}

QModelIndex ListModel::indexOfItem(const ListItem *item) const
{
    Q_ASSERT(item);
    for (int row = 0; row < m_list.size(); ++row)
    {
        if (m_list.at(row) == item)
            return index(row);
    }

    return QModelIndex();
}

void ListModel::clear()
{
    qDeleteAll(m_list);
    m_list.clear();
}

void ListModel::refresh(QString serverURL)
{
    try
    {
        // debugging information
        std::cout << "ServerURL is " << (const char *)serverURL.toAscii() << std::endl;
        this->refreshPowerSockets(serverURL);
    } catch(QString& ex)
    {
        // we should forward this exception to the GUI
        std::cerr << "Exception: " << (const char *)ex.toAscii() << std::endl;
        //exit(1);
    }
}

void ListModel::control(QString powerSocketId, int value)
{
    try
    {
        QString methodName = "control";
        xmlrpc_value *resultP;
        int intResult = -1;
        // xmlrpc addresses look like "<server addr>:<port>/RPC2"
        xmlrpc_client_call2f(m_envP, *m_clientPP, (m_serverURL + "/RPC2").toAscii(), methodName.toAscii(),
                             &resultP, "(si)", (const char *)powerSocketId.toAscii(),
                             (xmlrpc_int32)value);
        assertOk(m_envP);
        xmlrpc_read_int(m_envP, resultP, &intResult);
        assertOk(m_envP);
        // debugging info
        //std::cout << "control: result = " << intResult << ", value = " << value << std::endl;

        // we have just turned on or off a powersocket, so we should refresh our local list
        refresh(m_serverURL);

    } catch(QString& ex)
    {
        std::cerr << "Exception: " << (const char *)ex.toAscii() << std::endl;
    }
}

void ListModel::refreshPowerSockets(QString serverURL) throw (QString)
{
    QString methodName = "get_config";
    this->m_serverURL = serverURL;
    // debug
    //std::cout << "ServerURL in refreshPowerSockets is " << (const char *)serverURL.toAscii() << std::endl;

    // get list of powersockets from the server
    xmlrpc_value *powerSocketListP;
    xmlrpc_client_call2f(m_envP, *m_clientPP, (m_serverURL + "/RPC2").toAscii(), methodName.toAscii(),
                         &powerSocketListP, "()");
    assertOk(m_envP);

    int count = xmlrpc_array_size(m_envP, powerSocketListP);
    assertOk(m_envP);

    // add each powersocket to our local list
    for (int i = 0; i < count; ++i)
    {
        xmlrpc_value *currentEntryP = xmlrpc_array_get_item(m_envP, powerSocketListP, i);
        QString id = getStringFromXmlRpcStruct(m_envP, "id", currentEntryP);
        QString label = getStringFromXmlRpcStruct(m_envP, "label", currentEntryP);
        QString code = getStringFromXmlRpcStruct(m_envP, "code", currentEntryP);
        int state = getIntFromXmlRpcStruct(m_envP, "state", currentEntryP);
        assertOk(m_envP);

        PowerSocket *powerSocketP = new PowerSocket(id, label, code, state, this);

        bool powerSocketFound = false;
        for (int j = 0; j < m_list.size(); ++j)
        {
            PowerSocket *currentPowerSocket = (PowerSocket *)m_list[j];
            // replace data of the already existing powersocket with that id
            if (currentPowerSocket->id() == powerSocketP->id())
            {
                currentPowerSocket->setLabel(powerSocketP->label());
                currentPowerSocket->setCode(powerSocketP->code());
                currentPowerSocket->setState(powerSocketP->state());

                powerSocketFound = true;
                break;
            }
        }
        // if no powersocket was found, add a new one
        if (!powerSocketFound)
            this->appendRow(powerSocketP);
    }

    // debug
    //std::cout << "List size: " << m_list.size() << std::endl;

    // just for testing purposes, not needed
    /*QModelIndex first = index(0);
    QModelIndex last = index(m_list.size());
    emit dataChanged(first, last);*/
}

bool ListModel::removeRow(int row, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    if (row < 0 || row >= m_list.size())
        return false;

    beginRemoveRows(QModelIndex(), row, row);
    delete m_list.takeAt(row);
    endRemoveRows();

    return true;
}

bool ListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    if (row < 0 || (row + count) >= m_list.size())
        return false;

    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; ++i)
        delete m_list.takeAt(row);
    endRemoveRows();

    return true;
}

ListItem *ListModel::takeRow(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    ListItem *item = m_list.takeAt(row);
    endRemoveRows();
    return item;
}

