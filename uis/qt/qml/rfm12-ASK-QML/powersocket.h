#ifndef POWERSOCKET_H
#define POWERSOCKET_H

#include <QObject>
#include "listmodel.h"

class PowerSocket : public ListItem
{
    Q_OBJECT

public:
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
    int state() const;

signals:

public slots:

private:
    QString m_id;
    QString m_label;
    QString m_code;
    int m_state;

};

#endif // POWERSOCKET_H
