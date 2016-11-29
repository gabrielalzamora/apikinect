/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../a_server/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[34];
    char stringdata[364];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 16), // "srvKinectChanged"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 9), // "srvKinect"
QT_MOC_LITERAL(4, 39, 7), // "newSrvK"
QT_MOC_LITERAL(5, 47, 10), // "ledChanged"
QT_MOC_LITERAL(6, 58, 3), // "lop"
QT_MOC_LITERAL(7, 62, 9), // "irChanged"
QT_MOC_LITERAL(8, 72, 1), // "i"
QT_MOC_LITERAL(9, 74, 2), // "go"
QT_MOC_LITERAL(10, 77, 6), // "kindex"
QT_MOC_LITERAL(11, 84, 4), // "stop"
QT_MOC_LITERAL(12, 89, 10), // "paintVideo"
QT_MOC_LITERAL(13, 100, 10), // "paintDepth"
QT_MOC_LITERAL(14, 111, 12), // "paintBarrido"
QT_MOC_LITERAL(15, 124, 16), // "paintBarridoAxes"
QT_MOC_LITERAL(16, 141, 7), // "paint3D"
QT_MOC_LITERAL(17, 149, 7), // "paint2D"
QT_MOC_LITERAL(18, 157, 15), // "printTimeVector"
QT_MOC_LITERAL(19, 173, 12), // "setSrvKinect"
QT_MOC_LITERAL(20, 186, 12), // "getSrvKinect"
QT_MOC_LITERAL(21, 199, 4), // "init"
QT_MOC_LITERAL(22, 204, 11), // "setServerIp"
QT_MOC_LITERAL(23, 216, 9), // "putKcombo"
QT_MOC_LITERAL(24, 226, 12), // "initconnects"
QT_MOC_LITERAL(25, 239, 11), // "apiconnects"
QT_MOC_LITERAL(26, 251, 15), // "on_pbGo_clicked"
QT_MOC_LITERAL(27, 267, 17), // "on_pbStop_clicked"
QT_MOC_LITERAL(28, 285, 18), // "on_combo_activated"
QT_MOC_LITERAL(29, 304, 5), // "index"
QT_MOC_LITERAL(30, 310, 13), // "sliderVideoUp"
QT_MOC_LITERAL(31, 324, 13), // "sliderDepthUp"
QT_MOC_LITERAL(32, 338, 10), // "slider3DUp"
QT_MOC_LITERAL(33, 349, 14) // "sliderModuleUp"

    },
    "MainWindow\0srvKinectChanged\0\0srvKinect\0"
    "newSrvK\0ledChanged\0lop\0irChanged\0i\0"
    "go\0kindex\0stop\0paintVideo\0paintDepth\0"
    "paintBarrido\0paintBarridoAxes\0paint3D\0"
    "paint2D\0printTimeVector\0setSrvKinect\0"
    "getSrvKinect\0init\0setServerIp\0putKcombo\0"
    "initconnects\0apiconnects\0on_pbGo_clicked\0"
    "on_pbStop_clicked\0on_combo_activated\0"
    "index\0sliderVideoUp\0sliderDepthUp\0"
    "slider3DUp\0sliderModuleUp"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      26,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  144,    2, 0x06 /* Public */,
       5,    1,  147,    2, 0x06 /* Public */,
       7,    1,  150,    2, 0x06 /* Public */,
       9,    1,  153,    2, 0x06 /* Public */,
      11,    1,  156,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    0,  159,    2, 0x0a /* Public */,
      13,    0,  160,    2, 0x0a /* Public */,
      14,    0,  161,    2, 0x0a /* Public */,
      15,    0,  162,    2, 0x0a /* Public */,
      16,    0,  163,    2, 0x0a /* Public */,
      17,    0,  164,    2, 0x0a /* Public */,
      18,    0,  165,    2, 0x0a /* Public */,
      19,    1,  166,    2, 0x0a /* Public */,
      20,    0,  169,    2, 0x0a /* Public */,
      21,    0,  170,    2, 0x08 /* Private */,
      22,    0,  171,    2, 0x08 /* Private */,
      23,    0,  172,    2, 0x08 /* Private */,
      24,    0,  173,    2, 0x08 /* Private */,
      25,    0,  174,    2, 0x08 /* Private */,
      26,    0,  175,    2, 0x08 /* Private */,
      27,    0,  176,    2, 0x08 /* Private */,
      28,    1,  177,    2, 0x08 /* Private */,
      30,    1,  180,    2, 0x08 /* Private */,
      31,    1,  183,    2, 0x08 /* Private */,
      32,    1,  186,    2, 0x08 /* Private */,
      33,    1,  189,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,   10,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3,    4,
    0x80000000 | 3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   29,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->srvKinectChanged((*reinterpret_cast< srvKinect(*)>(_a[1]))); break;
        case 1: _t->ledChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->irChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->go((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->stop((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->paintVideo(); break;
        case 6: _t->paintDepth(); break;
        case 7: _t->paintBarrido(); break;
        case 8: _t->paintBarridoAxes(); break;
        case 9: _t->paint3D(); break;
        case 10: _t->paint2D(); break;
        case 11: _t->printTimeVector(); break;
        case 12: _t->setSrvKinect((*reinterpret_cast< srvKinect(*)>(_a[1]))); break;
        case 13: { srvKinect _r = _t->getSrvKinect();
            if (_a[0]) *reinterpret_cast< srvKinect*>(_a[0]) = _r; }  break;
        case 14: _t->init(); break;
        case 15: _t->setServerIp(); break;
        case 16: _t->putKcombo(); break;
        case 17: _t->initconnects(); break;
        case 18: _t->apiconnects(); break;
        case 19: _t->on_pbGo_clicked(); break;
        case 20: _t->on_pbStop_clicked(); break;
        case 21: _t->on_combo_activated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 22: _t->sliderVideoUp((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 23: _t->sliderDepthUp((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 24: _t->slider3DUp((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 25: _t->sliderModuleUp((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MainWindow::*_t)(srvKinect );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::srvKinectChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (MainWindow::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::ledChanged)) {
                *result = 1;
            }
        }
        {
            typedef void (MainWindow::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::irChanged)) {
                *result = 2;
            }
        }
        {
            typedef void (MainWindow::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::go)) {
                *result = 3;
            }
        }
        {
            typedef void (MainWindow::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::stop)) {
                *result = 4;
            }
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 26)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 26;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 26)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 26;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::srvKinectChanged(srvKinect _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MainWindow::ledChanged(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MainWindow::irChanged(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MainWindow::go(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void MainWindow::stop(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
