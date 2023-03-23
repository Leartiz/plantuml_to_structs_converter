SOURCES += \
    $$PWD/common/errors/err_text_creator.cpp \
    $$PWD/common/errors/error.cpp \
    $$PWD/common/errors/invalid_edge.cpp \
    $$PWD/common/errors/invalid_node.cpp \
    $$PWD/common/errors/null_edge.cpp \
    $$PWD/common/errors/null_node.cpp \
    $$PWD/common/errors/repeating_edge.cpp \
    $$PWD/common/errors/repeating_node.cpp \
    $$PWD/common/errors/unsuitable_edge.cpp \
    $$PWD/structures/class/class_dia.cpp \
    $$PWD/structures/description.cpp \
    $$PWD/structures/project.cpp \
    $$PWD/structures/robustness/robustness_dia.cpp \
    $$PWD/structures/sequence/sequence_dia.cpp \
    $$PWD/structures/use_case/uc_edge.cpp \
    $$PWD/structures/use_case/uc_node.cpp \
    $$PWD/structures/use_case/use_case_dia.cpp \
    $$PWD/translator/translator.cpp \
    $$PWD/translator/use_case/uc_lexic_analyzer.cpp \
    $$PWD/translator/use_case/uc_syntac_analyzer.cpp

HEADERS += \
    $$PWD/common/errors/err_text_creator.h \
    $$PWD/common/errors/error.h \
    $$PWD/common/errors/invalid_edge.h \
    $$PWD/common/errors/invalid_node.h \
    $$PWD/common/errors/null_edge.h \
    $$PWD/common/errors/null_node.h \
    $$PWD/common/errors/repeating_edge.h \
    $$PWD/common/errors/repeating_node.h \
    $$PWD/common/errors/unsuitable_edge.h \
    $$PWD/common/ijson.h \
    $$PWD/../../dependency/nlohmann/json.hpp \
    $$PWD/common/ivalid.h \
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
    $$PWD/translator/translator.h \
    $$PWD/translator/use_case/uc_lexic_analyzer.h \
    $$PWD/translator/use_case/uc_syntac_analyzer.h

INCLUDEPATH += \
    ../../dependency/ \
    ../module/structures/ \
    ../module/common/ \
    ../module/
