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

    // UseCaseGraph
private slots:
    void test_UseCaseGraph_read_okk();
    void test_UseCaseGraph_read_okk1();
    void test_UseCaseGraph_read_okk2();
    void test_UseCaseGraph_read_okk3();
    void test_UseCaseGraph_read_okk4();
    void test_UseCaseGraph_read_okk5();
    void test_UseCaseGraph_read_okks();

private slots:
    void test_UseCaseGraph_read_puml_data();
    void test_UseCaseGraph_read_puml();

};

#endif // TST_MODULE_H
