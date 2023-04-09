SOURCES += \
    $$PWD/classgraph.cpp \
    $$PWD/constructhelper.cpp \
    $$PWD/graph.cpp \
    $$PWD/grapherror.cpp \
    $$PWD/json_utils.cpp \
    $$PWD/project.cpp \
    $$PWD/robustnessgraph.cpp \
    $$PWD/sequencegraph.cpp \
    $$PWD/str_utils.cpp \
    $$PWD/usecasegraph.cpp

HEADERS += \
    $$PWD/../../dependency/nlohmann/json.hpp \
    $$PWD/classgraph.h \
    $$PWD/constructhelper.h \
    $$PWD/graph.h \
    $$PWD/grapherror.h \
    $$PWD/json_utils.h \
    $$PWD/project.h \
    $$PWD/robustnessgraph.h \
    $$PWD/sequencegraph.h \
    $$PWD/str_utils.h \
    $$PWD/usecasegraph.h

INCLUDEPATH += \
    ../../dependency/ \
    ../module/
