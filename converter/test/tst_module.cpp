#include <iostream>

#include <QtTest>
#include <QDebug>

#include "tst_module.h"

#include "use_case/uc_node.h"
#include "use_case/uc_edge.h"
#include "errors/bldr/null_node.h"
#include "errors/bldr/null_edge.h"
#include "errors/bldr/invalid_edge.h"

#include "nlohmann/json.hpp"

Module::Module() {}
Module::~Module() {}

// -----------------------------------------------------------------------

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

void Module::test_UC_edge_Builder_end_err()
{
    using namespace lenv;
    UC_edge::Builder b(""); b.type(UC_edge::GENERALIZATION);
    QVERIFY_THROWS_EXCEPTION(Null_node, b.end(nullptr));
}

void Module::test_UC_edge_Builder_end_okk()
{
    using namespace lenv;
    UC_node::Builder end_node_b("UseCase");
    const auto end_node{ end_node_b.name("UseCase")
                .type(UC_node::USE_CASE)
                .build_ptr() };

    UC_edge::Builder b(""); b.type(UC_edge::GENERALIZATION);
    QVERIFY_THROWS_NO_EXCEPTION(b.end(end_node));
}

// -----------------------------------------------------------------------

void Module::test_UC_edge_Builder_build_ptr_err()
{
    using namespace lenv;
    UC_edge::Builder edge_b("Association");
    edge_b.type(UC_edge::ASSOCIATION);
    QVERIFY_THROWS_EXCEPTION(Invalid_edge, edge_b.build_ptr());
}

void Module::test_UC_edge_Builder_build_ptr_okk()
{
    using namespace lenv;
    UC_edge::Builder edge_b("Association");
    edge_b.type(UC_edge::ASSOCIATION);

    UC_node::Builder beg_node_b("Actor");
    /* depends on the lifetime of this object! */
    const auto beg_node{ beg_node_b.name("Actor")
                .type(UC_node::ACTOR).build_ptr() };

    UC_node::Builder end_node_b("UseCase");
    const auto end_node{ end_node_b.name("UseCase")
                .type(UC_node::USE_CASE).build_ptr() };

    QVERIFY_THROWS_NO_EXCEPTION(edge_b.beg(beg_node));
    QVERIFY_THROWS_NO_EXCEPTION(edge_b.end(end_node));
    QVERIFY_THROWS_NO_EXCEPTION(edge_b.build_ptr());
}

void Module::test_UC_edge_Builder_complex()
{
    using namespace lenv;
    UC_edge::Builder edge_b("Association");
    edge_b.type(UC_edge::ASSOCIATION);

    QVERIFY_THROWS_EXCEPTION(Null_node, edge_b.beg(nullptr));
    QVERIFY_THROWS_EXCEPTION(Null_node, edge_b.end(nullptr));
    QVERIFY_THROWS_EXCEPTION(Invalid_edge, edge_b.build_ptr());

    UC_node::Builder beg_node_b("Actor");
    const auto beg_node{ beg_node_b.name("Actor")
                .type(UC_node::ACTOR).build_ptr() };

    QVERIFY_THROWS_NO_EXCEPTION(edge_b.beg(beg_node));
    QVERIFY_THROWS_EXCEPTION(Invalid_edge, edge_b.build_ptr());

    UC_node::Builder end_node_b("UseCase");
    const auto end_node{ end_node_b.name("UseCase")
                .type(UC_node::USE_CASE).build_ptr() };

    QVERIFY_THROWS_NO_EXCEPTION(edge_b.end(end_node));
    QVERIFY_THROWS_NO_EXCEPTION(edge_b.build_ptr());
}

// -----------------------------------------------------------------------

void Module::test_UC_edge_to_whole_json_err()
{
    using namespace lenv;

    UC_edge::Builder edge_b("Association");
    edge_b.type(UC_edge::ASSOCIATION);

    UC_edge_sp edge_sp; /* take out of space! */
    {
        UC_node::Builder beg_node_b("Actor");
        const auto beg_node{ beg_node_b.type(UC_node::ACTOR).build_ptr() };
        UC_node::Builder end_node_b("UseCase");
        const auto end_node{ end_node_b.type(UC_node::USE_CASE).build_ptr() };

        // ***

        QVERIFY_THROWS_NO_EXCEPTION(edge_b.beg(beg_node).end(end_node));
        QVERIFY_THROWS_NO_EXCEPTION(edge_sp = edge_b.build_ptr());
        QCOMPARE_EQ(edge_sp->is_valid(), true);
    }

    QCOMPARE_EQ(edge_sp->is_valid(), false);
    QVERIFY_THROWS_EXCEPTION(Null_node, edge_sp->to_whole_json());
}

void Module::test_UC_edge_to_whole_json_okk()
{
    using namespace lenv;

    UC_edge::Builder edge_b("Association");
    edge_b.type(UC_edge::ASSOCIATION);

    UC_node::Builder beg_node_b("Actor");
    const auto beg_node{ beg_node_b.type(UC_node::ACTOR).build_ptr() };
    UC_node::Builder end_node_b("UseCase");
    const auto end_node{ end_node_b.type(UC_node::USE_CASE).build_ptr() };

    // ***

    UC_edge_sp edge_sp;
    QVERIFY_THROWS_NO_EXCEPTION(edge_b.beg(beg_node).end(end_node));
    QVERIFY_THROWS_NO_EXCEPTION(edge_sp = edge_b.build_ptr());
    QCOMPARE_EQ(edge_sp->is_valid(), true);

    // ***

    const nlohmann::json actual = edge_sp->to_whole_json();
    const nlohmann::json expected = {
        { UC_edge::Field::type, 0 }, // static_cast<uint32_t>?
        { UC_edge::Field::id, "Association" },
        { UC_edge::Field::beg, { { UC_node::Field::id, "Actor" } } },
        { UC_edge::Field::end, { { UC_node::Field::id, "UseCase" } } },
    };

    QCOMPARE_EQ(actual.is_object(), true);
    QCOMPARE_EQ(expected.is_object(), true);
    QCOMPARE_EQ(actual == expected, true);
}

// -----------------------------------------------------------------------

void Module::test_UC_edge_to_short_json_okk()
{
    using namespace lenv;

    UC_edge::Builder edge_b("Association");
    edge_b.type(UC_edge::ASSOCIATION);

    UC_edge_sp edge_sp; /* take out of scope! */
    {
        UC_node::Builder beg_node_b("Actor");
        const auto beg_node{ beg_node_b.type(UC_node::ACTOR).build_ptr() };
        UC_node::Builder end_node_b("UseCase");
        const auto end_node{ end_node_b.type(UC_node::USE_CASE).build_ptr() };

        // ***

        QVERIFY_THROWS_NO_EXCEPTION(edge_b.beg(beg_node).end(end_node));
        QVERIFY_THROWS_NO_EXCEPTION(edge_sp = edge_b.build_ptr());
        QCOMPARE_EQ(edge_sp->is_valid(), true);
    }
    /* but short JSON can be obtained! */
    QCOMPARE_EQ(edge_sp->is_valid(), false);

    // ***

    nlohmann::json actual;
    QVERIFY_THROWS_NO_EXCEPTION(actual = edge_sp->to_short_json());
    const nlohmann::json expected = { { UC_edge::Field::id, "Association" }, };

    QCOMPARE_EQ(actual.is_object(), true);
    QCOMPARE_EQ(expected.is_object(), true);
    QCOMPARE_EQ(actual == expected, true);
}

void Module::test_UC_edge_to_short_json_okk1()
{
    using namespace lenv;

    UC_edge::Builder edge_b("Association");
    edge_b.type(UC_edge::ASSOCIATION);

    UC_node::Builder beg_node_b("Actor");
    const auto beg_node{ beg_node_b.type(UC_node::ACTOR).build_ptr() };
    UC_node::Builder end_node_b("UseCase");
    const auto end_node{ end_node_b.type(UC_node::USE_CASE).build_ptr() };

    // ***

    UC_edge_sp edge_sp;
    QVERIFY_THROWS_NO_EXCEPTION(edge_b.beg(beg_node).end(end_node));
    QVERIFY_THROWS_NO_EXCEPTION(edge_sp = edge_b.build_ptr());
    QCOMPARE_EQ(edge_sp->is_valid(), true);

    // ***

    nlohmann::json actual;
    QVERIFY_THROWS_NO_EXCEPTION(actual = edge_sp->to_short_json());
    const nlohmann::json expected = { { UC_edge::Field::id, "Association" }, };

    QCOMPARE_EQ(actual.is_object(), true);
    QCOMPARE_EQ(expected.is_object(), true);
    QCOMPARE_EQ(actual == expected, true);
}

// -----------------------------------------------------------------------

void Module::test_convert_uc_dia_data()
{

}

void Module::test_convert_uc_dia()
{

}

QTEST_APPLESS_MAIN(Module)
