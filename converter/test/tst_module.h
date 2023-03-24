#ifndef TST_MODULE_H
#define TST_MODULE_H

#include <QObject>

#include "use_case/uc_ptrs.h"

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
    void test_UC_edge_to_whole_json_err();
    void test_UC_edge_to_whole_json_okk();

    void test_UC_edge_to_short_json_okk();
    void test_UC_edge_to_short_json_okk1();

    // UC_node
private slots:

private slots:

    void test_convert_uc_dia_data();
    void test_convert_uc_dia();

};

#endif // TST_MODULE_H
