#include <sstream>
#include <fstream>
#include <iostream>

#include <QtTest>
#include <QDebug>

#include <QDir>
#include <QFile>

#include "tst_module.h"

#include "nlohmann/json.hpp"

#include "use_case/uc_ptrs.h"
#include "use_case/uc_node.h"
#include "use_case/uc_edge.h"
#include "use_case/use_case_dia.h"

#include "errors/bldr/null_node.h"
#include "errors/bldr/null_edge.h"
#include "errors/bldr/invalid_edge.h"
#include "errors/bldr/unsuitable_edge.h"
#include "errors/bldr/repeating_edge.h"
#include "errors/bldr/unknown_edge_type.h"
#include "errors/bldr/unknown_node_type.h"

#include "utils/string_utils.h"
#include "utils/puml_utils.h"

#include "translator/lex_analyzer.h"
#include "errors/tltr/lexer_error.h"

#include "translator/use_case/uc_dia_direct_converter.h"
#include "translator/direct_translator.h"

// -----------------------------------------------------------------------

#if QT_VERSION <= QT_VERSION_CHECK(6, 4, 0)
    #define QCOMPARE_EQ(lhs, rhs) QCOMPARE(lhs == rhs, true);
    #define QCOMPARE_NE(lhs, rhs) QCOMPARE(lhs == rhs, false);

    #define QVERIFY_THROWS_EXCEPTION(exceptiontype, expression) QVERIFY_EXCEPTION_THROWN(expression, exceptiontype);
    #define QVERIFY_THROWS_NO_EXCEPTION(expression) static_cast<void>(expression);
#endif

// -----------------------------------------------------------------------

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

void Module::test_UC_edge_type_to_str_complex()
{
    using namespace lenv;
    {
        std::string str;
        const UC_edge::Type type{ static_cast<UC_edge::Type>(0) };
        QVERIFY_THROWS_NO_EXCEPTION(str = lenv::UC_edge::type_to_str(type));
        QCOMPARE_EQ(str, "ASSOCIATION");
    }
    {
        std::string str;
        const UC_edge::Type type{ static_cast<UC_edge::Type>(1) };
        QVERIFY_THROWS_NO_EXCEPTION(str = lenv::UC_edge::type_to_str(type));
        QCOMPARE_EQ(str, "GENERALIZATION");
    }
    {
        std::string str;
        const UC_edge::Type type{ static_cast<UC_edge::Type>(4) };
        QVERIFY_THROWS_EXCEPTION(Unknown_edge_type, str = lenv::UC_edge::type_to_str(type));
    }
    {
        std::string str;
        const UC_edge::Type type{ static_cast<UC_edge::Type>(9) };
        QVERIFY_THROWS_EXCEPTION(Unknown_edge_type, str = lenv::UC_edge::type_to_str(type));
    }
    // ...
}

void Module::test_UC_edge_str_to_type_complex()
{
    using namespace lenv;
    {
        UC_edge::Type type{};
        const std::string str{ "AssOCIAtiOn" };
        QVERIFY_THROWS_NO_EXCEPTION(type = lenv::UC_edge::str_to_type(str));
        QCOMPARE_EQ(type, UC_edge::Type::ASSOCIATION);
    }
    {
        UC_edge::Type type{};
        const std::string str{ "ExtEND" };
        QVERIFY_THROWS_NO_EXCEPTION(type = lenv::UC_edge::str_to_type(str));
        QCOMPARE_EQ(type, UC_edge::Type::EXTEND);
    }
    {
        UC_edge::Type type{};
        const std::string str{ "Ext123" };
        QVERIFY_THROWS_EXCEPTION(Unknown_edge_type, type = lenv::UC_edge::str_to_type(str));
        static_cast<void>(type);
    }
    // ...
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

void Module::test_UC_node_type_to_str_complex()
{
    using namespace lenv;
    {
        std::string str;
        const UC_node::Type type{ static_cast<UC_node::Type>(0) };
        QVERIFY_THROWS_NO_EXCEPTION(str = lenv::UC_node::type_to_str(type));
        QCOMPARE_EQ(str, "ACTOR");
    }
    {
        std::string str;
        const UC_node::Type type{ static_cast<UC_node::Type>(1) };
        QVERIFY_THROWS_NO_EXCEPTION(str = lenv::UC_node::type_to_str(type));
        QCOMPARE_EQ(str, "USE_CASE");
    }
    {
        std::string str;
        const UC_node::Type type{ static_cast<UC_node::Type>(2) };
        QVERIFY_THROWS_EXCEPTION(Unknown_node_type, str = lenv::UC_node::type_to_str(type));
    }
    // ...
}

void Module::test_UC_node_str_to_type_complex()
{
    using namespace lenv;
    {
        UC_node::Type type{};
        const std::string str{ "Actor" };
        QVERIFY_THROWS_NO_EXCEPTION(type = lenv::UC_node::str_to_type(str));
        QCOMPARE_EQ(type, UC_node::Type::ACTOR);
    }
    {
        UC_node::Type type{};
        const std::string str{ "use_CASE" };
        QVERIFY_THROWS_NO_EXCEPTION(type = lenv::UC_node::str_to_type(str));
        QCOMPARE_EQ(type, UC_node::Type::USE_CASE);
    }
    {
        UC_node::Type type{};
        const std::string str{ "Ext123" };
        QVERIFY_THROWS_EXCEPTION(Unknown_node_type, type = lenv::UC_node::str_to_type(str));
        static_cast<void>(type);
    }
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
    // TODO:
}

void Module::test_UC_node_Adder_add_out_edge_okk()
{
    // TODO:
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

void Module::test_String_utils_to_upper_data()
{
    QTest::addColumn<std::string>("str");
    QTest::addColumn<std::string>("res");

    {
        const std::string str{ "приВЕТ" };
        const std::string res{ "приВЕТ" };
        QTest::newRow("приВЕТ") << str << res;
    }
    {
        const std::string str{ "useCase" };
        const std::string res{ "USECASE" };
        QTest::newRow("useCase") << str << res;
    }
    {
        const std::string str{ "actor: 🤬🙄😎" };
        const std::string res{ "ACTOR: 🤬🙄😎" };
        QTest::newRow("actor: 🤬🙄😎") << str << res;
    }
    {
        const std::string str{ "приветPrivet阪市立学" };
        const std::string res{ "приветPRIVET阪市立学" };
        QTest::newRow("приветPrivet阪市立学") << str << res;
    }
    {
        const std::string str{ "@startuml" };
        const std::string res{ "@STARTUML" };
        QTest::newRow("@startuml") << str << res;
    }
    // ...
}

void Module::test_String_utils_to_upper()
{
    QFETCH(std::string, str);
    QFETCH(std::string, res);

    auto got{ lenv::str_utils::to_upper(str) };
    QCOMPARE_EQ(got, res);
}

void Module::test_String_utils_to_lower_data()
{
    QTest::addColumn<std::string>("str");
    QTest::addColumn<std::string>("res");

    {
        const std::string str{ "приВЕТ" };
        const std::string res{ "приВЕТ" };
        QTest::newRow("приВЕТ") << str << res;
    }
    {
        const std::string str{ "USECASE" };
        const std::string res{ "usecase" };
        QTest::newRow("USECASE") << str << res;
    }
    {
        const std::string str{ "приветPRIVET阪市立学" };
        const std::string res{ "приветprivet阪市立学" };
        QTest::newRow("приветPRIVET阪市立学") << str << res;
    }
    {
        const std::string str{ "ACtoR: 🤬🙄😎" };
        const std::string res{ "actor: 🤬🙄😎" };
        QTest::newRow("ACtoR: 🤬🙄😎") << str << res;
    }
    {
        const std::string str{ "@STARTUML" };
        const std::string res{ "@startuml" };
        QTest::newRow("@STARTUML") << str << res;
    }
    // ...
}

void Module::test_String_utils_to_lower()
{
    QFETCH(std::string, str);
    QFETCH(std::string, res);

    auto got{ lenv::str_utils::to_lower(str) };
    QCOMPARE_EQ(got, res);
}

// -----------------------------------------------------------------------

void Module::test_String_utils_start_with_data()
{
    QTest::addColumn<std::string>("str");
    QTest::addColumn<std::string>("start");
    QTest::addColumn<bool>("sensitive");
    QTest::addColumn<bool>("res");

    {
        const std::string str{ "@STARTUML" };
        const std::string start{ "@START" };
        const bool sensitive{ true };
        const bool res{ true };
        QTest::newRow("@STARTUML") << str << start << sensitive << res;
    }
    {
        const std::string str{ "   @STARTUML" };
        const std::string start{ "@START" };
        const bool sensitive{ true };
        const bool res{ false };
        QTest::newRow("   @STARTUML") << str << start << sensitive << res;
    }
    {
        const std::string str{ "@STARTUML" };
        const std::string start{ "@start" };
        const bool sensitive{ false };
        const bool res{ true };
        QTest::newRow("@STARTUML") << str << start << sensitive << res;
    }
    {
        const std::string str{ "при 🤬 вет" };
        const std::string start{ "при 🤬" };
        const bool sensitive{ false };
        const bool res{ true };
        QTest::newRow("при 🤬 вет") << str << start << sensitive << res;
    }
    // ...
}

void Module::test_String_utils_start_with()
{
    QFETCH(std::string, str);
    QFETCH(std::string, start);
    QFETCH(bool, sensitive);
    QFETCH(bool, res);

    auto got{ lenv::str_utils::start_with(str, start, sensitive) };
    QCOMPARE_EQ(got, res);
}

void Module::test_String_utils_stop_with_data()
{
    QTest::addColumn<std::string>("str");
    QTest::addColumn<std::string>("stop");
    QTest::addColumn<bool>("sensitive");
    QTest::addColumn<bool>("res");

    {
        const std::string str{ "@STARTUML" };
        const std::string start{ "UML" };
        const bool sensitive{ true };
        const bool res{ true };
        QTest::newRow("@STARTUML") << str << start << sensitive << res;
    }
    {
        const std::string str{ "@STARTUML123" };
        const std::string start{ "tuml123" };
        const bool sensitive{ false };
        const bool res{ true };
        QTest::newRow("@STARTUML123") << str << start << sensitive << res;
    }
    {
        const std::string str{ "@STARTUML123   " };
        const std::string start{ "tuml123" };
        const bool sensitive{ false };
        const bool res{ false };
        QTest::newRow("@STARTUML123   ") << str << start << sensitive << res;
    }
    {
        const std::string str{ "при 🤬 вет" };
        const std::string start{ "🤬 вет" };
        const bool sensitive{ false };
        const bool res{ true };
        QTest::newRow("при 🤬 вет") << str << start << sensitive << res;
    }
    // ...
}

void Module::test_String_utils_stop_with()
{
    QFETCH(std::string, str);
    QFETCH(std::string, stop);
    QFETCH(bool, sensitive);
    QFETCH(bool, res);

    auto got{ lenv::str_utils::stop_with(str, stop, sensitive) };
    QCOMPARE_EQ(got, res);
}

// -----------------------------------------------------------------------

void Module::test_String_utils_eq_data()
{
    QTest::addColumn<std::string>("lhs");
    QTest::addColumn<std::string>("rhs");
    QTest::addColumn<bool>("sensitive");
    QTest::addColumn<bool>("res");

    {
        const std::string lhs{ "@STARTUML" };
        const std::string rhs{ "@startuml" };
        const bool sensitive{ false };
        const bool res{ true };
        QTest::newRow("@STARTUML") << lhs << rhs << sensitive << res;
    }
    {
        const std::string lhs{ "@STARTUML123" };
        const std::string rhs{ "@startuml123" };
        const bool sensitive{ true };
        const bool res{ false };
        QTest::newRow("@STARTUML123") << lhs << rhs << sensitive << res;
    }
    {
        const std::string lhs{ "при 🤬 вет" };
        const std::string rhs{ "при 🤬 вет" };
        const bool sensitive{ false };
        const bool res{ true };
        QTest::newRow("при 🤬 вет") << lhs << rhs << sensitive << res;
    }
    // ...
}

void Module::test_String_utils_eq()
{
    QFETCH(std::string, lhs);
    QFETCH(std::string, rhs);
    QFETCH(bool, sensitive);
    QFETCH(bool, res);

    auto got{ lenv::str_utils::eq(lhs, rhs, sensitive) };
    QCOMPARE_EQ(got, res);
}

void Module::test_String_utils_eq_ref_data()
{
    QTest::addColumn<std::string>("lhs");
    QTest::addColumn<std::string>("rhs");
    QTest::addColumn<bool>("sensitive");
    QTest::addColumn<bool>("res");

    {
        const std::string lhs{ "@STARTUML" };
        const std::string rhs{ "@startuml" };
        const bool sensitive{ false };
        const bool res{ true };
        QTest::newRow("@STARTUML") << lhs << rhs << sensitive << res;
    }
    {
        const std::string lhs{ "@STARTUML123" };
        const std::string rhs{ "@startuml123" };
        const bool sensitive{ true };
        const bool res{ false };
        QTest::newRow("@STARTUML123") << lhs << rhs << sensitive << res;
    }
    {
        const std::string lhs{ "при 🤬 вет" };
        const std::string rhs{ "при 🤬 вет" };
        const bool sensitive{ false };
        const bool res{ true };
        QTest::newRow("при 🤬 вет") << lhs << rhs << sensitive << res;
    }
    {
        const std::string lhs{ "" };
        const std::string rhs{ "при" };
        const bool sensitive{ false };
        const bool res{ false };
        QTest::newRow("[empty]") << lhs << rhs << sensitive << res;
    }
    // ...
}

void Module::test_String_utils_eq_ref()
{
    QFETCH(std::string, lhs);
    QFETCH(std::string, rhs);
    QFETCH(bool, sensitive);
    QFETCH(bool, res);

    auto got{ lenv::str_utils::eq_ref(lhs, rhs, sensitive) };
    QCOMPARE_EQ(got, res);
}

// -----------------------------------------------------------------------

void Module::test_String_trim_left_data()
{
    QTest::addColumn<std::string>("str");
    QTest::addColumn<std::string>("chs");
    QTest::addColumn<std::string>("res");

    {
        const std::string str{ "\"name\"" };
        const std::string chs{ "\"" };
        const std::string res{ "name\"" };
        QTest::newRow("\"name\"") << str << chs << res;
    }
    {
        const std::string str{ "@Startuml name" };
        const std::string chs{ "tS@" };
        const std::string res{ "artuml name" };
        QTest::newRow("@Startuml name") << str << chs << res;
    }
    {
        const std::string str{ "- @ - +-  Start - @ - +-" };
        const std::string chs{ "@+- " };
        const std::string res{ "Start - @ - +-" };
        QTest::newRow("- @ - +-  Start - @ - +-") << str << chs << res;
    }
    // ...
}

void Module::test_String_trim_left()
{
    QFETCH(std::string, str);
    QFETCH(std::string, chs);
    QFETCH(std::string, res);

    auto got{ lenv::str_utils::trim_left(str, chs) };
    QCOMPARE_EQ(got, res);
}

void Module::test_String_trim_rght_data()
{
    QTest::addColumn<std::string>("str");
    QTest::addColumn<std::string>("chs");
    QTest::addColumn<std::string>("res");

    {
        const std::string str{ "\"name\"" };
        const std::string chs{ "\"" };
        const std::string res{ "\"name" };
        QTest::newRow("\"name\"") << str << chs << res;
    }
    {
        const std::string str{ "@Startuml name@St" };
        const std::string chs{ "tS@" };
        const std::string res{ "@Startuml name" };
        QTest::newRow("@Startuml name@St") << str << chs << res;
    }
    {
        const std::string str{ "- @ - +-  Start - @ - +-" };
        const std::string chs{ "@+- " };
        const std::string res{ "- @ - +-  Start" };
        QTest::newRow("- @ - +-  Start - @ - +-") << str << chs << res;
    }
    // ...
}

void Module::test_String_trim_rght()
{
    QFETCH(std::string, str);
    QFETCH(std::string, chs);
    QFETCH(std::string, res);

    auto got{ lenv::str_utils::trim_rght(str, chs) };
    QCOMPARE_EQ(got, res);
}

void Module::test_String_trim_data()
{
    QTest::addColumn<std::string>("str");
    QTest::addColumn<std::string>("chs");
    QTest::addColumn<std::string>("res");

    {
        const std::string str{ "\"name\"" };
        const std::string chs{ "\"" };
        const std::string res{ "name" };
        QTest::newRow("\"name\"") << str << chs << res;
    }
    {
        const std::string str{ "@Startuml name@St" };
        const std::string chs{ "tS@" };
        const std::string res{ "artuml name" };
        QTest::newRow("@Startuml name@St") << str << chs << res;
    }
    {
        const std::string str{ "- @ - +-  Start - @ - +-" };
        const std::string chs{ "@+- " };
        const std::string res{ "Start" };
        QTest::newRow("- @ - +-  Start - @ - +-") << str << chs << res;
    }
    // ...
}

void Module::test_String_trim()
{
    QFETCH(std::string, str);
    QFETCH(std::string, chs);
    QFETCH(std::string, res);

    auto got{ lenv::str_utils::trim(str, chs) };
    QCOMPARE_EQ(got, res);
}

// -----------------------------------------------------------------------

void Module::test_String_trim_left_space_data()
{
    QTest::addColumn<std::string>("str");
    QTest::addColumn<std::string>("res");

    {
        const std::string str{ "    @Startuml name@St" };
        const std::string res{ "@Startuml name@St" };
        QTest::newRow("    @Startuml name@St") << str << res;
    }
    {
        const std::string str{ "\n\t   @St     " };
        const std::string res{ "@St     " };
        QTest::newRow("\\n\\t   @St     ") << str << res;
    }
    // ...
}

void Module::test_String_trim_left_space()
{
    QFETCH(std::string, str);
    QFETCH(std::string, res);

    auto got{ lenv::str_utils::trim_left_space(str) };
    QCOMPARE_EQ(got, res);
}

void Module::test_String_trim_rght_space_data()
{
    QTest::addColumn<std::string>("str");
    QTest::addColumn<std::string>("res");

    {
        const std::string str{ "    @Startuml name@St    " };
        const std::string res{ "    @Startuml name@St" };
        QTest::newRow("    @Startuml name@St    ") << str << res;
    }
    {
        const std::string str{ "\n\t   @St     \n" };
        const std::string res{ "\n\t   @St" };
        QTest::newRow("\\n\\t   @St     \\n") << str << res;
    }
    // ...
}

void Module::test_String_trim_rght_space()
{
    QFETCH(std::string, str);
    QFETCH(std::string, res);

    auto got{ lenv::str_utils::trim_rght_space(str) };
    QCOMPARE_EQ(got, res);
}

void Module::test_String_trim_space_data()
{
    QTest::addColumn<std::string>("str");
    QTest::addColumn<std::string>("res");

    {
        const std::string str{ "    @Startuml name@St    " };
        const std::string res{ "@Startuml name@St" };
        QTest::newRow("    @Startuml name@St    ") << str << res;
    }
    {
        const std::string str{ "\n\t   @St     \n" };
        const std::string res{ "@St" };
        QTest::newRow("\\n\\t   @St     \\n") << str << res;
    }
    // ...
}

void Module::test_String_trim_space()
{
    QFETCH(std::string, str);
    QFETCH(std::string, res);

    auto got{ lenv::str_utils::trim_space(str) };
    QCOMPARE_EQ(got, res);
}

// Puml_utils
// -----------------------------------------------------------------------

void Module::test_Puml_utils_read_startuml_directive_data()
{
    QTest::addColumn<std::string>("line");
    QTest::addColumn<std::string>("exd_out_name");
    QTest::addColumn<bool>("exd_status");

    {
        const std::string line{ "@Startuml" };
        const std::string exd_out_name{ "" };
        const bool exd_status{ true };
        QTest::newRow("@Startuml") << line << exd_out_name << exd_status;
    }
    {
        const std::string line{ "@Startuml inner" };
        const std::string exd_out_name{ "inner" };
        const bool exd_status{ true };
        QTest::newRow("@Startuml inner") << line << exd_out_name << exd_status;
    }
    {
        const std::string line{ "@Sta" };
        const std::string exd_out_name{ "" };
        const bool exd_status{ false };
        QTest::newRow("@Sta") << line << exd_out_name << exd_status;
    }
    {
        const std::string line{ "     @STARTUML      my   dia  " };
        const std::string exd_out_name{ "my   dia" };
        const bool exd_status{ true };
        QTest::newRow("     @STARTUML      my   dia  ")
                << line << exd_out_name << exd_status;
    }
    {
        const std::string line{ "     @STARTUML         " };
        const std::string exd_out_name{ "" };
        const bool exd_status{ true };
        QTest::newRow("     @STARTUML         ")
                << line << exd_out_name << exd_status;
    }
    {
        const std::string line{ "@enduml" };
        const std::string exd_out_name{ "" };
        const bool exd_status{ false };
        QTest::newRow("@enduml")
                << line << exd_out_name << exd_status;
    }
    // ...
}

void Module::test_Puml_utils_read_startuml_directive()
{
    using namespace lenv;
    QFETCH(std::string, line);
    QFETCH(std::string, exd_out_name);
    QFETCH(bool, exd_status);

    std::string got_out_name{};
    QCOMPARE_EQ(wsd_utils::read_startuml_directive(line, got_out_name), exd_status);
    if (exd_status) QCOMPARE_EQ(got_out_name, exd_out_name);
}

void Module::test_Puml_utils_read_enduml_directive_data()
{
    QTest::addColumn<std::string>("line");
    QTest::addColumn<bool>("exd_status");

    {
        const std::string line{ "@Startuml" };
        const bool exd_status{ false };
        QTest::newRow("@Startuml") << line << exd_status;
    }
    {
        const std::string line{ "@enduml         " };
        const bool exd_status{ true };
        QTest::newRow("@enduml         ") << line << exd_status;
    }
    {
        const std::string line{ "@en du ml" };
        const bool exd_status{ false };
        QTest::newRow("@en du ml") << line << exd_status;
    }
    {
        const std::string line{ "     @enduml    " };
        const bool exd_status{ true };
        QTest::newRow("     @enduml    ") << line << exd_status;
    }
    {
        const std::string line{ "kdasaswe" };
        const bool exd_status{ false };
        QTest::newRow("kdasaswe") << line << exd_status;
    }
    // ...
}

void Module::test_Puml_utils_read_enduml_directive()
{
    using namespace lenv;
    QFETCH(std::string, line);
    QFETCH(bool, exd_status);

    const bool got_status{ wsd_utils::read_enduml_directive(line) };
    QCOMPARE_EQ(got_status, exd_status);
}

// -----------------------------------------------------------------------

void Module::test_Puml_utils_UC_dia_read_use_case_creation_data()
{
    using namespace lenv;
    QTest::addColumn<std::string>("line");
    QTest::addColumn<std::string>("exd_name");
    QTest::addColumn<std::string>("exd_id");
    QTest::addColumn<UC_node::Type>("exd_type");
    QTest::addColumn<bool>("exd_status");

    /* true */
    {
        const std::string line{ "usecase Reg" };
        const std::string exd_name{ "Reg" };
        const std::string exd_id{ "Reg" };
        const UC_node::Type exd_type{ UC_node::USE_CASE };
        const bool exd_status{ true };
        QTest::newRow("usecase Reg")
                << line << exd_name << exd_id << exd_type << exd_status;
    }
    {
        const std::string line{ "usecase \"Reg\"" };
        const std::string exd_name{ "Reg" };
        const std::string exd_id{ "Reg" };
        const UC_node::Type exd_type{ UC_node::USE_CASE };
        const bool exd_status{ true };
        QTest::newRow("usecase \"Reg\"")
                << line << exd_name << exd_id << exd_type << exd_status;
    }
    {
        const std::string line{ "usecase \"Registr\" as Reg" };
        const std::string exd_name{ "Registr" };
        const std::string exd_id{ "Reg" };
        const UC_node::Type exd_type{ UC_node::USE_CASE };
        const bool exd_status{ true };
        QTest::newRow("usecase \"Registr\" as Reg")
                << line << exd_name << exd_id << exd_type << exd_status;
    }
    {
        const std::string line{ "usecase \"Reg reg\" as Reg" };
        const std::string exd_name{ "Reg reg" };
        const std::string exd_id{ "Reg" };
        const UC_node::Type exd_type{ UC_node::USE_CASE };
        const bool exd_status{ true };
        QTest::newRow("usecase \"Reg reg\" as Reg")
                << line << exd_name << exd_id << exd_type << exd_status;
    }
    {
        const std::string line{ "usecase :Reg ds:" };
        const std::string exd_name{ "Reg ds" };
        const std::string exd_id{ "Reg ds" };
        const UC_node::Type exd_type{ UC_node::ACTOR };
        const bool exd_status{ true };
        QTest::newRow("usecase :Reg ds:")
                << line << exd_name << exd_id << exd_type << exd_status;
    }
    // *** 5
    {
        const std::string line{ "usecase :Reg ds: as \"Reggg  d\"" };
        const std::string exd_name{ "Reggg  d" };
        const std::string exd_id{ "Reg ds" };
        const UC_node::Type exd_type{ UC_node::ACTOR };
        const bool exd_status{ true };
        QTest::newRow("usecase :Reg ds: as \"Reggg  d\"")
                << line << exd_name << exd_id << exd_type << exd_status;
    }
    {
        const std::string line{ "usecase :Reg ds: as (Reggg  d)" };
        const std::string exd_name{ "Reg ds" };
        const std::string exd_id{ "Reggg  d" };
        const UC_node::Type exd_type{ UC_node::USE_CASE };
        const bool exd_status{ true };
        QTest::newRow("usecase :Reg ds: as (Reggg  d)")
                << line << exd_name << exd_id << exd_type << exd_status;
    }
    {
        const std::string line{ "usecase :Reg ds: as :Reggg  d:" };
        const std::string exd_name{ "Reg ds" };
        const std::string exd_id{ "Reggg  d" };
        const UC_node::Type exd_type{ UC_node::ACTOR };
        const bool exd_status{ true };
        QTest::newRow("usecase :Reg ds: as :Reggg  d:")
                << line << exd_name << exd_id << exd_type << exd_status;
    }
    {
        const std::string line{ "usecase (Reg ds) as :Reggg  d:" };
        const std::string exd_name{ "Reg ds" };
        const std::string exd_id{ "Reggg  d" };
        const UC_node::Type exd_type{ UC_node::USE_CASE };
        const bool exd_status{ true };
        QTest::newRow("usecase (Reg ds) as :Reggg  d:")
                << line << exd_name << exd_id << exd_type << exd_status;
    }
    {
        const std::string line{ "usecase \"Reg ds\" as :Reggg  d:" };
        const std::string exd_name{ "Reg ds" };
        const std::string exd_id{ "Reggg  d" };
        const UC_node::Type exd_type{ UC_node::ACTOR };
        const bool exd_status{ true };
        QTest::newRow("usecase \"Reg ds\" as :Reggg  d:")
                << line << exd_name << exd_id << exd_type << exd_status;
    }
    // *** 10
    {
        const std::string line{ "usecase (Reg ds)" };
        const std::string exd_name{ "Reg ds" };
        const std::string exd_id{ "Reg ds" };
        const UC_node::Type exd_type{ UC_node::USE_CASE };
        const bool exd_status{ true };
        QTest::newRow("usecase (Reg ds)")
                << line << exd_name << exd_id << exd_type << exd_status;
    }
    {
        const std::string line{ "usecase (Reg ds)" };
        const std::string exd_name{ "Reg ds" };
        const std::string exd_id{ "Reg ds" };
        const UC_node::Type exd_type{ UC_node::USE_CASE };
        const bool exd_status{ true };
        QTest::newRow("usecase (Reg ds)")
                << line << exd_name << exd_id << exd_type << exd_status;
    }
    {
        const std::string line{ "usecase \"dasd asd\"" };
        const std::string exd_name{ "dasd asd" };
        const std::string exd_id{ "dasd asd" };
        const UC_node::Type exd_type{ UC_node::USE_CASE };
        const bool exd_status{ true };
        QTest::newRow("usecase \"dasd asd\"")
                << line << exd_name << exd_id << exd_type << exd_status;
    }
    {
        const std::string line{ "usecase \"dasd asd\" AS REG" };
        const std::string exd_name{ "dasd asd" };
        const std::string exd_id{ "REG" };
        const UC_node::Type exd_type{ UC_node::USE_CASE };
        const bool exd_status{ true };
        QTest::newRow("usecase \"dasd asd\" AS REG")
                << line << exd_name << exd_id << exd_type << exd_status;
    }
    {
        const std::string line{ "USECASE   (dasd asd)    AS    (REG)" };
        const std::string exd_name{ "dasd asd" };
        const std::string exd_id{ "REG" };
        const UC_node::Type exd_type{ UC_node::USE_CASE };
        const bool exd_status{ true };
        QTest::newRow("USECASE   (dasd asd)    AS    (REG)")
                << line << exd_name << exd_id << exd_type << exd_status;
    }
    // *** 15
    {
        const std::string line{ "usecase \"Reg reg\" as as" };
        const std::string exd_name{ "Reg reg" };
        const std::string exd_id{ "as" };
        const UC_node::Type exd_type{ UC_node::USE_CASE };
        const bool exd_status{ true };
        QTest::newRow("usecase \"Reg reg\" as as")
                << line << exd_name << exd_id << exd_type << exd_status;
    }
    {
        const std::string line{ "usecase Reg as \"Обычная регистрация\nпользователя\nбез.\"" };
        const std::string exd_name{ "Обычная регистрация\nпользователя\nбез." };
        const std::string exd_id{ "Reg" };
        const UC_node::Type exd_type{ UC_node::USE_CASE };
        const bool exd_status{ true };
        QTest::newRow("usecase Reg as \"Обычная регистрация\\nпользователя\\nбез.\"")
                << line << exd_name << exd_id << exd_type << exd_status;
    }

    /* false */
    {
        const std::string line{ "usecase Reg bs" };
        const std::string exd_name{ "" };
        const std::string exd_id{ "" };
        const UC_node::Type exd_type{ UC_node::USE_CASE };
        const bool exd_status{ false };
        QTest::newRow("usecase Reg bs")
                << line << exd_name << exd_id << exd_type << exd_status;
    }
    {
        const std::string line{ "uscase Reg bs" };
        const std::string exd_name{ "" };
        const std::string exd_id{ "" };
        const UC_node::Type exd_type{ UC_node::USE_CASE };
        const bool exd_status{ false };
        QTest::newRow("uscase Reg bs")
                << line << exd_name << exd_id << exd_type << exd_status;
    }
    {
        const std::string line{ "usecase \"Reg reg\" as as as" };
        const std::string exd_name{ "" };
        const std::string exd_id{ "" };
        const UC_node::Type exd_type{ UC_node::USE_CASE };
        const bool exd_status{ false };
        QTest::newRow("usecase \"Reg reg\" as as as")
                << line << exd_name << exd_id << exd_type << exd_status;
    }
    {
        const std::string line{ "usecase \"Reg reg" };
        const std::string exd_name{ "" };
        const std::string exd_id{ "" };
        const UC_node::Type exd_type{ UC_node::USE_CASE };
        const bool exd_status{ false };
        QTest::newRow("usecase \"Reg reg")
                << line << exd_name << exd_id << exd_type << exd_status;
    }
    {
        const std::string line{ "usecase :Reg reg: as :sss" };
        const std::string exd_name{ "" };
        const std::string exd_id{ "" };
        const UC_node::Type exd_type{ UC_node::USE_CASE };
        const bool exd_status{ false };
        QTest::newRow("usecase \"Reg reg")
                << line << exd_name << exd_id << exd_type << exd_status;
    }
    // *** 5
    {
        const std::string line{ "usecase (RegReg as d" };
        const std::string exd_name{ "" };
        const std::string exd_id{ "" };
        const UC_node::Type exd_type{ UC_node::USE_CASE };
        const bool exd_status{ false };
        QTest::newRow("usecase (RegReg as d")
                << line << exd_name << exd_id << exd_type << exd_status;
    }
}

void Module::test_Puml_utils_UC_dia_read_use_case_creation()
{
    using namespace lenv;
    QFETCH(std::string, line);
    QFETCH(std::string, exd_name);
    QFETCH(std::string, exd_id);
    QFETCH(UC_node::Type, exd_type);
    QFETCH(bool, exd_status);

    // ***

    std::string got_name, got_id;
    UC_node::Type got_type{ UC_node::ACTOR };
    QCOMPARE_EQ(wsd_utils::UC_dia::read_use_case_creation(line,
                    got_name, got_id, got_type), exd_status);

    if (exd_status) {
        QCOMPARE_EQ(exd_id, got_id);
        QCOMPARE_EQ(exd_name, got_name);
        QCOMPARE_EQ(exd_type, got_type);
    }
}

void Module::test_Puml_utils_UC_dia_read_actor_creation_data()
{
    using namespace lenv;
    QTest::addColumn<std::string>("line");
    QTest::addColumn<std::string>("exd_name");
    QTest::addColumn<std::string>("exd_id");
    QTest::addColumn<UC_node::Type>("exd_type");
    QTest::addColumn<bool>("exd_status");

    /* true */
    {
        const std::string line{ "actor User" };
        const std::string exd_name{ "User" };
        const std::string exd_id{ "User" };
        const UC_node::Type exd_type{ UC_node::ACTOR };
        const bool exd_status{ true };
        QTest::newRow("actor User")
                << line << exd_name << exd_id << exd_type << exd_status;
    }
    {
        const std::string line{ "actor (User) as \"dsa\"" };
        const std::string exd_name{ "dsa" };
        const std::string exd_id{ "User" };
        const UC_node::Type exd_type{ UC_node::USE_CASE };
        const bool exd_status{ true };
        QTest::newRow("actor (User) as \"dsa\"")
                << line << exd_name << exd_id << exd_type << exd_status;
    }
    {
        const std::string line{ "actor (User) as (dsa)" };
        const std::string exd_name{ "User" };
        const std::string exd_id{ "dsa" };
        const UC_node::Type exd_type{ UC_node::USE_CASE };
        const bool exd_status{ true };
        QTest::newRow("actor (User) as (dsa)")
                << line << exd_name << exd_id << exd_type << exd_status;
    }
    {
        const std::string line{ "actor (User) as :dsa:" };
        const std::string exd_name{ "User" };
        const std::string exd_id{ "dsa" };
        const UC_node::Type exd_type{ UC_node::USE_CASE };
        const bool exd_status{ true };
        QTest::newRow("actor (User) as :dsa:")
                << line << exd_name << exd_id << exd_type << exd_status;
    }
    {
        const std::string line{ "actor :User: as (dsa)" };
        const std::string exd_name{ "User" };
        const std::string exd_id{ "dsa" };
        const UC_node::Type exd_type{ UC_node::USE_CASE };
        const bool exd_status{ true };
        QTest::newRow("actor :User: as (dsa)")
                << line << exd_name << exd_id << exd_type << exd_status;
    }
    // *** 5
    {
        const std::string line{ "actor User as dsa" };
        const std::string exd_name{ "User" };
        const std::string exd_id{ "dsa" };
        const UC_node::Type exd_type{ UC_node::ACTOR };
        const bool exd_status{ true };
        QTest::newRow("actor User as dsa")
                << line << exd_name << exd_id << exd_type << exd_status;
    }
    {
        const std::string line{ "actor (User)" };
        const std::string exd_name{ "User" };
        const std::string exd_id{ "User" };
        const UC_node::Type exd_type{ UC_node::USE_CASE };
        const bool exd_status{ true };
        QTest::newRow("actor (User)")
                << line << exd_name << exd_id << exd_type << exd_status;
    }

    /* false */
    {
        const std::string line{ "actor ds ds ds" };
        const std::string exd_name{ "" };
        const std::string exd_id{ "" };
        const UC_node::Type exd_type{ UC_node::ACTOR };
        const bool exd_status{ false };
        QTest::newRow("actor ds ds ds")
                << line << exd_name << exd_id << exd_type << exd_status;
    }
    {
        const std::string line{ "actor (())ds" };
        const std::string exd_name{ "" };
        const std::string exd_id{ "" };
        const UC_node::Type exd_type{ UC_node::ACTOR };
        const bool exd_status{ false };
        QTest::newRow("actor (())ds")
                << line << exd_name << exd_id << exd_type << exd_status;
    }
    {
        const std::string line{ "actor User as ds dffs w" };
        const std::string exd_name{ "" };
        const std::string exd_id{ "" };
        const UC_node::Type exd_type{ UC_node::ACTOR };
        const bool exd_status{ false };
        QTest::newRow("actor User as ds dffs w")
                << line << exd_name << exd_id << exd_type << exd_status;
    }
}

void Module::test_Puml_utils_UC_dia_read_actor_creation()
{
    using namespace lenv;
    QFETCH(std::string, line);
    QFETCH(std::string, exd_name);
    QFETCH(std::string, exd_id);
    QFETCH(UC_node::Type, exd_type);
    QFETCH(bool, exd_status);

    // ***

    std::string got_name, got_id;
    UC_node::Type got_type{ UC_node::ACTOR };
    QCOMPARE_EQ(wsd_utils::UC_dia::read_actor_creation(line,
                    got_name, got_id, got_type), exd_status);

    if (exd_status) {
        QCOMPARE_EQ(exd_id, got_id);
        QCOMPARE_EQ(exd_name, got_name);
        QCOMPARE_EQ(exd_type, got_type);
    }
}

void Module::test_Puml_utils_UC_dia_read_connection_creation_data()
{
    using namespace lenv;
    QTest::addColumn<std::string>("line");
    QTest::addColumn<std::string>("exd_lstr");
    QTest::addColumn<std::string>("exd_rstr");
    QTest::addColumn<UC_edge::Type>("exd_edge_type");
    QTest::addColumn<bool>("exd_status");

    /* true */
    {
        const std::string line{ ":das: --> (dasda)" };
        const std::string exd_lstr{ ":das:" };
        const std::string exd_rstr{ "(dasda)" };
        const UC_edge::Type exd_type{ UC_edge::ASSOCIATION };
        const bool exd_status{ true };
        QTest::newRow(":das: --> (dasda)")
                << line << exd_lstr << exd_rstr << exd_type << exd_status;
    }
}

void Module::test_Puml_utils_UC_dia_read_connection_creation()
{
    using namespace lenv;
    QFETCH(std::string, line);
    QFETCH(std::string, exd_lstr);
    QFETCH(std::string, exd_rstr);
    QFETCH(UC_edge::Type, exd_edge_type);
    QFETCH(bool, exd_status);

    // ***

//    std::string got_lstr, got_rstr;
//    UC_edge::Type got_edge_type{ UC_edge::ASSOCIATION };
//    QCOMPARE_EQ(Puml_utils::UC_dia::read_connection_creation(line,
//                    got_lstr, got_rstr, got_edge_type), exd_status);

//    if (exd_status) {
//        QCOMPARE_EQ(exd_lstr, got_lstr);
//        QCOMPARE_EQ(exd_rstr, got_rstr);
//        QCOMPARE_EQ(exd_edge_type, got_edge_type);
//    }
}

void Module::test_Puml_utils_UC_dia_arrow_to_type_data()
{
    using namespace lenv;
    QTest::addColumn<std::string>("arrow");
    QTest::addColumn<UC_edge::Type>("exd_edge_type");
    QTest::addColumn<bool>("exd_status");

    /* true */
    {
        const std::string arrow{ "-->" };
        const UC_edge::Type exd_edge_type{ UC_edge::ASSOCIATION };
        const bool exd_status{ true };
        QTest::newRow("-->") << arrow << exd_edge_type << exd_status;
    }
}

void Module::test_Puml_utils_UC_dia_arrow_to_type()
{

}

// -----------------------------------------------------------------------

void Module::test_Lex_analyzer_tokenize_okk()
{
    std::istringstream sin{
        "@startuml\n"
        "@enduml"
    };
    const std::vector<lenv::Token> expect{
        lenv::Token{ "@startuml", lenv::Token::DIRECTIVE_STARTUML },
        lenv::Token{ "\n", lenv::Token::LINE_END },
        lenv::Token{ "@enduml", lenv::Token::DIRECTIVE_ENDUML },
    };

    std::vector<lenv::Token> actual;
    QVERIFY_THROWS_NO_EXCEPTION(actual = lenv::tokenize(sin));
    QCOMPARE_EQ(expect, actual);
}

void Module::test_Lex_analyzer_tokenize_okk1()
{
    using namespace lenv;
    std::istringstream sin{
        "\n"
    };
    const std::vector<Token> expect{
        Token{ "\n", Token::LINE_END },
    };

    std::vector<Token> actual;
    QVERIFY_THROWS_NO_EXCEPTION(actual = tokenize(sin));
    QCOMPARE_EQ(expect, actual);
}

void Module::test_Lex_analyzer_tokenize_okk2()
{
    std::istringstream sin{
        "@startuml\n"
    };
    const std::vector<lenv::Token> expect{
        lenv::Token{ "@startuml", lenv::Token::DIRECTIVE_STARTUML },
        lenv::Token{ "\n", lenv::Token::LINE_END },
    };

    std::vector<lenv::Token> actual;
    QVERIFY_THROWS_NO_EXCEPTION(actual = lenv::tokenize(sin));
    QCOMPARE_EQ(expect, actual);
}

void Module::test_Lex_analyzer_tokenize_okk3()
{
    std::istringstream sin{
        "@startuml\n"
        "@enduml "
    };
    const std::vector<lenv::Token> expect{
        lenv::Token{ "@startuml", lenv::Token::DIRECTIVE_STARTUML },
        lenv::Token{ "\n", lenv::Token::LINE_END },
        lenv::Token{ "@enduml", lenv::Token::DIRECTIVE_ENDUML },
        lenv::Token{ " ", lenv::Token::WHITESPACE },
    };

    std::vector<lenv::Token> actual;
    QVERIFY_THROWS_NO_EXCEPTION(actual = lenv::tokenize(sin));
    QCOMPARE_EQ(expect, actual);
}

void Module::test_Lex_analyzer_tokenize_okk4()
{
    std::istringstream sin{
        "@startuml\n"
        "@endumldasdadas"
    };
    const std::vector<lenv::Token> expect{
        lenv::Token{ "@startuml", lenv::Token::DIRECTIVE_STARTUML },
        lenv::Token{ "\n", lenv::Token::LINE_END },
        lenv::Token{ "@enduml", lenv::Token::DIRECTIVE_ENDUML },
        lenv::Token{ "dasdadas", lenv::Token::DIRECTIVE_VALUE },
    };

    std::vector<lenv::Token> actual;
    QVERIFY_THROWS_NO_EXCEPTION(actual = lenv::tokenize(sin));
    QCOMPARE_EQ(expect, actual);
}

void Module::test_Lex_analyzer_tokenize_okk5()
{
    std::istringstream sin{
        "@startuml  \t    dia_name     \n"
        "@enduml"
    };
    const std::vector<lenv::Token> expect{
        lenv::Token{ "@startuml", lenv::Token::DIRECTIVE_STARTUML },
        lenv::Token{ "  \t    ", lenv::Token::WHITESPACE },
        lenv::Token{ "dia_name", lenv::Token::DIRECTIVE_VALUE },
        lenv::Token{ "     ", lenv::Token::WHITESPACE },
        lenv::Token{ "\n", lenv::Token::LINE_END },
        lenv::Token{ "@enduml", lenv::Token::DIRECTIVE_ENDUML },
    };

    std::vector<lenv::Token> actual;
    QVERIFY_THROWS_NO_EXCEPTION(actual = lenv::tokenize(sin));
    QCOMPARE_EQ(expect, actual);
}

void Module::test_Lex_analyzer_tokenize_okk6()
{
    std::istringstream sin{""};
    std::vector<lenv::Token> expect{};

    std::vector<lenv::Token> actual;
    QVERIFY_THROWS_NO_EXCEPTION(actual = lenv::tokenize(sin));
    QCOMPARE_EQ(expect, actual);
}

void Module::test_Lex_analyzer_tokenize_okk7()
{
    std::istringstream sin{
        "@startumlasdasd       "
    };
    const std::vector<lenv::Token> expect{
        lenv::Token{ "@startuml", lenv::Token::DIRECTIVE_STARTUML },
        lenv::Token{ "asdasd", lenv::Token::DIRECTIVE_VALUE },
        lenv::Token{ "       ", lenv::Token::WHITESPACE },
    };

    std::vector<lenv::Token> actual;
    QVERIFY_THROWS_NO_EXCEPTION(actual = lenv::tokenize(sin));
    QCOMPARE_EQ(expect, actual);
}

void Module::test_Lex_analyzer_tokenize_okk8()
{
    std::istringstream sin{
        "    \t\t\n"
    };
    const std::vector<lenv::Token> expect{
        lenv::Token{ "    \t\t", lenv::Token::WHITESPACE },
        lenv::Token{ "\n", lenv::Token::LINE_END },
    };

    std::vector<lenv::Token> actual;
    QVERIFY_THROWS_NO_EXCEPTION(actual = lenv::tokenize(sin));
    QCOMPARE_EQ(expect, actual);
}

void Module::test_Lex_analyzer_tokenize_okk9()
{
    std::istringstream sin{
        "@startuml\n"
        ":   dsad ds sd sd:\n"
        "@enduml"
    };
    const std::vector<lenv::Token> expect{
        lenv::Token{ "@startuml", lenv::Token::DIRECTIVE_STARTUML },
        lenv::Token{ "\n", lenv::Token::LINE_END },
        lenv::Token{ "   dsad ds sd sd", lenv::Token::ACTOR_FAST_USE },
        lenv::Token{ "\n", lenv::Token::LINE_END },
        lenv::Token{ "@enduml", lenv::Token::DIRECTIVE_ENDUML },
    };

    std::vector<lenv::Token> actual;
    QVERIFY_THROWS_NO_EXCEPTION(actual = lenv::tokenize(sin));
    QCOMPARE_EQ(expect, actual);
}

void Module::test_Lex_analyzer_tokenize_okk10()
{
    std::istringstream sin{
        "@startuml\n"
        ":Первый Actor:\n"
        ":Второй\\nactor: as Участник2\n"
        "actor Участник3\n"
        "actor :Последний actor: as     Участник4\n"
        "@enduml"
    };
    const std::vector<lenv::Token> expect{
        lenv::Token{ "@startuml", lenv::Token::DIRECTIVE_STARTUML },
        lenv::Token{ "\n", lenv::Token::LINE_END },
        lenv::Token{ "Первый Actor", lenv::Token::ACTOR_FAST_USE },
        lenv::Token{ "\n", lenv::Token::LINE_END },
        lenv::Token{ "Второй\\nactor", lenv::Token::ACTOR_FAST_USE },
        lenv::Token{ " ", lenv::Token::WHITESPACE },
        lenv::Token{ "as", lenv::Token::KW_AS },
        lenv::Token{ " ", lenv::Token::WHITESPACE },
        lenv::Token{ "Участник2", lenv::Token::IDENTIFIER },
        lenv::Token{ "\n", lenv::Token::LINE_END },
        lenv::Token{ "actor", lenv::Token::KW_ACTOR },
        lenv::Token{ " ", lenv::Token::WHITESPACE },
        lenv::Token{ "Участник3", lenv::Token::IDENTIFIER },
        lenv::Token{ "\n", lenv::Token::LINE_END },
        lenv::Token{ "actor", lenv::Token::KW_ACTOR },
        lenv::Token{ " ", lenv::Token::WHITESPACE },
        lenv::Token{ "Последний actor", lenv::Token::ACTOR_FAST_USE },
        lenv::Token{ " ", lenv::Token::WHITESPACE },
        lenv::Token{ "as", lenv::Token::KW_AS },
        lenv::Token{ "     ", lenv::Token::WHITESPACE },
        lenv::Token{ "Участник4", lenv::Token::IDENTIFIER },
        lenv::Token{ "\n", lenv::Token::LINE_END },
        lenv::Token{ "@enduml", lenv::Token::DIRECTIVE_ENDUML },
    };

    std::vector<lenv::Token> actual;
    QVERIFY_THROWS_NO_EXCEPTION(actual = lenv::tokenize(sin));
    QCOMPARE_EQ(expect, actual);
}

void Module::test_Lex_analyzer_tokenize_okk11()
{
    std::istringstream sin{
        "actor"
    };
    const std::vector<lenv::Token> expect{
        lenv::Token{ "actor", lenv::Token::KW_ACTOR },
    };

    std::vector<lenv::Token> actual;
    QVERIFY_THROWS_NO_EXCEPTION(actual = lenv::tokenize(sin));
    QCOMPARE_EQ(expect, actual);
}

void Module::test_Lex_analyzer_tokenize_okk12()
{
    using namespace lenv;
    std::istringstream sin{
        "@startuml uc_expts  \n"
        "\n"
        "(Один прецедент)\n"
        "(Другой прецедент) as (UC2)\n"
        "usecase Прецедент3   \n"
        "usecase (Последний\\nпрецедент) as UC4\n"
        "\n"
        "@enduml"
    };
    const std::vector<Token> expect{
        Token{ "@startuml", lenv::Token::DIRECTIVE_STARTUML },
        Token{ " ", lenv::Token::WHITESPACE },
        Token{ "uc_expts", lenv::Token::DIRECTIVE_VALUE },
        Token{ "  ", lenv::Token::WHITESPACE },
        Token{ "\n", lenv::Token::LINE_END },

        Token{ "\n", lenv::Token::LINE_END },

        Token{ "Один прецедент", lenv::Token::USECASE_FAST_USE },
        Token{ "\n", lenv::Token::LINE_END },

        Token{ "Другой прецедент", lenv::Token::USECASE_FAST_USE },
        Token{ " ", lenv::Token::WHITESPACE },
        Token{ "as", lenv::Token::KW_AS },
        Token{ " ", lenv::Token::WHITESPACE },
        Token{ "UC2", lenv::Token::USECASE_FAST_USE },
        Token{ "\n", lenv::Token::LINE_END },

        Token{ "usecase", lenv::Token::KW_USECASE },
        Token{ " ", lenv::Token::WHITESPACE },
        Token{ "Прецедент3", lenv::Token::IDENTIFIER },
        Token{ "   ", lenv::Token::WHITESPACE },
        Token{ "\n", lenv::Token::LINE_END },

        Token{ "usecase", lenv::Token::KW_USECASE },
        Token{ " ", lenv::Token::WHITESPACE },
        Token{ "Последний\\nпрецедент", lenv::Token::USECASE_FAST_USE },
        Token{ " ", lenv::Token::WHITESPACE },
        Token{ "as", lenv::Token::KW_AS },
        Token{ " ", lenv::Token::WHITESPACE },
        Token{ "UC4", lenv::Token::IDENTIFIER },
        Token{ "\n", lenv::Token::LINE_END },

        Token{ "\n", lenv::Token::LINE_END },

        Token{ "@enduml", lenv::Token::DIRECTIVE_ENDUML },
    };

    std::vector<Token> actual;
    QVERIFY_THROWS_NO_EXCEPTION(actual = tokenize(sin));
    QCOMPARE_EQ(expect.size(), actual.size());
    QCOMPARE_EQ(expect, actual);

    /*
    for (size_t i = 0; i < expect.size(); ++i) {
        QCOMPARE_EQ(expect[i], actual[i]);
    }
    */
}

void Module::test_Lex_analyzer_tokenize_okk13()
{
    std::istringstream sin{
        "@startuml\n"
        ":User: --> (Reg)\n"
        "@enduml"
    };
    const std::vector<lenv::Token> expect{
        lenv::Token{ "@startuml", lenv::Token::DIRECTIVE_STARTUML },
        lenv::Token{ "\n", lenv::Token::LINE_END },

        lenv::Token{ "User", lenv::Token::ACTOR_FAST_USE },
        lenv::Token{ " ", lenv::Token::WHITESPACE },
        lenv::Token{ "-->", lenv::Token::ARROW },
        lenv::Token{ " ", lenv::Token::WHITESPACE },
        lenv::Token{ "Reg", lenv::Token::USECASE_FAST_USE },
        lenv::Token{ "\n", lenv::Token::LINE_END },

        lenv::Token{ "@enduml", lenv::Token::DIRECTIVE_ENDUML },
    };

    std::vector<lenv::Token> actual;
    QVERIFY_THROWS_NO_EXCEPTION(actual = lenv::tokenize(sin));
    QCOMPARE_EQ(expect, actual);
}

void Module::test_Lex_analyzer_tokenize_okk14()
{
    std::istringstream sin{
        "@startuml\n"
        "usecase Registration AS Reg\n"
        "actor User as User\n"
        "\n"
        "User --> Reg\n"
        "User<--Reg\n"
        "@enduml"
    };
    const std::vector<lenv::Token> expect{
        lenv::Token{ "@startuml", lenv::Token::DIRECTIVE_STARTUML },
        lenv::Token{ "\n", lenv::Token::LINE_END },

        lenv::Token{ "usecase", lenv::Token::KW_USECASE },
        lenv::Token{ " ", lenv::Token::WHITESPACE },
        lenv::Token{ "Registration", lenv::Token::IDENTIFIER },
        lenv::Token{ " ", lenv::Token::WHITESPACE },
        lenv::Token{ "AS", lenv::Token::KW_AS },
        lenv::Token{ " ", lenv::Token::WHITESPACE },
        lenv::Token{ "Reg", lenv::Token::IDENTIFIER },
        lenv::Token{ "\n", lenv::Token::LINE_END },

        lenv::Token{ "actor", lenv::Token::KW_ACTOR },
        lenv::Token{ " ", lenv::Token::WHITESPACE },
        lenv::Token{ "User", lenv::Token::IDENTIFIER },
        lenv::Token{ " ", lenv::Token::WHITESPACE },
        lenv::Token{ "as", lenv::Token::KW_AS },
        lenv::Token{ " ", lenv::Token::WHITESPACE },
        lenv::Token{ "User", lenv::Token::IDENTIFIER },
        lenv::Token{ "\n", lenv::Token::LINE_END },

        lenv::Token{ "\n", lenv::Token::LINE_END },

        lenv::Token{ "User", lenv::Token::IDENTIFIER },
        lenv::Token{ " ", lenv::Token::WHITESPACE },
        lenv::Token{ "-->", lenv::Token::ARROW },
        lenv::Token{ " ", lenv::Token::WHITESPACE },
        lenv::Token{ "Reg", lenv::Token::IDENTIFIER },
        lenv::Token{ "\n", lenv::Token::LINE_END },

        lenv::Token{ "User", lenv::Token::IDENTIFIER },
        lenv::Token{ "<--", lenv::Token::ARROW },
        lenv::Token{ "Reg", lenv::Token::IDENTIFIER },
        lenv::Token{ "\n", lenv::Token::LINE_END },

        lenv::Token{ "@enduml", lenv::Token::DIRECTIVE_ENDUML },
    };

    std::vector<lenv::Token> actual;
    QVERIFY_THROWS_NO_EXCEPTION(actual = lenv::tokenize(sin));
    QCOMPARE_EQ(expect, actual);
}

// -----------------------------------------------------------------------

void Module::test_Lex_analyzer_tokenize_err()
{
    std::istringstream sin{
        "@"
    };
    std::vector<lenv::Token> expect{};

    std::vector<lenv::Token> actual;
    QVERIFY_THROWS_EXCEPTION(lenv::Lexer_error, actual = lenv::tokenize(sin));
    QCOMPARE_EQ(actual.empty(), true);
}

void Module::test_Lex_analyzer_tokenize_err1()
{
    std::istringstream sin{
        "@sta"
    };

    std::vector<lenv::Token> actual;
    QVERIFY_THROWS_EXCEPTION(lenv::Lexer_error, actual = lenv::tokenize(sin));
    QCOMPARE_EQ(actual.empty(), true);
}

void Module::test_Lex_analyzer_tokenize_err2()
{
    std::istringstream sin{
        "@startuml\n"
        "@endu"
    };

    std::vector<lenv::Token> actual;
    QVERIFY_THROWS_EXCEPTION(lenv::Lexer_error, actual = lenv::tokenize(sin));
    QCOMPARE_EQ(actual.empty(), true);
}

void Module::test_Lex_analyzer_tokenize_err3()
{
    std::istringstream sin{
        "@startuml\n"
        ":   dsad ds\n sd sd:\n"
        "@enduml"
    };

    std::vector<lenv::Token> actual;
    QVERIFY_THROWS_EXCEPTION(lenv::Lexer_error, actual = lenv::tokenize(sin));
    QCOMPARE_EQ(actual.empty(), true);
}

// Direct_translator
// -----------------------------------------------------------------------

void Module::test_Direct_translator_convert_uc_dia_data()
{
    QTest::addColumn<std::string>("inn_fpath");
    QTest::addColumn<std::string>("expect_out_fpath");
    QTest::addColumn<std::string>("actual_out_fpath");

    // ***

    auto cur_dir{ QDir::current() };
    QCOMPARE_EQ(cur_dir.cdUp(), true);
    QCOMPARE_EQ(cur_dir.cdUp(), true);
    QCOMPARE_EQ(cur_dir.cd("converter/test/tltr/use_case_dias"), true); // always like this?

    const auto test_catalogs{ cur_dir.entryList(QDir::NoDotAndDotDot|QDir::Dirs) };
    QCOMPARE_EQ(test_catalogs.isEmpty(), false);
    QCOMPARE_EQ(test_catalogs.contains(".."), false);
    QCOMPARE_EQ(test_catalogs.contains("."), false);

    // ***

    for (qsizetype i = 0; i < test_catalogs.size(); ++i) {
        const QDir cur_test_catalog{ cur_dir.absolutePath() + "/" + test_catalogs[i] };
        QCOMPARE_EQ(cur_test_catalog.exists(), true);

        const auto dia_source{ cur_test_catalog.absoluteFilePath("inn.wsd") }; // required PlantUML OK!
        const auto dia_expect_destin{ cur_test_catalog.absoluteFilePath("expect_out.json") }; // at least empty!
        const auto dia_actual_destin{ cur_test_catalog.absoluteFilePath("actual_out.json") }; // may not exist!

        QCOMPARE_EQ(QFile::exists(dia_source), true);
        QCOMPARE_EQ(QFile::exists(dia_expect_destin), true);
        //QCOMPARE_EQ(QFile::exists(dia_actual_destin), true);

        const std::string inn_fpath{ dia_source.toStdString() };
        const std::string expect_out_fpath{ dia_expect_destin.toStdString() };
        const std::string actual_out_fpath{ dia_actual_destin.toStdString() };

        const std::string tst_name{ "catalog name: /" + test_catalogs[i].toStdString() };
        QTest::newRow(tst_name.c_str()) << inn_fpath << expect_out_fpath << actual_out_fpath;
    }
}

void Module::test_Direct_translator_convert_uc_dia()
{
    QFETCH(std::string, inn_fpath);
    QFETCH(std::string, expect_out_fpath);
    QFETCH(std::string, actual_out_fpath);

    // *** open all files now! ***

    std::ifstream fin_inn{ inn_fpath };
    QCOMPARE_EQ(fin_inn.is_open(), true);

    std::ifstream fin_expect{ expect_out_fpath };
    QCOMPARE_EQ(fin_inn.is_open(), true);

    std::ofstream fout_actual{ actual_out_fpath };
    QCOMPARE_EQ(fout_actual.is_open(), true);

    // *** check the file with PlantUML ***

    // TODO:

    // *** write what happened ***

    auto tr = lenv::Direct_translator{};
    auto uc_dia_sp = tr.convert_uc_dia(fin_inn);

    nlohmann::json actual;
    QVERIFY_THROWS_NO_EXCEPTION(actual = uc_dia_sp->to_whole_json());
    fout_actual << std::setw(2) << actual; // not sticky manipulator!

    // *** compare adjusted expectation ***

    nlohmann::json expected;
    QVERIFY_THROWS_NO_EXCEPTION(fin_expect >> expected);

    QCOMPARE_EQ(actual.is_object(), true);
    QCOMPARE_EQ(expected.is_object(), true);
    QCOMPARE_EQ(actual == expected, true);
}

QTEST_APPLESS_MAIN(Module)
