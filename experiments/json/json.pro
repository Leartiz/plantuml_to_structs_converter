TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

INCLUDEPATH += \
    ../../dependency/
HEADERS += \
    ../../dependency/nlohmann/json/json.hpp
