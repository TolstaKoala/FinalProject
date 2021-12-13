/****************************************************************************
** Meta object code from reading C++ file 'comportreaderwriter.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../comportreaderwriter.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'comportreaderwriter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ComPortReaderWriter_t {
    QByteArrayData data[16];
    char stringdata0[195];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ComPortReaderWriter_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ComPortReaderWriter_t qt_meta_stringdata_ComPortReaderWriter = {
    {
QT_MOC_LITERAL(0, 0, 19), // "ComPortReaderWriter"
QT_MOC_LITERAL(1, 20, 9), // "port_open"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 5), // "state"
QT_MOC_LITERAL(4, 37, 10), // "x4101_data"
QT_MOC_LITERAL(5, 48, 7), // "x4101_t"
QT_MOC_LITERAL(6, 56, 4), // "data"
QT_MOC_LITERAL(7, 61, 10), // "x4102_data"
QT_MOC_LITERAL(8, 72, 11), // "receiveData"
QT_MOC_LITERAL(9, 84, 23), // "parse_com_port_settings"
QT_MOC_LITERAL(10, 108, 18), // "com_port_setting_t"
QT_MOC_LITERAL(11, 127, 17), // "com_port_settings"
QT_MOC_LITERAL(12, 145, 16), // "comPortDataReady"
QT_MOC_LITERAL(13, 162, 10), // "send0x4101"
QT_MOC_LITERAL(14, 173, 10), // "send0x4102"
QT_MOC_LITERAL(15, 184, 10) // "checkError"

    },
    "ComPortReaderWriter\0port_open\0\0state\0"
    "x4101_data\0x4101_t\0data\0x4102_data\0"
    "receiveData\0parse_com_port_settings\0"
    "com_port_setting_t\0com_port_settings\0"
    "comPortDataReady\0send0x4101\0send0x4102\0"
    "checkError"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ComPortReaderWriter[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,
       4,    1,   62,    2, 0x06 /* Public */,
       7,    1,   65,    2, 0x06 /* Public */,
       8,    1,   68,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    1,   71,    2, 0x0a /* Public */,
      12,    0,   74,    2, 0x09 /* Protected */,
      13,    0,   75,    2, 0x09 /* Protected */,
      14,    0,   76,    2, 0x09 /* Protected */,
      15,    0,   77,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, QMetaType::UInt,    6,
    QMetaType::Void, QMetaType::QByteArray,    6,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ComPortReaderWriter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ComPortReaderWriter *_t = static_cast<ComPortReaderWriter *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->port_open((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->x4101_data((*reinterpret_cast< x4101_t(*)>(_a[1]))); break;
        case 2: _t->x4102_data((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 3: _t->receiveData((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 4: _t->parse_com_port_settings((*reinterpret_cast< com_port_setting_t(*)>(_a[1]))); break;
        case 5: _t->comPortDataReady(); break;
        case 6: _t->send0x4101(); break;
        case 7: _t->send0x4102(); break;
        case 8: _t->checkError(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< com_port_setting_t >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ComPortReaderWriter::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ComPortReaderWriter::port_open)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ComPortReaderWriter::*)(x4101_t );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ComPortReaderWriter::x4101_data)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ComPortReaderWriter::*)(unsigned int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ComPortReaderWriter::x4102_data)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (ComPortReaderWriter::*)(QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ComPortReaderWriter::receiveData)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ComPortReaderWriter::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_ComPortReaderWriter.data,
    qt_meta_data_ComPortReaderWriter,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ComPortReaderWriter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ComPortReaderWriter::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ComPortReaderWriter.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ComPortReaderWriter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void ComPortReaderWriter::port_open(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ComPortReaderWriter::x4101_data(x4101_t _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ComPortReaderWriter::x4102_data(unsigned int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ComPortReaderWriter::receiveData(QByteArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
