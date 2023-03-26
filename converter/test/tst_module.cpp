#include <sstream>
#include <iostream>

#include <QtTest>
#include <QDebug>

#include "tst_module.h"

#include "use_case/uc_node.h"
#include "use_case/uc_edge.h"
#include "use_case/use_case_dia.h"

#include "translator/use_case/uc_dia_direct_converter.h"

#include "errors/bldr/null_node.h"
#include "errors/bldr/null_edge.h"
#include "errors/bldr/invalid_edge.h"
#include "errors/bldr/unsuitable_edge.h"
#include "errors/bldr/repeating_node.h"
#include "errors/bldr/repeating_edge.h"

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
        { UC_edge::Field::type, UC_edge::ASSOCIATION }, // static_cast<uint32_t>?
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

void Module::test_UC_edge_is_valid_err()
{
    // TODO:
}

void Module::test_UC_edge_is_valid_okk()
{
    // TODO:
}

// UC_node
// -----------------------------------------------------------------------

void Module::test_UC_node_Builder_Builder_err()
{
    using namespace lenv;
    UC_node_sp node{ nullptr };
    QVERIFY_THROWS_EXCEPTION(
                Null_node,
                UC_node::Builder node_b(nullptr, "Actor")
                );
}

void Module::test_UC_node_Builder_Builder_okk()
{
    using namespace lenv;
    UC_node::Builder node_b("Actor");
    node_b.name("Actor")
            .type(UC_node::ACTOR)
            .rob_dia(nullptr)
            .seq_dia(nullptr);

    UC_node_sp node{ nullptr };
    QVERIFY_THROWS_NO_EXCEPTION(node = node_b.build_ptr()); // always OK!
}

void Module::test_UC_node_Builder_Builder_okk1()
{
    using namespace lenv;
    UC_node::Builder node_b("UseCase");
    node_b.name("UseCase")
            .type(UC_node::USE_CASE)
            .rob_dia(nullptr)
            .seq_dia(nullptr);

    UC_node_sp node{ nullptr };
    QVERIFY_THROWS_NO_EXCEPTION(node = node_b.build_ptr());
}

void Module::test_UC_node_Builder_Builder_okk2()
{
    using namespace lenv;
    UC_node::Builder node_b("Actor");
    node_b.name("Actor")
            .type(UC_node::ACTOR)
            .rob_dia(nullptr)
            .seq_dia(nullptr);

    UC_node_sp node{ nullptr };
    QVERIFY_THROWS_NO_EXCEPTION(
                node = node_b.build_ptr()
            );

    UC_node::Builder node_b_for_clone(node, "Actor2");
    node_b_for_clone.name("Actor2")
            .type(UC_node::ACTOR)
            .rob_dia(nullptr)
            .seq_dia(nullptr);

    UC_node_sp cloned_node{ nullptr };
    QVERIFY_THROWS_NO_EXCEPTION(
                cloned_node = node_b_for_clone.build_ptr()
            );

    QCOMPARE_NE(node->id(), cloned_node->id());
}

// -----------------------------------------------------------------------

void Module::test_UC_node_Adder_add_inn_edge_err()
{
    using namespace lenv;
    UC_node::Builder beg_uc_node_b("UC1");
    auto beg_uc_node{ beg_uc_node_b.name("Student")
                .type(UC_node::ACTOR)
                .rob_dia(nullptr)
                .seq_dia(nullptr)
                .build_ptr() };

    UC_node::Adder beg_uc_node_a{ beg_uc_node };
    QVERIFY_THROWS_EXCEPTION(Null_edge, beg_uc_node_a.add_out_edge(nullptr));
    QVERIFY_THROWS_EXCEPTION(Null_edge, beg_uc_node_a.add_inn_edge(nullptr));
}

void Module::test_UC_node_Adder_add_inn_edge_err1()
{
    using namespace lenv;
    UC_node::Builder beg_uc_node_b("Student");
    auto beg_uc_node{ beg_uc_node_b.name("Student")
                .type(UC_node::ACTOR)
                .rob_dia(nullptr)
                .seq_dia(nullptr)
                .build_ptr() };

    // only for building edge!
    UC_node::Builder end_uc_node_b("Registration");
    auto end_uc_node{ end_uc_node_b.name("Registration")
                .type(UC_node::USE_CASE)
                .rob_dia(nullptr)
                .seq_dia(nullptr)
                .build_ptr() };

    UC_edge::Builder uc_edge_b("1");
    auto uc_edge = uc_edge_b.type(UC_edge::ASSOCIATION)
            .beg(beg_uc_node)
            .end(end_uc_node)
            .build_ptr();

    beg_uc_node.reset(); // clear node - will result in an error.

    // ***

    UC_node::Adder beg_uc_node_a{ end_uc_node };
    QVERIFY_THROWS_EXCEPTION(Null_node, beg_uc_node_a.add_inn_edge(uc_edge));
}

void Module::test_UC_node_Adder_add_inn_edge_err2()
{
    using namespace lenv;
    UC_node::Builder beg_uc_node_b("Student");
    auto beg_uc_node{ beg_uc_node_b.name("Student")
                .type(UC_node::ACTOR)
                .rob_dia(nullptr)
                .seq_dia(nullptr)
                .build_ptr() };

    // only for building edge!
    UC_node::Builder end_uc_node_b("Registration");
    auto end_uc_node{ end_uc_node_b.name("Registration")
                .type(UC_node::USE_CASE)
                .rob_dia(nullptr)
                .seq_dia(nullptr)
                .build_ptr() };

    UC_edge::Builder uc_edge_b("1");
    auto uc_edge = uc_edge_b.type(UC_edge::ASSOCIATION)
            .beg(beg_uc_node)
            .end(end_uc_node)
            .build_ptr();

    // ***

    UC_node::Builder uc_node_b("User");
    auto uc_node{ uc_node_b.name("User")
                .type(UC_node::ACTOR)
                .rob_dia(nullptr)
                .seq_dia(nullptr)
                .build_ptr() };

    // ***

    {
        UC_node::Adder uc_node_a{ uc_node };
        QVERIFY_THROWS_EXCEPTION(Unsuitable_edge, uc_node_a.add_inn_edge(uc_edge));
        QVERIFY_THROWS_EXCEPTION(Unsuitable_edge, uc_node_a.add_out_edge(uc_edge));
    }
}

void Module::test_UC_node_Adder_add_inn_edge_err3()
{
    using namespace lenv;
    UC_node::Builder beg_uc_node_b("Student");
    auto beg_uc_node{ beg_uc_node_b.name("Student")
                .type(UC_node::ACTOR)
                .rob_dia(nullptr)
                .seq_dia(nullptr)
                .build_ptr() };

    // only for building edge!
    UC_node::Builder end_uc_node_b("Reg");
    auto end_uc_node{ end_uc_node_b.name("Registration")
                .type(UC_node::USE_CASE)
                .rob_dia(nullptr)
                .seq_dia(nullptr)
                .build_ptr() };

    UC_edge::Builder uc_edge_b("1");
    auto uc_edge = uc_edge_b.type(UC_edge::ASSOCIATION)
            .beg(beg_uc_node)
            .end(end_uc_node)
            .build_ptr();

    // ***

    UC_node::Adder end_uc_node_a{ end_uc_node };
    QVERIFY_THROWS_NO_EXCEPTION(end_uc_node_a.add_inn_edge(uc_edge));
    QVERIFY_THROWS_EXCEPTION(Repeating_edge, end_uc_node_a.add_inn_edge(uc_edge));
}

void Module::test_UC_node_Adder_add_inn_edge_okk()
{
    using namespace lenv;
    UC_node::Builder uc_node_b("Student");
    auto uc_node{ uc_node_b.name("Student")
                .type(UC_node::ACTOR)
                .rob_dia(nullptr)
                .seq_dia(nullptr)
                .build_ptr() };

    UC_edge::Builder uc_edge_b("1");
    auto uc_edge = uc_edge_b.type(UC_edge::ASSOCIATION)
            .beg(uc_node)
            .end(uc_node)
            .build_ptr(); // a loop!

    // ***

    UC_node::Adder uc_node_a{ uc_node };
    QVERIFY_THROWS_NO_EXCEPTION(uc_node_a.add_inn_edge(uc_edge));
    QVERIFY_THROWS_NO_EXCEPTION(uc_node_a.add_out_edge(uc_edge));
}

void Module::test_UC_node_Adder_add_out_edge_err()
{

}

void Module::test_UC_node_Adder_add_out_edge_okk()
{

}

// -----------------------------------------------------------------------

void Module::test_UC_node_to_whole_json_err()
{
    using namespace lenv;
    UC_node::Builder beg_uc_node_b("Student");
    auto beg_uc_node{ beg_uc_node_b.name("Student")
                .type(UC_node::ACTOR)
                .rob_dia(nullptr)
                .seq_dia(nullptr)
                .build_ptr() };

    // only for building edge!
    UC_node::Builder end_uc_node_b("Registration");
    auto end_uc_node{ end_uc_node_b.name("Registration")
                .type(UC_node::USE_CASE)
                .rob_dia(nullptr)
                .seq_dia(nullptr)
                .build_ptr() };

    // scope for edge!
    {
        UC_edge::Builder uc_edge_b("1");
        auto uc_edge = uc_edge_b.type(UC_edge::ASSOCIATION)
                .beg(beg_uc_node)
                .end(end_uc_node)
                .build_ptr();

        UC_node::Adder beg_uc_node_a{ beg_uc_node };
        QVERIFY_THROWS_NO_EXCEPTION(beg_uc_node_a.add_out_edge(uc_edge));
    }

    // ***

    nlohmann::json actual;
    QVERIFY_THROWS_EXCEPTION(
                Null_edge,
                actual = beg_uc_node->to_whole_json()
            );
}

void Module::test_UC_node_to_whole_json_okk()
{
    using namespace lenv;
    UC_node::Builder node_b("Actor_Id");
    node_b.name("Actor_name")
            .type(UC_node::ACTOR)
            .rob_dia(nullptr)
            .seq_dia(nullptr);

    UC_node_sp node;
    QVERIFY_THROWS_NO_EXCEPTION(node = node_b.build_ptr());

    // ***

    nlohmann::json actual;
    QVERIFY_THROWS_NO_EXCEPTION(actual = node->to_whole_json());

    const nlohmann::json expected = {
        { UC_node::Field::id, "Actor_Id" },
        { UC_node::Field::name, "Actor_name" },
        { UC_node::Field::type, static_cast<uint32_t>(UC_node::ACTOR) },
        { UC_node::Field::inn_edges, nlohmann::json::array() },
        { UC_node::Field::out_edges, nlohmann::json::array() },
        { UC_node::Field::rob_dia, nullptr },
        { UC_node::Field::seq_dia, nullptr },
    };

    QCOMPARE_EQ(actual.is_object(), true);
    QCOMPARE_EQ(expected.is_object(), true);
    QCOMPARE_EQ(actual == expected, true);
}

void Module::test_UC_node_to_whole_json_okk1()
{
    using namespace lenv;
    UC_node::Builder beg_uc_node_b("Student");
    auto beg_uc_node{ beg_uc_node_b.name("Student")
                .type(UC_node::ACTOR)
                .rob_dia(nullptr)
                .seq_dia(nullptr)
                .build_ptr() };

    // only for building edge!
    UC_node::Builder end_uc_node_b("Registration");
    auto end_uc_node{ end_uc_node_b.name("Registration")
                .type(UC_node::USE_CASE)
                .rob_dia(nullptr)
                .seq_dia(nullptr)
                .build_ptr() };

    UC_edge::Builder uc_edge_b("1");
    auto uc_edge = uc_edge_b.type(UC_edge::ASSOCIATION)
            .beg(beg_uc_node)
            .end(end_uc_node)
            .build_ptr();

    // ***

    UC_node::Adder beg_uc_node_a{ beg_uc_node };
    QVERIFY_THROWS_NO_EXCEPTION(beg_uc_node_a.add_out_edge(uc_edge));

    UC_node::Adder end_uc_node_a{ end_uc_node };
    QVERIFY_THROWS_NO_EXCEPTION(end_uc_node_a.add_inn_edge(uc_edge));

    // ***

    nlohmann::json actual;
    QVERIFY_THROWS_NO_EXCEPTION(actual = beg_uc_node->to_whole_json());

    const nlohmann::json expected = {
        { UC_node::Field::id, "Student" },
        { UC_node::Field::name, "Student" },
        { UC_node::Field::type, UC_node::ACTOR },
        { UC_node::Field::inn_edges, nlohmann::json::array() },
        { UC_node::Field::out_edges, { { { UC_edge::Field::id, "1" } } } },
        { UC_node::Field::rob_dia, nullptr },
        { UC_node::Field::seq_dia, nullptr },
    };

    QCOMPARE_EQ(actual.is_object(), true);
    QCOMPARE_EQ(expected.is_object(), true);
    QCOMPARE_EQ(actual == expected, true);
}

void Module::test_UC_node_to_whole_json_okk2()
{
    using namespace lenv;
    UC_node::Builder beg_uc_node_b("Student");
    auto beg_uc_node{ beg_uc_node_b.name("Student")
                .type(UC_node::ACTOR)
                .rob_dia(nullptr)
                .seq_dia(nullptr)
                .build_ptr() };

    // only for building edge!
    UC_node::Builder end_uc_node_b("Reg");
    auto end_uc_node{ end_uc_node_b.name("Registration")
                .type(UC_node::USE_CASE)
                .rob_dia(nullptr)
                .seq_dia(nullptr)
                .build_ptr() };

    UC_edge::Builder uc_edge_b("1");
    auto uc_edge = uc_edge_b.type(UC_edge::ASSOCIATION)
            .beg(beg_uc_node)
            .end(end_uc_node)
            .build_ptr();

    // ***

    UC_node::Adder end_uc_node_a{ end_uc_node };
    QVERIFY_THROWS_NO_EXCEPTION(end_uc_node_a.add_inn_edge(uc_edge));

    UC_node::Adder beg_uc_node_a{ beg_uc_node };
    QVERIFY_THROWS_NO_EXCEPTION(beg_uc_node_a.add_out_edge(uc_edge));

    // ***

    nlohmann::json actual;
    QVERIFY_THROWS_NO_EXCEPTION(actual = end_uc_node->to_whole_json());

    const nlohmann::json expected = {
        { UC_node::Field::id, "Reg" },
        { UC_node::Field::name, "Registration" },
        { UC_node::Field::type, UC_node::USE_CASE },
        { UC_node::Field::inn_edges, { { { UC_edge::Field::id, "1" } } } },
        { UC_node::Field::out_edges, nlohmann::json::array() },
        { UC_node::Field::rob_dia, nullptr },
        { UC_node::Field::seq_dia, nullptr },
    };

    QCOMPARE_EQ(actual.is_object(), true);
    QCOMPARE_EQ(expected.is_object(), true);
    QCOMPARE_EQ(actual == expected, true);
}

void Module::test_UC_node_to_whole_json_okk3()
{
    using namespace lenv;
    UC_node::Builder uc_node_b("Student");
    auto uc_node{ uc_node_b.name("Student")
                .type(UC_node::ACTOR)
                .rob_dia(nullptr)
                .seq_dia(nullptr)
                .build_ptr() };

    UC_edge::Builder uc_edge_b("1");
    auto uc_edge = uc_edge_b.type(UC_edge::ASSOCIATION)
            .beg(uc_node)
            .end(uc_node)
            .build_ptr(); // a loop!

    // ***

    UC_node::Adder uc_node_a{ uc_node };
    QVERIFY_THROWS_NO_EXCEPTION(uc_node_a.add_inn_edge(uc_edge));
    QVERIFY_THROWS_NO_EXCEPTION(uc_node_a.add_out_edge(uc_edge));

    // ***

    nlohmann::json actual;
    QVERIFY_THROWS_NO_EXCEPTION(actual = uc_node->to_whole_json());

    const nlohmann::json expected = {
        { UC_node::Field::id, "Student" },
        { UC_node::Field::name, "Student" },
        { UC_node::Field::type, UC_node::ACTOR },
        { UC_node::Field::inn_edges, { { { UC_edge::Field::id, "1" } } } },
        { UC_node::Field::out_edges, { { { UC_edge::Field::id, "1" } } } },
        { UC_node::Field::rob_dia, nullptr },
        { UC_node::Field::seq_dia, nullptr },
    };

    QCOMPARE_EQ(actual.is_object(), true);
    QCOMPARE_EQ(expected.is_object(), true);
    QCOMPARE_EQ(actual == expected, true);
}

void Module::test_UC_node_to_short_json_okk()
{
    using namespace lenv;
    UC_node::Builder node_b("Actor_Id");
    node_b.name("Actor_name")
            .type(UC_node::ACTOR)
            .rob_dia(nullptr)
            .seq_dia(nullptr);

    UC_node_sp node;
    QVERIFY_THROWS_NO_EXCEPTION(node = node_b.build_ptr());

    // ***

    nlohmann::json actual;
    QVERIFY_THROWS_NO_EXCEPTION(actual = node->to_short_json());

    const nlohmann::json expected = { { UC_node::Field::id, "Actor_Id" }, };

    QCOMPARE_EQ(actual.is_object(), true);
    QCOMPARE_EQ(expected.is_object(), true);
    QCOMPARE_EQ(actual == expected, true);
}

// -----------------------------------------------------------------------

void Module::test_UC_node_is_valid_err()
{
    // TODO:
}

void Module::test_UC_node_is_valid_okk()
{
    // TODO:
}

// Use_Case_dia
// -----------------------------------------------------------------------

void Module::test_Use_Case_dia_to_whole_json_err()
{

}

void Module::test_Use_Case_dia_to_whole_json_okk()
{
    using namespace lenv;
    const Use_Case_dia::Builder uc_dia_b;
    auto uc_dia = uc_dia_b.build_ptr();

    nlohmann::json actual;
    QVERIFY_THROWS_NO_EXCEPTION(actual = uc_dia->to_whole_json());

    const nlohmann::json expected = {
        { Use_Case_dia::Field::id, Use_Case_dia::id },
        { Use_Case_dia::Field::nodes, nlohmann::json::array() },
        { Use_Case_dia::Field::edges, nlohmann::json::array() },
    };

    QCOMPARE_EQ(actual.is_object(), true);
    QCOMPARE_EQ(expected.is_object(), true);
    QCOMPARE_EQ(actual == expected, true);
}

void Module::test_Use_Case_dia_to_whole_json_okk1()
{
    using namespace lenv;
    const Use_Case_dia::Builder uc_dia_b;
    auto uc_dia = uc_dia_b.build_ptr();

    // ***

    UC_node::Builder uc_node_b("Student");
    auto uc_node{ uc_node_b.name("Student")
                .type(UC_node::ACTOR)
                .rob_dia(nullptr)
                .seq_dia(nullptr)
                .build_ptr() };

    UC_edge::Builder uc_edge_b("1");
    auto uc_edge = uc_edge_b.type(UC_edge::ASSOCIATION)
            .beg(uc_node)
            .end(uc_node)
            .build_ptr(); // a loop!

    // ***

    QVERIFY_THROWS_NO_EXCEPTION(uc_dia->add_node_bfore_adder(uc_node));
    QVERIFY_THROWS_NO_EXCEPTION(uc_dia->add_edge(uc_edge, "Student", "Student"));

    // ***

    nlohmann::json actual;
    QVERIFY_THROWS_NO_EXCEPTION(actual = uc_dia->to_whole_json());

    const nlohmann::json expected = {
        { Use_Case_dia::Field::id, Use_Case_dia::id },
        { Use_Case_dia::Field::nodes, { uc_node->to_whole_json() } },
        { Use_Case_dia::Field::edges, { uc_edge->to_whole_json() } },
    };

    QCOMPARE_EQ(actual[Use_Case_dia::Field::nodes].is_array(), true);
    QCOMPARE_EQ(actual[Use_Case_dia::Field::edges].is_array(), true);

    QCOMPARE_EQ(actual.is_object(), true);
    QCOMPARE_EQ(expected.is_object(), true);
    QCOMPARE_EQ(actual == expected, true);
}

void Module::test_Use_Case_dia_to_whole_json_okk2()
{
    // TODO:
}

// String_utils
// -----------------------------------------------------------------------

void Module::test_String_utils_start_with_data()
{

}

void Module::test_String_utils_start_with()
{

}

void Module::test_String_utils_stop_with_data()
{

}

void Module::test_String_utils_stop_with()
{

}

// Puml_utils
// -----------------------------------------------------------------------


// Direct_translator
// -----------------------------------------------------------------------

void Module::test_Direct_translator_convert_uc_dia_okks()
{

}

QTEST_APPLESS_MAIN(Module)
