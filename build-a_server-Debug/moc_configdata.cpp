/****************************************************************************
** Meta object code from reading C++ file 'configdata.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../a_server/apikinect/configdata.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'configdata.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ConfigData_t {
    QByteArrayData data[16];
    char stringdata[177];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ConfigData_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ConfigData_t qt_meta_stringdata_ConfigData = {
    {
QT_MOC_LITERAL(0, 0, 10), // "ConfigData"
QT_MOC_LITERAL(1, 11, 16), // "ledOptionChanged"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 6), // "option"
QT_MOC_LITERAL(4, 36, 15), // "irOptionChanged"
QT_MOC_LITERAL(5, 52, 5), // "value"
QT_MOC_LITERAL(6, 58, 16), // "srvKinectChanged"
QT_MOC_LITERAL(7, 75, 9), // "srvKinect"
QT_MOC_LITERAL(8, 85, 7), // "newSrvK"
QT_MOC_LITERAL(9, 93, 17), // "configDataChanged"
QT_MOC_LITERAL(10, 111, 11), // "getIrOption"
QT_MOC_LITERAL(11, 123, 11), // "setIrOption"
QT_MOC_LITERAL(12, 135, 12), // "getLedOption"
QT_MOC_LITERAL(13, 148, 12), // "setLedOption"
QT_MOC_LITERAL(14, 161, 7), // "getSrvK"
QT_MOC_LITERAL(15, 169, 7) // "setSrvK"

    },
    "ConfigData\0ledOptionChanged\0\0option\0"
    "irOptionChanged\0value\0srvKinectChanged\0"
    "srvKinect\0newSrvK\0configDataChanged\0"
    "getIrOption\0setIrOption\0getLedOption\0"
    "setLedOption\0getSrvK\0setSrvK"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ConfigData[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x06 /* Public */,
       4,    1,   67,    2, 0x06 /* Public */,
       6,    1,   70,    2, 0x06 /* Public */,
       9,    0,   73,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    0,   74,    2, 0x0a /* Public */,
      11,    1,   75,    2, 0x0a /* Public */,
      12,    0,   78,    2, 0x0a /* Public */,
      13,    1,   79,    2, 0x0a /* Public */,
      14,    0,   82,    2, 0x0a /* Public */,
      15,    1,   83,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Int,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Int,
    QMetaType::Void, QMetaType::Int,    3,
    0x80000000 | 7,
    QMetaType::Void, 0x80000000 | 7,    8,

       0        // eod
};

void ConfigData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ConfigData *_t = static_cast<ConfigData *>(_o);
        switch (_id) {
        case 0: _t->ledOptionChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->irOptionChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->srvKinectChanged((*reinterpret_cast< srvKinect(*)>(_a[1]))); break;
        case 3: _t->configDataChanged(); break;
        case 4: { int _r = _t->getIrOption();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 5: _t->setIrOption((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: { int _r = _t->getLedOption();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 7: _t->setLedOption((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: { srvKinect _r = _t->getSrvK();
            if (_a[0]) *reinterpret_cast< srvKinect*>(_a[0]) = _r; }  break;
        case 9: _t->setSrvK((*reinterpret_cast< srvKinect(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ConfigData::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConfigData::ledOptionChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (ConfigData::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConfigData::irOptionChanged)) {
                *result = 1;
            }
        }
        {
            typedef void (ConfigData::*_t)(srvKinect );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConfigData::srvKinectChanged)) {
                *result = 2;
            }
        }
        {
            typedef void (ConfigData::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConfigData::configDataChanged)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject ConfigData::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ConfigData.data,
      qt_meta_data_ConfigData,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ConfigData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ConfigData::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ConfigData.stringdata))
        return static_cast<void*>(const_cast< ConfigData*>(this));
    return QObject::qt_metacast(_clname);
}

int ConfigData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void ConfigData::ledOptionChanged(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ConfigData::irOptionChanged(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ConfigData::srvKinectChanged(srvKinect _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ConfigData::configDataChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
