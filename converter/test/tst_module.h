#ifndef TST_MODULE_H
#define TST_MODULE_H

#include <QObject>

class Module : public QObject
{
    Q_OBJECT

public:
    Module();
    ~Module();

private slots:
    void initTestCase();
    void cleanupTestCase();

    // UC_edge
private slots:
    void test_UC_edge_Builder_beg_err();
    void test_UC_edge_Builder_beg_err1();
    void test_UC_edge_Builder_beg_okk();

    void test_UC_edge_Builder_end_err();
    void test_UC_edge_Builder_end_okk();

    void test_UC_edge_Builder_build_ptr_err();
    void test_UC_edge_Builder_build_ptr_okk();
    void test_UC_edge_Builder_complex();

private slots:
    void test_UC_edge_type_to_str_complex();
    void test_UC_edge_str_to_type_complex();

private slots:
    void test_UC_edge_to_whole_json_err();
    void test_UC_edge_to_whole_json_okk();

    void test_UC_edge_to_short_json_okk();
    void test_UC_edge_to_short_json_okk1();

private slots:
    void test_UC_edge_is_valid_err();
    void test_UC_edge_is_valid_okk();

    // UC_node
private slots:
    void test_UC_node_Builder_Builder_err();
    void test_UC_node_Builder_Builder_okk();
    void test_UC_node_Builder_Builder_okk1();
    void test_UC_node_Builder_Builder_okk2();

private slots:
    void test_UC_node_type_to_str_complex();
    void test_UC_node_str_to_type_complex();

private slots:
    void test_UC_node_Adder_add_inn_edge_err();
    void test_UC_node_Adder_add_inn_edge_err1();
    void test_UC_node_Adder_add_inn_edge_err2();
    void test_UC_node_Adder_add_inn_edge_err3();
    void test_UC_node_Adder_add_inn_edge_okk();

    void test_UC_node_Adder_add_out_edge_err();
    void test_UC_node_Adder_add_out_edge_okk();

private slots:
    void test_UC_node_to_whole_json_err();
    void test_UC_node_to_whole_json_okk();
    void test_UC_node_to_whole_json_okk1();
    void test_UC_node_to_whole_json_okk2();
    void test_UC_node_to_whole_json_okk3();

    void test_UC_node_to_short_json_okk();

private slots:
    void test_UC_node_is_valid_err();
    void test_UC_node_is_valid_okk();

    // Use_Case_dia
private slots:
    void test_Use_Case_dia_to_whole_json_err();
    void test_Use_Case_dia_to_whole_json_okk();
    void test_Use_Case_dia_to_whole_json_okk1();
    void test_Use_Case_dia_to_whole_json_okk2();

    // String_utils
private slots:
    void test_String_utils_to_upper_data();
    void test_String_utils_to_upper();
    void test_String_utils_to_lower_data();
    void test_String_utils_to_lower();

private slots:
    void test_String_utils_start_with_data();
    void test_String_utils_start_with();
    void test_String_utils_stop_with_data();
    void test_String_utils_stop_with();

private slots:
    void test_String_utils_eq_data();
    void test_String_utils_eq();
    void test_String_utils_eq_ref_data();
    void test_String_utils_eq_ref();

private slots:
    void test_String_trim_left_data();
    void test_String_trim_left();
    void test_String_trim_rght_data();
    void test_String_trim_rght();
    void test_String_trim_data();
    void test_String_trim();

private slots:
    void test_String_trim_left_space_data();
    void test_String_trim_left_space();
    void test_String_trim_rght_space_data();
    void test_String_trim_rght_space();
    void test_String_trim_space_data();
    void test_String_trim_space();

    // Puml_utils
private slots:
    void test_Puml_utils_read_startuml_directive_data();
    void test_Puml_utils_read_startuml_directive();
    void test_Puml_utils_read_enduml_directive_data();
    void test_Puml_utils_read_enduml_directive();

private slots:
    void test_Puml_utils_UC_dia_read_use_case_creation_data();
    void test_Puml_utils_UC_dia_read_use_case_creation();
    void test_Puml_utils_UC_dia_read_actor_creation_data();
    void test_Puml_utils_UC_dia_read_actor_creation();
    void test_Puml_utils_UC_dia_read_connection_creation_data();
    void test_Puml_utils_UC_dia_read_connection_creation();
    void test_Puml_utils_UC_dia_arrow_to_type_data();
    void test_Puml_utils_UC_dia_arrow_to_type();

    // Lex_analyzer
private slots:
    void test_Lex_analyzer_tokenize_okk();
    void test_Lex_analyzer_tokenize_okk1();
    void test_Lex_analyzer_tokenize_okk2();
    void test_Lex_analyzer_tokenize_okk3();
    void test_Lex_analyzer_tokenize_okk4();
    void test_Lex_analyzer_tokenize_okk5();
    void test_Lex_analyzer_tokenize_okk6();
    void test_Lex_analyzer_tokenize_okk7();
    void test_Lex_analyzer_tokenize_okk8();
    void test_Lex_analyzer_tokenize_okk9();
    void test_Lex_analyzer_tokenize_okk10();
    void test_Lex_analyzer_tokenize_okk11();
    void test_Lex_analyzer_tokenize_okk12();
    void test_Lex_analyzer_tokenize_okk13();
    void test_Lex_analyzer_tokenize_okk14();

    void test_Lex_analyzer_tokenize_err();
    void test_Lex_analyzer_tokenize_err1();
    void test_Lex_analyzer_tokenize_err2();
    void test_Lex_analyzer_tokenize_err3();

    // Direct_translator
private slots:
    void test_Direct_translator_convert_uc_dia_data();
    void test_Direct_translator_convert_uc_dia();
};

#endif // TST_MODULE_H

