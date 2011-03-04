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
    names[StateRole] = "state";

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

int PowerSocket::state() const
{
    return this->m_state;
}
