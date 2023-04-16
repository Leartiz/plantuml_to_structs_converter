#ifndef TST_MODULE_H
#define TST_MODULE_H

#include <QtTest>

// add necessary includes here

class Module : public QObject
{
    Q_OBJECT

public:
    Module() {};
    ~Module() {};

private slots:
    void initTestCase() {};
    void cleanupTestCase() {};

    // str_utils
private slots:
    void test_str_utils_to_upper_data();
    void test_str_utils_to_upper();
    void test_str_utils_to_lower_data();
    void test_str_utils_to_lower();

private slots:
    void test_str_utils_start_with_data();
    void test_str_utils_start_with();
    void test_str_utils_stop_with_data();
    void test_str_utils_stop_with();

private slots:
    void test_str_utils_eq_data();
    void test_str_utils_eq();
    void test_str_utils_eq_ref_data();
    void test_str_utils_eq_ref();

private slots:
    void test_str_utils_trim_left_data();
    void test_str_utils_trim_left();
    void test_str_utils_trim_rght_data();
    void test_str_utils_trim_rght();
    void test_str_utils_trim_data();
    void test_str_utils_trim();

private slots:
    void test_str_utils_trim_left_space_data();
    void test_str_utils_trim_left_space();
    void test_str_utils_trim_rght_space_data();
    void test_str_utils_trim_rght_space();
    void test_str_utils_trim_space_data();
    void test_str_utils_trim_space();

    // UseCaseGraph
private slots:
    void test_UseCaseGraph_read_okk();
    void test_UseCaseGraph_read_okk1();
    void test_UseCaseGraph_read_okk2();
    void test_UseCaseGraph_read_okk3();
    void test_UseCaseGraph_read_okk4();
    void test_UseCaseGraph_read_okk5();

    void test_UseCaseGraph_read_err();
    void test_UseCaseGraph_read_err1();

    void test_UseCaseGraph_read_puml_data();
    void test_UseCaseGraph_read_puml();

    // RobustnessGraph
private slots:
    void test_RobustnessGraph_read_okk();
    void test_RobustnessGraph_read_okk1();
    void test_RobustnessGraph_read_okk3();

    void test_RobustnessGraph_read_puml_data();
    void test_RobustnessGraph_read_puml();

    // SequenceGraph
private slots:

    // ClassGraph
private slots:
    void test_ClassGraph_read_okk();

    // LayoutFlowGraph
private slots:

};

#endif // TST_MODULE_H
