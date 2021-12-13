#-------------------------------------------------
#
# Project created by QtCreator 2017-03-31T11:22:58
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SIKTester
TEMPLATE = app

QMAKE_CXXFLAGS_RELEASE -= -O0B
QMAKE_CXXFLAGS_DEBUG += -gdwarf-3 -O0

SOURCES += main.cpp\
        mainwindow.cpp\
        comportreaderwriter.cpp\
        comportsettingswgt.cpp \
        parserwgt.cpp \


HEADERS  += mainwindow.h \
            comportreaderwriter.h\
            comportsettingswgt.h \
            parserwgt.h \

FORMS    += mainwindow.ui\
            comportsettingswgt.ui \
            parserwgt.ui

