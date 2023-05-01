QT += core testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_module.cpp

HEADERS += \
    tst_module.h

include(../module/module.pri)
