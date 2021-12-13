/****************************************************************************
** Meta object code from reading C++ file 'parserwgt.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../parserwgt.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'parserwgt.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ParserWgt_t {
    QByteArrayData data[11];
    char stringdata0[95];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ParserWgt_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ParserWgt_t qt_meta_stringdata_ParserWgt = {
    {
QT_MOC_LITERAL(0, 0, 9), // "ParserWgt"
QT_MOC_LITERAL(1, 10, 10), // "x4101_data"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 7), // "x4101_t"
QT_MOC_LITERAL(4, 30, 4), // "data"
QT_MOC_LITERAL(5, 35, 10), // "x4102_data"
QT_MOC_LITERAL(6, 46, 10), // "x4104_data"
QT_MOC_LITERAL(7, 57, 7), // "x4104_t"
QT_MOC_LITERAL(8, 65, 9), // "get_x4101"
QT_MOC_LITERAL(9, 75, 9), // "get_x4102"
QT_MOC_LITERAL(10, 85, 9) // "get_x4104"

    },
    "ParserWgt\0x4101_data\0\0x4101_t\0data\0"
    "x4102_data\0x4104_data\0x4104_t\0get_x4101\0"
    "get_x4102\0get_x4104"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ParserWgt[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       5,    1,   47,    2, 0x06 /* Public */,
       6,    1,   50,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    1,   53,    2, 0x0a /* Public */,
       9,    1,   56,    2, 0x0a /* Public */,
      10,    1,   59,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::UInt,    4,
    QMetaType::Void, 0x80000000 | 7,    4,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::UInt,    4,
    QMetaType::Void, 0x80000000 | 7,    4,

       0        // eod
};

void ParserWgt::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ParserWgt *_t = static_cast<ParserWgt *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->x4101_data((*reinterpret_cast< x4101_t(*)>(_a[1]))); break;
        case 1: _t->x4102_data((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 2: _t->x4104_data((*reinterpret_cast< x4104_t(*)>(_a[1]))); break;
        case 3: _t->get_x4101((*reinterpret_cast< x4101_t(*)>(_a[1]))); break;
        case 4: _t->get_x4102((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 5: _t->get_x4104((*reinterpret_cast< x4104_t(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ParserWgt::*)(x4101_t );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ParserWgt::x4101_data)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ParserWgt::*)(unsigned int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ParserWgt::x4102_data)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ParserWgt::*)(x4104_t );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ParserWgt::x4104_data)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ParserWgt::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_ParserWgt.data,
    qt_meta_data_ParserWgt,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ParserWgt::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ParserWgt::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ParserWgt.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ParserWgt::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void ParserWgt::x4101_data(x4101_t _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ParserWgt::x4102_data(unsigned int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ParserWgt::x4104_data(x4104_t _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
