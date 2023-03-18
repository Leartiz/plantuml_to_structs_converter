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
    void test_case1();

};

#endif // TST_MODULE_H
