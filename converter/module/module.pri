SOURCES += \
    $$PWD/common/errors/any_error.cpp \
    $$PWD/common/errors/bldr/bldr_error.cpp \
    $$PWD/common/errors/bldr/node_after_adder.cpp \
    $$PWD/common/errors/bldr/not_found_node.cpp \
    $$PWD/common/errors/bldr/unknown_edge_type.cpp \
    $$PWD/common/errors/bldr/unknown_node_type.cpp \
    $$PWD/common/errors/err_text_creator.cpp \
    $$PWD/common/errors/bldr/invalid_edge.cpp \
    $$PWD/common/errors/bldr/invalid_node.cpp \
    $$PWD/common/errors/bldr/null_edge.cpp \
    $$PWD/common/errors/bldr/null_node.cpp \
    $$PWD/common/errors/bldr/repeating_edge.cpp \
    $$PWD/common/errors/bldr/repeating_node.cpp \
    $$PWD/common/errors/bldr/unsuitable_edge.cpp \
    $$PWD/common/errors/not_implemented.cpp \
    $$PWD/common/errors/tltr/lexer_error.cpp \
    $$PWD/common/errors/tltr/tltr_error.cpp \
    $$PWD/common/errors/tltr/unknown_directive.cpp \
    $$PWD/common/generator/order_generator.cpp \
    $$PWD/common/utils/conv_utils.cpp \
    $$PWD/common/utils/puml_utils.cpp \
    $$PWD/common/utils/string_utils.cpp \
    $$PWD/structures/class/class_dia.cpp \
    $$PWD/structures/description.cpp \
    $$PWD/structures/format.cpp \
    $$PWD/structures/layout.cpp \
    $$PWD/structures/layout_flow/layout_flow_dia.cpp \
    $$PWD/structures/project.cpp \
    $$PWD/structures/robustness/robustness_dia.cpp \
    $$PWD/structures/sequence/sequence_dia.cpp \
    $$PWD/structures/use_case/uc_edge.cpp \
    $$PWD/structures/use_case/uc_node.cpp \
    $$PWD/structures/use_case/use_case_dia.cpp \
    $$PWD/translator/arrow.cpp \
    $$PWD/translator/complete_translator.cpp \
    $$PWD/translator/dia_direct_converter.cpp \
    $$PWD/translator/direct_translator.cpp \
    $$PWD/translator/lex_analyzer.cpp \
    $$PWD/translator/token.cpp \
    $$PWD/translator/translator.cpp \
    $$PWD/translator/use_case/uc_dia_direct_converter.cpp

HEADERS += \
    $$PWD/common/errors/any_error.h \
    $$PWD/common/errors/bldr/bldr_error.h \
    $$PWD/common/errors/bldr/node_after_adder.h \
    $$PWD/common/errors/bldr/not_found_node.h \
    $$PWD/common/errors/bldr/unknown_edge_type.h \
    $$PWD/common/errors/bldr/unknown_node_type.h \
    $$PWD/common/errors/err_text_creator.h \
    $$PWD/common/errors/bldr/invalid_edge.h \
    $$PWD/common/errors/bldr/invalid_node.h \
    $$PWD/common/errors/bldr/null_edge.h \
    $$PWD/common/errors/bldr/null_node.h \
    $$PWD/common/errors/bldr/repeating_edge.h \
    $$PWD/common/errors/bldr/repeating_node.h \
    $$PWD/common/errors/bldr/unsuitable_edge.h \
    $$PWD/common/errors/not_implemented.h \
    $$PWD/common/errors/tltr/lexer_error.h \
    $$PWD/common/errors/tltr/tltr_error.h \
    $$PWD/common/errors/tltr/unknown_directive.h \
    $$PWD/common/generator/igenerator.h \
    $$PWD/common/generator/order_generator.h \
    $$PWD/common/ijson.h \
    $$PWD/../../dependency/nlohmann/json.hpp \
    $$PWD/common/ivalid.h \
    $$PWD/common/utils/conv_utils.h \
    $$PWD/common/utils/puml_utils.h \
    $$PWD/common/utils/string_utils.h \
    $$PWD/structures/class/class_dia.h \
    $$PWD/structures/class/class_ptrs.h \
    $$PWD/structures/description.h \
    $$PWD/structures/format.h \
    $$PWD/structures/layout.h \
    $$PWD/structures/layout_flow/layout_flow_dia.h \
    $$PWD/structures/layout_flow/lw_ptrs.h \
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
    $$PWD/translator/arrow.h \
    $$PWD/translator/complete_translator.h \
    $$PWD/translator/dia_direct_converter.h \
    $$PWD/translator/direct_translator.h \
    $$PWD/translator/lex_analyzer.h \
    $$PWD/translator/token.h \
    $$PWD/translator/translator.h \
    $$PWD/translator/use_case/uc_dia_direct_converter.h \
    $$PWD/translator/use_case/uc_dia_token_tag.h

INCLUDEPATH += \
    ../../dependency/ \
    ../module/structures/ \
    ../module/common/ \
    ../module/
