/****************************************************************************
** Meta object code from reading C++ file 'powersocket.h'
**
** Created: Fri Mar 4 16:15:52 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../rfm12-ASK-QML/powersocket.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'powersocket.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PowerSocket[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_PowerSocket[] = {
    "PowerSocket\0"
};

const QMetaObject PowerSocket::staticMetaObject = {
    { &ListItem::staticMetaObject, qt_meta_stringdata_PowerSocket,
      qt_meta_data_PowerSocket, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PowerSocket::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PowerSocket::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PowerSocket::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PowerSocket))
        return static_cast<void*>(const_cast< PowerSocket*>(this));
    return ListItem::qt_metacast(_clname);
}

int PowerSocket::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ListItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
