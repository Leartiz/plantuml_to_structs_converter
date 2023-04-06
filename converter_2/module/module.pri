SOURCES += \
    $$PWD/classgraph.cpp \
    $$PWD/constructhelper.cpp \
    $$PWD/graph.cpp \
    $$PWD/grapherror.cpp \
    $$PWD/project.cpp \
    $$PWD/usecasegraph.cpp

HEADERS += \
    $$PWD/../../dependency/nlohmann/json.hpp \
    $$PWD/classgraph.h \
    $$PWD/constructhelper.h \
    $$PWD/graph.h \
    $$PWD/grapherror.h \
    $$PWD/project.h \
    $$PWD/usecasegraph.h

INCLUDEPATH += \
    ../../dependency/ \
    ../module/
