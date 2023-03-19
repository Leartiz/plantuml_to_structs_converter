SOURCES += \
    $$PWD/structures/class/class_dia.cpp \
    $$PWD/structures/description.cpp \
    $$PWD/structures/project.cpp \
    $$PWD/structures/robustness/robustness_dia.cpp \
    $$PWD/structures/sequence/sequence_dia.cpp \
    $$PWD/structures/use_case/uc_edge.cpp \
    $$PWD/structures/use_case/uc_node.cpp \
    $$PWD/structures/use_case/use_case_dia.cpp \
    $$PWD/translator/translator.cpp

HEADERS += \
    $$PWD/common/ijson.h \
    $$PWD/dependency/json.hpp \
    $$PWD/structures/class/class_dia.h \
    $$PWD/structures/class/class_ptrs.h \
    $$PWD/structures/description.h \
    $$PWD/structures/project.h \
    $$PWD/structures/robustness/rob_ptrs.h \
    $$PWD/structures/robustness/robustness_dia.h \
    $$PWD/structures/sequence/seq_ptrs.h \
    $$PWD/structures/sequence/sequence_dia.h \
    $$PWD/structures/structure ptrs.h \
    $$PWD/structures/structure ptrs.h \
    $$PWD/structures/use_case/uc_edge.h \
    $$PWD/structures/use_case/uc_node.h \
    $$PWD/structures/use_case/uc_ptrs.h \
    $$PWD/structures/use_case/use_case_dia.h \
    $$PWD/translator/translator.h

INCLUDEPATH += \
    ../module/structures/ \
    ../module/
