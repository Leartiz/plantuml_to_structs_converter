#include <QtTest>
#include <QDebug>

#include "tst_module.h"

#include "use_case/uc_node.h"
#include "use_case/uc_edge.h"
#include "errors/null_node.h"
#include "errors/null_edge.h"

Module::Module() {}
Module::~Module() {}

void Module::initTestCase() {}
void Module::cleanupTestCase() {}

// UC_edge
// -----------------------------------------------------------------------

void Module::test_UC_edge_Builder_beg_err()
{
    using namespace lenv;
    try {
        UC_edge::Builder b("");
        b.type(UC_edge::ASSOCIATION)
                .beg(nullptr);
    }
    catch(const Null_node& ne) {
        QCOMPARE_EQ(true, true); // OK!
    }
    catch(...) {
        qWarning() << "Expected exception: Null_node";
        QCOMPARE_EQ(true, false);
    }
}

void Module::test_UC_edge_Builder_beg_err1()
{
    using namespace lenv;
    UC_edge::Builder b(""); b.type(UC_edge::ASSOCIATION);
    QVERIFY_THROWS_EXCEPTION(Null_node, b.beg(nullptr));
}

void Module::test_UC_edge_Builder_beg_okk()
{
    using namespace lenv;
    UC_node::Builder beg_node_b("Actor");
    const auto beg_node{ beg_node_b.name("Actor")
                .type(UC_node::ACTOR)
                .build_ptr() }; // next will be a test!

    UC_edge::Builder b(""); b.type(UC_edge::ASSOCIATION);
    QVERIFY_THROWS_NO_EXCEPTION(b.beg(beg_node));
}

// -----------------------------------------------------------------------

void Module::test_convert_uc_dia_data()
{

}

void Module::test_convert_uc_dia()
{

}

QTEST_APPLESS_MAIN(Module)
