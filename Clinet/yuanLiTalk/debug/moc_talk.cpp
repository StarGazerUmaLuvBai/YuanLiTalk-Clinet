/****************************************************************************
** Meta object code from reading C++ file 'talk.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../talk.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'talk.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Talk_t {
    QByteArrayData data[6];
    char stringdata0[64];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Talk_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Talk_t qt_meta_stringdata_Talk = {
    {
QT_MOC_LITERAL(0, 0, 4), // "Talk"
QT_MOC_LITERAL(1, 5, 12), // "readySendMsg"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 15), // "on_send_clicked"
QT_MOC_LITERAL(4, 35, 11), // "hadReadyRec"
QT_MOC_LITERAL(5, 47, 16) // "on_clear_clicked"

    },
    "Talk\0readySendMsg\0\0on_send_clicked\0"
    "hadReadyRec\0on_clear_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Talk[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   37,    2, 0x08 /* Private */,
       4,    1,   38,    2, 0x08 /* Private */,
       5,    0,   41,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,

       0        // eod
};

void Talk::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Talk *_t = static_cast<Talk *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->readySendMsg((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->on_send_clicked(); break;
        case 2: _t->hadReadyRec((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->on_clear_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Talk::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Talk::readySendMsg)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject Talk::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Talk.data,
      qt_meta_data_Talk,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Talk::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Talk::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Talk.stringdata0))
        return static_cast<void*>(const_cast< Talk*>(this));
    return QDialog::qt_metacast(_clname);
}

int Talk::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void Talk::readySendMsg(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
