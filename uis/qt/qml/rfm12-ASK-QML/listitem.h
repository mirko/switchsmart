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

#ifndef LISTITEM_H
#define LISTITEM_H

#include <QObject>
#include <QtGui/QApplication>
#include <QVariant>

/**
  * Abstract class for list items such as PowerSockets.
  * These are also used by QML.
  */
class ListItem : public QObject
{
    Q_OBJECT
public:
    ListItem(QObject *parent = 0) : QObject(parent) { }
    virtual ~ListItem() { }
    virtual QString id() const = 0;
    virtual QVariant data(int role) const = 0;
    virtual QHash<int, QByteArray> roleNames() const = 0;

signals:
    void dataChanged();
};

#endif // LISTITEM_H
