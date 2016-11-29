/****************************************************************************
** Meta object code from reading C++ file 'mainserver.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../a_server/apikinect/mainserver.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainserver.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainServer_t {
    QByteArrayData data[39];
    char stringdata[417];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainServer_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainServer_t qt_meta_stringdata_MainServer = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainServer"
QT_MOC_LITERAL(1, 11, 10), // "printVideo"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 10), // "printDepth"
QT_MOC_LITERAL(4, 34, 12), // "printBarrido"
QT_MOC_LITERAL(5, 47, 7), // "print3D"
QT_MOC_LITERAL(6, 55, 7), // "print2D"
QT_MOC_LITERAL(7, 63, 15), // "printTimeVector"
QT_MOC_LITERAL(8, 79, 15), // "updateSrvKinect"
QT_MOC_LITERAL(9, 95, 9), // "srvKinect"
QT_MOC_LITERAL(10, 105, 7), // "newSrvK"
QT_MOC_LITERAL(11, 113, 21), // "updateClientSrvKinect"
QT_MOC_LITERAL(12, 135, 6), // "startK"
QT_MOC_LITERAL(13, 142, 6), // "indexK"
QT_MOC_LITERAL(14, 149, 5), // "stopK"
QT_MOC_LITERAL(15, 155, 12), // "updateKinect"
QT_MOC_LITERAL(16, 168, 11), // "getServerIp"
QT_MOC_LITERAL(17, 180, 15), // "getDeviceStatus"
QT_MOC_LITERAL(18, 196, 12), // "setSrvKinect"
QT_MOC_LITERAL(19, 209, 15), // "setGUISrvKinect"
QT_MOC_LITERAL(20, 225, 12), // "getSrvKinect"
QT_MOC_LITERAL(21, 238, 16), // "setCurrentKIndex"
QT_MOC_LITERAL(22, 255, 5), // "index"
QT_MOC_LITERAL(23, 261, 16), // "getCurrentKIndex"
QT_MOC_LITERAL(24, 278, 10), // "getKnumber"
QT_MOC_LITERAL(25, 289, 7), // "getTime"
QT_MOC_LITERAL(26, 297, 7), // "eOption"
QT_MOC_LITERAL(27, 305, 3), // "opt"
QT_MOC_LITERAL(28, 309, 8), // "getAccel"
QT_MOC_LITERAL(29, 318, 5), // "accel"
QT_MOC_LITERAL(30, 324, 2), // "go"
QT_MOC_LITERAL(31, 327, 4), // "stop"
QT_MOC_LITERAL(32, 332, 4), // "init"
QT_MOC_LITERAL(33, 337, 9), // "depthVoid"
QT_MOC_LITERAL(34, 347, 11), // "startServer"
QT_MOC_LITERAL(35, 359, 15), // "attendNewClient"
QT_MOC_LITERAL(36, 375, 14), // "nextVideoFrame"
QT_MOC_LITERAL(37, 390, 14), // "nextDepthFrame"
QT_MOC_LITERAL(38, 405, 11) // "next3DFrame"

    },
    "MainServer\0printVideo\0\0printDepth\0"
    "printBarrido\0print3D\0print2D\0"
    "printTimeVector\0updateSrvKinect\0"
    "srvKinect\0newSrvK\0updateClientSrvKinect\0"
    "startK\0indexK\0stopK\0updateKinect\0"
    "getServerIp\0getDeviceStatus\0setSrvKinect\0"
    "setGUISrvKinect\0getSrvKinect\0"
    "setCurrentKIndex\0index\0getCurrentKIndex\0"
    "getKnumber\0getTime\0eOption\0opt\0getAccel\0"
    "accel\0go\0stop\0init\0depthVoid\0startServer\0"
    "attendNewClient\0nextVideoFrame\0"
    "nextDepthFrame\0next3DFrame"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainServer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      32,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  174,    2, 0x06 /* Public */,
       3,    0,  175,    2, 0x06 /* Public */,
       4,    0,  176,    2, 0x06 /* Public */,
       5,    0,  177,    2, 0x06 /* Public */,
       6,    0,  178,    2, 0x06 /* Public */,
       7,    0,  179,    2, 0x06 /* Public */,
       8,    1,  180,    2, 0x06 /* Public */,
      11,    1,  183,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    1,  186,    2, 0x0a /* Public */,
      12,    0,  189,    2, 0x2a /* Public | MethodCloned */,
      14,    1,  190,    2, 0x0a /* Public */,
      14,    0,  193,    2, 0x2a /* Public | MethodCloned */,
      15,    0,  194,    2, 0x0a /* Public */,
      16,    0,  195,    2, 0x0a /* Public */,
      17,    0,  196,    2, 0x0a /* Public */,
      18,    1,  197,    2, 0x0a /* Public */,
      19,    1,  200,    2, 0x0a /* Public */,
      20,    0,  203,    2, 0x0a /* Public */,
      21,    1,  204,    2, 0x0a /* Public */,
      23,    0,  207,    2, 0x0a /* Public */,
      24,    0,  208,    2, 0x0a /* Public */,
      25,    1,  209,    2, 0x0a /* Public */,
      28,    0,  212,    2, 0x0a /* Public */,
      30,    0,  213,    2, 0x0a /* Public */,
      31,    0,  214,    2, 0x0a /* Public */,
      32,    0,  215,    2, 0x08 /* Private */,
      33,    0,  216,    2, 0x08 /* Private */,
      34,    0,  217,    2, 0x08 /* Private */,
      35,    0,  218,    2, 0x08 /* Private */,
      36,    0,  219,    2, 0x08 /* Private */,
      37,    0,  220,    2, 0x08 /* Private */,
      38,    0,  221,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void, 0x80000000 | 9,   10,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::QString,
    QMetaType::Int,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void, 0x80000000 | 9,   10,
    0x80000000 | 9,
    QMetaType::Void, QMetaType::Int,   22,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Int, 0x80000000 | 26,   27,
    0x80000000 | 29,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Int,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainServer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainServer *_t = static_cast<MainServer *>(_o);
        switch (_id) {
        case 0: _t->printVideo(); break;
        case 1: _t->printDepth(); break;
        case 2: _t->printBarrido(); break;
        case 3: _t->print3D(); break;
        case 4: _t->print2D(); break;
        case 5: _t->printTimeVector(); break;
        case 6: _t->updateSrvKinect((*reinterpret_cast< srvKinect(*)>(_a[1]))); break;
        case 7: _t->updateClientSrvKinect((*reinterpret_cast< srvKinect(*)>(_a[1]))); break;
        case 8: _t->startK((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->startK(); break;
        case 10: _t->stopK((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->stopK(); break;
        case 12: _t->updateKinect(); break;
        case 13: { QString _r = _t->getServerIp();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 14: { int _r = _t->getDeviceStatus();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 15: _t->setSrvKinect((*reinterpret_cast< srvKinect(*)>(_a[1]))); break;
        case 16: _t->setGUISrvKinect((*reinterpret_cast< srvKinect(*)>(_a[1]))); break;
        case 17: { srvKinect _r = _t->getSrvKinect();
            if (_a[0]) *reinterpret_cast< srvKinect*>(_a[0]) = _r; }  break;
        case 18: _t->setCurrentKIndex((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: { int _r = _t->getCurrentKIndex();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 20: { int _r = _t->getKnumber();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 21: { int _r = _t->getTime((*reinterpret_cast< eOption(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 22: { accel _r = _t->getAccel();
            if (_a[0]) *reinterpret_cast< accel*>(_a[0]) = _r; }  break;
        case 23: _t->go(); break;
        case 24: _t->stop(); break;
        case 25: _t->init(); break;
        case 26: { int _r = _t->depthVoid();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 27: _t->startServer(); break;
        case 28: _t->attendNewClient(); break;
        case 29: _t->nextVideoFrame(); break;
        case 30: _t->nextDepthFrame(); break;
        case 31: _t->next3DFrame(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MainServer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainServer::printVideo)) {
                *result = 0;
            }
        }
        {
            typedef void (MainServer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainServer::printDepth)) {
                *result = 1;
            }
        }
        {
            typedef void (MainServer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainServer::printBarrido)) {
                *result = 2;
            }
        }
        {
            typedef void (MainServer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainServer::print3D)) {
                *result = 3;
            }
        }
        {
            typedef void (MainServer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainServer::print2D)) {
                *result = 4;
            }
        }
        {
            typedef void (MainServer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainServer::printTimeVector)) {
                *result = 5;
            }
        }
        {
            typedef void (MainServer::*_t)(srvKinect );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainServer::updateSrvKinect)) {
                *result = 6;
            }
        }
        {
            typedef void (MainServer::*_t)(srvKinect );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainServer::updateClientSrvKinect)) {
                *result = 7;
            }
        }
    }
}

const QMetaObject MainServer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MainServer.data,
      qt_meta_data_MainServer,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MainServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainServer::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MainServer.stringdata))
        return static_cast<void*>(const_cast< MainServer*>(this));
    return QObject::qt_metacast(_clname);
}

int MainServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 32)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 32;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 32)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 32;
    }
    return _id;
}

// SIGNAL 0
void MainServer::printVideo()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void MainServer::printDepth()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void MainServer::printBarrido()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void MainServer::print3D()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}

// SIGNAL 4
void MainServer::print2D()
{
    QMetaObject::activate(this, &staticMetaObject, 4, Q_NULLPTR);
}

// SIGNAL 5
void MainServer::printTimeVector()
{
    QMetaObject::activate(this, &staticMetaObject, 5, Q_NULLPTR);
}

// SIGNAL 6
void MainServer::updateSrvKinect(srvKinect _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void MainServer::updateClientSrvKinect(srvKinect _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_END_MOC_NAMESPACE
