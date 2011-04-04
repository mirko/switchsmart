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

#include "powersocket.h"

PowerSocket::PowerSocket(QString &id, QString &label, QString &code, int state,
                         QObject *parent) : ListItem(parent)
{
    this->m_id = id;
    this->m_label = label;
    this->m_code = code;
    this->m_state = state;
}

QHash<int, QByteArray> PowerSocket::roleNames() const
{
    QHash<int, QByteArray> names;
    names[IdRole] = "id";
    names[LabelRole] = "label";
    names[CodeRole] = "code";
    names[StateRole] = "socketState";

    return names;
}

QVariant PowerSocket::data(int role) const
{
    switch (role)
    {
    case IdRole:
        return id();
    case LabelRole:
        return label();
    case CodeRole:
        return code();
    case StateRole:
        return state();
    default:
        return QVariant();
    }
}

QString PowerSocket::id() const
{
    return this->m_id;
}

QString PowerSocket::label() const
{
    return this->m_label;
}
void PowerSocket::setLabel(QString label)
{
    if (label != m_label)
    {
        this->m_label = label;
        emit dataChanged();
    }
}

QString PowerSocket::code() const
{
    return this->m_code;
}
void PowerSocket::setCode(QString code)
{
    if (code != m_code)
    {
        this->m_code = code;
        emit dataChanged();
    }
}

qint8 PowerSocket::state() const
{
    return this->m_state;
}
QString PowerSocket::stateAsQString() const
{
    char foo[2] = "0";
    foo[0] += m_state;
    const char *bar = (const char *)foo;
    return QString(bar);
}
void PowerSocket::setState(int state)
{
    if (state != m_state)
    {
        this->m_state = state;
        emit dataChanged();
    }
}
