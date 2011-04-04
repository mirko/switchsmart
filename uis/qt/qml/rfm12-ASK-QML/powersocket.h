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

#ifndef POWERSOCKET_H
#define POWERSOCKET_H

#include <QObject>
#include "listmodel.h"
#include "listitem.h"

class PowerSocket : public ListItem
{
    Q_OBJECT

public:
    /**
      * Roles for accessing members from QML
      */
    enum Roles
    {
        IdRole = Qt::UserRole + 1,
        LabelRole,
        CodeRole,
        StateRole
    };

    PowerSocket(QObject *parent = 0) : ListItem(parent) { }
    explicit PowerSocket(QString &id, QString &label, QString &code, int state,
                         QObject *parent = 0);

    QVariant data(int role) const;
    QHash<int, QByteArray> roleNames() const;

    QString id() const;
    QString label() const;
    void setLabel(QString label);
    QString code() const;
    void setCode(QString code);
    qint8 state() const;
    QString stateAsQString() const;
    void setState(int state);

signals:

public slots:

private:
    QString m_id;
    QString m_label;
    QString m_code;
    qint8 m_state;

};

#endif // POWERSOCKET_H
