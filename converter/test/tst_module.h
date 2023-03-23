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

    void test_convert_uc_dia_data();
    void test_convert_uc_dia();

};

#endif // TST_MODULE_H
