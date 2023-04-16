#include <sstream>
#include <fstream>
#include <algorithm>

#include "tst_module.h"

#include "usecasegraph.h"
#include "robustnessgraph.h"
#include "classgraph.h"
#include "grapherror.h"

#include "str_utils.h"
#include "nlohmann/json.hpp"

#if QT_VERSION <= QT_VERSION_CHECK(6, 4, 0)
    Q_DECLARE_METATYPE(std::string);

    #define QCOMPARE_EQ(lhs, rhs) QCOMPARE(lhs == rhs, true);
    #define QCOMPARE_NE(lhs, rhs) QCOMPARE(lhs == rhs, false);

    #define QVERIFY_THROWS_EXCEPTION(exceptiontype, expression) QVERIFY_EXCEPTION_THROWN(expression, exceptiontype);
    #define QVERIFY_THROWS_NO_EXCEPTION(expression) static_cast<void>(expression);
#endif

using namespace std;
using namespace nlohmann;

// str_utils
// -----------------------------------------------------------------------
void Module::test_str_utils_to_upper_data()
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

void Module::test_str_utils_to_upper()
{
    QFETCH(std::string, str);
    QFETCH(std::string, res);

    auto got{ str_utils::to_upper(str) };
    QCOMPARE_EQ(got, res);
}

void Module::test_str_utils_to_lower_data()
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

void Module::test_str_utils_to_lower()
{
    QFETCH(std::string, str);
    QFETCH(std::string, res);

    auto got{ str_utils::to_lower(str) };
    QCOMPARE_EQ(got, res);
}

// -----------------------------------------------------------------------

void Module::test_str_utils_start_with_data()
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

void Module::test_str_utils_start_with()
{
    QFETCH(std::string, str);
    QFETCH(std::string, start);
    QFETCH(bool, sensitive);
    QFETCH(bool, res);

    auto got{ str_utils::start_with(str, start, sensitive) };
    QCOMPARE_EQ(got, res);
}

void Module::test_str_utils_stop_with_data()
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

void Module::test_str_utils_stop_with()
{
    QFETCH(std::string, str);
    QFETCH(std::string, stop);
    QFETCH(bool, sensitive);
    QFETCH(bool, res);

    auto got{ str_utils::stop_with(str, stop, sensitive) };
    QCOMPARE_EQ(got, res);
}

// -----------------------------------------------------------------------

void Module::test_str_utils_eq_data()
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

void Module::test_str_utils_eq()
{
    QFETCH(std::string, lhs);
    QFETCH(std::string, rhs);
    QFETCH(bool, sensitive);
    QFETCH(bool, res);

    auto got{ str_utils::eq(lhs, rhs, sensitive) };
    QCOMPARE_EQ(got, res);
}

void Module::test_str_utils_eq_ref_data()
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

void Module::test_str_utils_eq_ref()
{
    QFETCH(std::string, lhs);
    QFETCH(std::string, rhs);
    QFETCH(bool, sensitive);
    QFETCH(bool, res);

    auto got{ str_utils::eq_ref(lhs, rhs, sensitive) };
    QCOMPARE_EQ(got, res);
}

// -----------------------------------------------------------------------

void Module::test_str_utils_trim_left_data()
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

void Module::test_str_utils_trim_left()
{
    QFETCH(std::string, str);
    QFETCH(std::string, chs);
    QFETCH(std::string, res);

    auto got{ str_utils::trim_left(str, chs) };
    QCOMPARE_EQ(got, res);
}

void Module::test_str_utils_trim_rght_data()
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

void Module::test_str_utils_trim_rght()
{
    QFETCH(std::string, str);
    QFETCH(std::string, chs);
    QFETCH(std::string, res);

    auto got{ str_utils::trim_rght(str, chs) };
    QCOMPARE_EQ(got, res);
}

void Module::test_str_utils_trim_data()
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

void Module::test_str_utils_trim()
{
    QFETCH(std::string, str);
    QFETCH(std::string, chs);
    QFETCH(std::string, res);

    auto got{ str_utils::trim(str, chs) };
    QCOMPARE_EQ(got, res);
}

// -----------------------------------------------------------------------

void Module::test_str_utils_trim_left_space_data()
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

void Module::test_str_utils_trim_left_space()
{
    QFETCH(std::string, str);
    QFETCH(std::string, res);

    auto got{ str_utils::trim_left_space(str) };
    QCOMPARE_EQ(got, res);
}

void Module::test_str_utils_trim_rght_space_data()
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

void Module::test_str_utils_trim_rght_space()
{
    QFETCH(std::string, str);
    QFETCH(std::string, res);

    auto got{ str_utils::trim_rght_space(str) };
    QCOMPARE_EQ(got, res);
}

void Module::test_str_utils_trim_space_data()
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

void Module::test_str_utils_trim_space()
{
    QFETCH(std::string, str);
    QFETCH(std::string, res);

    auto got{ str_utils::trim_space(str) };
    QCOMPARE_EQ(got, res);
}

// UseCaseGraph
// -----------------------------------------------------------------------

void Module::test_UseCaseGraph_read_okk()
{
    UseCaseGraph ucg;
    istringstream sin{
        "@startuml\n"
        "usecase (Last\\nprecedent) as UC4\n"
        "actor :Last\\nprecedent: as A1\n"
        "A1 --> UC4\n"
        "@enduml"
    };
    QVERIFY_THROWS_NO_EXCEPTION(ucg.read_puml(sin));
    QCOMPARE_EQ(ucg.nodes.size(), size_t(2));
    QCOMPARE_EQ(ucg.edges.size(), size_t(1));
}

void Module::test_UseCaseGraph_read_okk1()
{
    UseCaseGraph ucg;
    istringstream sin{
        "   @startuml   \n"
        "  skinparam actorStyle awesome  \n"
        "   \n"
        "  usecase (Последний прецедент)   as UC1  \n"
        "  usecase \"Последний прецедент\" as UC2  \n"
        "  usecase \"Последний прецедент\" as UC3  \n"
        "  usecase (Последний прецедент)   as UC4  \n"
        "   \n"
        "  actor :Последний прецедент:   as A1  \n"
        "  actor \"Последний прецедент\" as A2  \n"
        "  actor \"Последний прецедент\" as A3  \n"
        "  actor :Последний прецедент:   as A4  \n"
        "  \n"
        "  UC1 <--  A1  \n"
        "  UC2 ..>  A2 : <<extend>>  \n"
        "  UC3 ..>  A3 : <<include>> \n"
        "  UC4 --|> A4  \n"
        "  UC5 --> A5   \n"
        "  \n"
        "   @enduml   \n"
    };
    QVERIFY_THROWS_NO_EXCEPTION(ucg.read_puml(sin));
    QCOMPARE_EQ(ucg.nodes.size(), size_t(10));
    QCOMPARE_EQ(ucg.edges.size(), size_t(5));
}

void Module::test_UseCaseGraph_read_okk2()
{
    UseCaseGraph ucg;
    istringstream sin{
        "  UC1 <--  A1  \n"
    };
    QVERIFY_THROWS_NO_EXCEPTION(ucg.read_puml(sin));
    QCOMPARE_EQ(ucg.nodes.size(), size_t(2));
    QCOMPARE_EQ(ucg.edges.size(), size_t(1));

    // ***

    {
        auto detected_node = ucg.nodes.begin();
        QVERIFY_THROWS_NO_EXCEPTION(detected_node = find_if(begin(ucg.nodes), end(ucg.nodes),
                                                            [](const shared_ptr<Graph::Node> node) {
            return node->id == "UC1";
        }));
        QCOMPARE_EQ((detected_node == ucg.nodes.end()), false);

        auto uc_node = static_pointer_cast<UseCaseGraph::UcNode>(*detected_node);
        QCOMPARE_EQ(uc_node->id, string("UC1"));
        QCOMPARE_EQ(uc_node->name, string("UC1"));
        QCOMPARE_EQ(uc_node->inns.size(), size_t(1));
        QCOMPARE_EQ(uc_node->outs.size(), size_t(0));
        QCOMPARE_EQ(uc_node->type, UseCaseGraph::UcNode::Actor);
    }
    // ***
    {
        auto detected_node = ucg.nodes.begin();
        QVERIFY_THROWS_NO_EXCEPTION(detected_node = find_if(begin(ucg.nodes), end(ucg.nodes),
                                                            [](const shared_ptr<Graph::Node> node) {
            return node->id == "A1";
        }));
        QCOMPARE_EQ((detected_node == ucg.nodes.end()), false);

        auto uc_node = static_pointer_cast<UseCaseGraph::UcNode>(*detected_node);
        QCOMPARE_EQ(uc_node->id, string("A1"));
        QCOMPARE_EQ(uc_node->name, string("A1"));
        QCOMPARE_EQ(uc_node->outs.size(), size_t(1));
        QCOMPARE_EQ(uc_node->inns.size(), size_t(0));
        QCOMPARE_EQ(uc_node->type, UseCaseGraph::UcNode::Actor);
    }
    // ***
    {
        auto detected_edge = ucg.edges[0];
        auto uc_edge = static_pointer_cast<UseCaseGraph::UcEdge>(detected_edge);

        QCOMPARE_EQ(uc_edge->id, string("1"));
        QCOMPARE_EQ(uc_edge->name, string(""));
        QCOMPARE_EQ(uc_edge->type, UseCaseGraph::UcEdge::Association);
        QCOMPARE_EQ(uc_edge->beg.lock()->id, string("A1"));
        QCOMPARE_EQ(uc_edge->end.lock()->id, string("UC1"));
    }
}

void Module::test_UseCaseGraph_read_okk3()
{
    UseCaseGraph ucg;
    istringstream sin{
        "usecase   (Последний прецедент)   as   UC1   \n"
        "   UC1 .u.>   A1   :   <<include>>   \n"
    };
    QVERIFY_THROWS_NO_EXCEPTION(ucg.read_puml(sin));
    QCOMPARE_EQ(ucg.nodes.size(), size_t(2));
    QCOMPARE_EQ(ucg.edges.size(), size_t(1));

    // ***

    {
        auto detected_node = ucg.nodes.begin();
        QVERIFY_THROWS_NO_EXCEPTION(detected_node = find_if(begin(ucg.nodes), end(ucg.nodes),
                                                            [](const shared_ptr<Graph::Node> node) {
            return node->id == "UC1";
        }));
        QCOMPARE_EQ((detected_node == ucg.nodes.end()), false);

        auto uc_node = static_pointer_cast<UseCaseGraph::UcNode>(*detected_node);
        QCOMPARE_EQ(uc_node->id, string("UC1"));
        QCOMPARE_EQ(uc_node->name, string("Последний прецедент"));
        QCOMPARE_EQ(uc_node->outs.size(), size_t(1));
        QCOMPARE_EQ(uc_node->inns.size(), size_t(0));
        QCOMPARE_EQ(uc_node->type, UseCaseGraph::UcNode::Usecase);
    }
    // ***
    {
        auto detected_node = ucg.nodes.begin();
        QVERIFY_THROWS_NO_EXCEPTION(detected_node = find_if(begin(ucg.nodes), end(ucg.nodes),
                                                            [](const shared_ptr<Graph::Node> node) {
            return node->id == "A1";
        }));
        QCOMPARE_EQ((detected_node == ucg.nodes.end()), false);

        auto uc_node = static_pointer_cast<UseCaseGraph::UcNode>(*detected_node);
        QCOMPARE_EQ(uc_node->id, string("A1"));
        QCOMPARE_EQ(uc_node->name, string("A1"));
        QCOMPARE_EQ(uc_node->inns.size(), size_t(1));
        QCOMPARE_EQ(uc_node->outs.size(), size_t(0));
        QCOMPARE_EQ(uc_node->type, UseCaseGraph::UcNode::Actor);
    }
    // ***
    {
        auto detected_edge = ucg.edges[0];
        auto uc_edge = static_pointer_cast<UseCaseGraph::UcEdge>(detected_edge);

        QCOMPARE_EQ(uc_edge->id, string("1"));
        QCOMPARE_EQ(uc_edge->name, string("<<include>>"));
        QCOMPARE_EQ(uc_edge->type, UseCaseGraph::UcEdge::Include);
        QCOMPARE_EQ(uc_edge->beg.lock()->id, string("UC1"));
        QCOMPARE_EQ(uc_edge->end.lock()->id, string("A1"));
    }
}

void Module::test_UseCaseGraph_read_okk4()
{
    UseCaseGraph ucg;
    istringstream sin{
        "  top to bottom   direction     \n"
        "  usecase   (Last precedent)   as   UC1  \n"
        "  actor :Last actor: as A1   \n"
        "  UC1 <---down--- A1   \n"
    };
    QVERIFY_THROWS_NO_EXCEPTION(ucg.read_puml(sin));
    QCOMPARE_EQ(ucg.nodes.size(), size_t(2));
    QCOMPARE_EQ(ucg.edges.size(), size_t(1));

    // ***

    {
        auto detected_node = ucg.nodes.begin();
        QVERIFY_THROWS_NO_EXCEPTION(detected_node = find_if(begin(ucg.nodes), end(ucg.nodes),
                                                            [](const shared_ptr<Graph::Node> node) {
            return node->id == "UC1";
        }));
        QCOMPARE_EQ((detected_node == ucg.nodes.end()), false);

        auto uc_node = static_pointer_cast<UseCaseGraph::UcNode>(*detected_node);
        QCOMPARE_EQ(uc_node->id, string("UC1"));
        QCOMPARE_EQ(uc_node->name, string("Last precedent"));
        QCOMPARE_EQ(uc_node->inns.size(), size_t(1));
        QCOMPARE_EQ(uc_node->outs.size(), size_t(0));
        QCOMPARE_EQ(uc_node->type, UseCaseGraph::UcNode::Usecase);
    }
    // ***
    {
        auto detected_node = ucg.nodes.begin();
        QVERIFY_THROWS_NO_EXCEPTION(detected_node = find_if(begin(ucg.nodes), end(ucg.nodes),
                                                            [](const shared_ptr<Graph::Node> node) {
            return node->id == "A1";
        }));
        QCOMPARE_EQ((detected_node == ucg.nodes.end()), false);

        auto uc_node = static_pointer_cast<UseCaseGraph::UcNode>(*detected_node);
        QCOMPARE_EQ(uc_node->id, string("A1"));
        QCOMPARE_EQ(uc_node->name, string("Last actor"));
        QCOMPARE_EQ(uc_node->outs.size(), size_t(1));
        QCOMPARE_EQ(uc_node->inns.size(), size_t(0));
        QCOMPARE_EQ(uc_node->type, UseCaseGraph::UcNode::Actor);
    }
    // ***
    {
        auto detected_edge = ucg.edges[0];
        auto uc_edge = static_pointer_cast<UseCaseGraph::UcEdge>(detected_edge);

        QCOMPARE_EQ(uc_edge->id, string("1"));
        QCOMPARE_EQ(uc_edge->name, string(""));
        QCOMPARE_EQ(uc_edge->type, UseCaseGraph::UcEdge::Association);
        QCOMPARE_EQ(uc_edge->end.lock()->id, string("UC1"));
        QCOMPARE_EQ(uc_edge->beg.lock()->id, string("A1"));
    }
}

void Module::test_UseCaseGraph_read_okk5()
{
    UseCaseGraph ucg;
    istringstream sin{
        "  @startuml \n"
        "  top to bottom   direction     \n"
        "  package Ресторан {     \n"
        "   }   "
    };
    QVERIFY_THROWS_NO_EXCEPTION(ucg.read_puml(sin));
    QCOMPARE_EQ(ucg.nodes.size(), size_t(0));
    QCOMPARE_EQ(ucg.edges.size(), size_t(0));
}

// -----------------------------------------------------------------------

void Module::test_UseCaseGraph_read_err()
{
    UseCaseGraph ucg;
    istringstream sin{
        "  @startuml \n"
        "  top to bottom   direction     \n"
        "  package Ресторан {     \n"
    };
    QVERIFY_THROWS_EXCEPTION(GraphError, ucg.read_puml(sin));
    QCOMPARE_EQ(ucg.nodes.size(), size_t(0));
    QCOMPARE_EQ(ucg.edges.size(), size_t(0));
}

void Module::test_UseCaseGraph_read_err1()
{
    UseCaseGraph ucg;
    istringstream sin{
        "  @startuml \n"
        "  top to bottom   direction     \n"
        "  boundary MainWin as MainWin \n"
        "   UC1 .u.>   A1   :   <<include>>   \n"
    };
    QVERIFY_THROWS_EXCEPTION(GraphError, ucg.read_puml(sin));
    QCOMPARE_EQ(ucg.nodes.size(), size_t(0));
    QCOMPARE_EQ(ucg.edges.size(), size_t(0));
}

// -----------------------------------------------------------------------

void Module::test_UseCaseGraph_read_puml_data()
{
    QTest::addColumn<string>("inn_fpath");
    QTest::addColumn<string>("expect_out_fpath");
    QTest::addColumn<string>("actual_out_fpath");

    // ***

    auto cur_dir{ QDir::current() };
    QCOMPARE_EQ(cur_dir.cdUp(), true); QCOMPARE_EQ(cur_dir.cdUp(), true);
    QCOMPARE_EQ(cur_dir.cd("converter_2/test/read_puml/uc_graph"), true);

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

void Module::test_UseCaseGraph_read_puml()
{
    QFETCH(string, inn_fpath);
    QFETCH(string, expect_out_fpath);
    QFETCH(string, actual_out_fpath);

    // ***

    ifstream fin_inn{ inn_fpath };
    QCOMPARE_EQ(fin_inn.is_open(), true);

    ifstream fin_expect{ expect_out_fpath };
    QCOMPARE_EQ(fin_inn.is_open(), true);

    ofstream fout_actual{ actual_out_fpath };
    QCOMPARE_EQ(fout_actual.is_open(), true);

    stringstream siout_actual;

    // ***

    // TODO: проверка синтаксиса инструментом PlantUML

    // ***

    UseCaseGraph uc_graph;
    QVERIFY_THROWS_NO_EXCEPTION(uc_graph.read_puml(fin_inn));
    QVERIFY_THROWS_NO_EXCEPTION(uc_graph.write_json(fout_actual));
    QVERIFY_THROWS_NO_EXCEPTION(uc_graph.write_json(siout_actual));

    // ***

    json actual;
    QVERIFY_THROWS_NO_EXCEPTION(siout_actual >> actual);

    json expected;
    QVERIFY_THROWS_NO_EXCEPTION(fin_expect >> expected);

    QCOMPARE_EQ(actual.is_object(), true);
    QCOMPARE_EQ(expected.is_object(), true);
    QCOMPARE_EQ((actual == expected), true);
}

// RobustnessGraph
// -----------------------------------------------------------------------

void Module::test_RobustnessGraph_read_okk()
{
    RobustnessGraph robG;
    istringstream sin{
        "@startuml\n"
        "\n"
        "actor \"Пользователь\" as User \n"
        "boundary \"Главное окно\" as MainWin \n"
        "boundary \"Окно ошибки\" as ErrWin #red \n"
        "\n"
        "entity DbFacade \n"
        "control load \n"
        "\n"
        "User -- MainWin \n"
        "User -- ErrWin \n"
        "\n"
        "@enduml"
    };
    QVERIFY_THROWS_NO_EXCEPTION(robG.read_puml(sin));
    QCOMPARE_EQ(robG.nodes.size(), size_t(5));
    QCOMPARE_EQ(robG.edges.size(), size_t(4)); // important!
}

void Module::test_RobustnessGraph_read_okk1()
{
    RobustnessGraph robG;
    istringstream sin{
        "actor \"Пользователь\" as User \n"
        "boundary \"Главное окно\" as MainWin \n"
        "\n"
        "User --> MainWin : one line... \n"
        "\n"
    };

    QVERIFY_THROWS_NO_EXCEPTION(robG.read_puml(sin));
    QCOMPARE_EQ(robG.nodes.size(), size_t(2));
    QCOMPARE_EQ(robG.edges.size(), size_t(1));

    // ***

    {
        auto detected_node = robG.nodes.begin();
        QVERIFY_THROWS_NO_EXCEPTION(detected_node = find_if(begin(robG.nodes), end(robG.nodes),
                                                            [](const shared_ptr<Graph::Node> node) {
            return node->id == "User";
        }));
        QCOMPARE_EQ((detected_node == robG.nodes.end()), false);

        auto rob_node = static_pointer_cast<RobustnessGraph::RobNode>(*detected_node);
        QCOMPARE_EQ(rob_node->id, string("User"));
        QCOMPARE_EQ(rob_node->name, string("Пользователь"));
        QCOMPARE_EQ(rob_node->type, RobustnessGraph::RobNode::Actor);
        QCOMPARE_EQ(rob_node->is_error, false);
        QCOMPARE_EQ(rob_node->outs.size(), size_t(1));
        QCOMPARE_EQ(rob_node->inns.size(), size_t(0));
    }
    // ***
    {
        auto detected_node = robG.nodes.begin();
        QVERIFY_THROWS_NO_EXCEPTION(detected_node = find_if(begin(robG.nodes), end(robG.nodes),
                                                            [](const shared_ptr<Graph::Node> node) {
            return node->id == "MainWin";
        }));
        QCOMPARE_EQ((detected_node == robG.nodes.end()), false);

        auto rob_node = static_pointer_cast<RobustnessGraph::RobNode>(*detected_node);
        QCOMPARE_EQ(rob_node->id, string("MainWin"));
        QCOMPARE_EQ(rob_node->name, string("Главное окно"));
        QCOMPARE_EQ(rob_node->type, RobustnessGraph::RobNode::Boundary);
        QCOMPARE_EQ(rob_node->is_error, false);
        QCOMPARE_EQ(rob_node->outs.size(), size_t(0));
        QCOMPARE_EQ(rob_node->inns.size(), size_t(1));
    }
    // ***
    {
        auto detected_edge = robG.edges[0];
        auto uc_edge = static_pointer_cast<RobustnessGraph::RobEdge>(detected_edge);

        QCOMPARE_EQ(uc_edge->id, string("1"));
        QCOMPARE_EQ(uc_edge->name, string("one line..."));
        QCOMPARE_EQ(uc_edge->end.lock()->id, string("MainWin"));
        QCOMPARE_EQ(uc_edge->beg.lock()->id, string("User"));
    }
}

void Module::test_RobustnessGraph_read_okk3()
{
    RobustnessGraph robG;
    istringstream sin{
        "control \"display add student screen\" as display_add_student_screen \n"
        "boundary \"Главное окно\" as MainWin \n"
        "\n"
        "display_add_student_screen -- MainWin : edge text \n"
        "\n"
    };

    QVERIFY_THROWS_NO_EXCEPTION(robG.read_puml(sin));
    QCOMPARE_EQ(robG.nodes.size(), size_t(2));
    QCOMPARE_EQ(robG.edges.size(), size_t(2));

    // ***

    {
        auto detected_node = robG.nodes.begin();
        QVERIFY_THROWS_NO_EXCEPTION(detected_node = find_if(begin(robG.nodes), end(robG.nodes),
                                                            [](const shared_ptr<Graph::Node> node) {
            return node->id == "display_add_student_screen";
        }));
        QCOMPARE_EQ((detected_node == robG.nodes.end()), false);

        auto rob_node = static_pointer_cast<RobustnessGraph::RobNode>(*detected_node);
        QCOMPARE_EQ(rob_node->id, string("display_add_student_screen"));
        QCOMPARE_EQ(rob_node->name, string("display add student screen"));
        QCOMPARE_EQ(rob_node->type, RobustnessGraph::RobNode::Control);
        QCOMPARE_EQ(rob_node->is_error, false);
        QCOMPARE_EQ(rob_node->outs.size(), size_t(1));
        QCOMPARE_EQ(rob_node->inns.size(), size_t(1));
    }
    // ***
    {
        auto detected_node = robG.nodes.begin();
        QVERIFY_THROWS_NO_EXCEPTION(detected_node = find_if(begin(robG.nodes), end(robG.nodes),
                                                            [](const shared_ptr<Graph::Node> node) {
            return node->id == "MainWin";
        }));
        QCOMPARE_EQ((detected_node == robG.nodes.end()), false);

        auto rob_node = static_pointer_cast<RobustnessGraph::RobNode>(*detected_node);
        QCOMPARE_EQ(rob_node->id, string("MainWin"));
        QCOMPARE_EQ(rob_node->name, string("Главное окно"));
        QCOMPARE_EQ(rob_node->type, RobustnessGraph::RobNode::Boundary);
        QCOMPARE_EQ(rob_node->is_error, false);
        QCOMPARE_EQ(rob_node->outs.size(), size_t(1));
        QCOMPARE_EQ(rob_node->inns.size(), size_t(1));
    }
    // ***
    {
        auto detected_edge = robG.edges.begin();
        QVERIFY_THROWS_NO_EXCEPTION(detected_edge = find_if(begin(robG.edges), end(robG.edges),
                                                            [](const shared_ptr<Graph::Edge> edge) {
            return edge->id == "1";
        }));
        auto uc_edge = static_pointer_cast<RobustnessGraph::RobEdge>(*detected_edge);

        QCOMPARE_EQ(uc_edge->id, string("1"));
        QCOMPARE_EQ(uc_edge->name, string("edge text"));
        QCOMPARE_EQ(uc_edge->beg.lock()->id, string("display_add_student_screen"));
        QCOMPARE_EQ(uc_edge->end.lock()->id, string("MainWin"));
    }
    // ***
    {
        auto detected_edge = robG.edges.begin();
        QVERIFY_THROWS_NO_EXCEPTION(detected_edge = find_if(begin(robG.edges), end(robG.edges),
                                                            [](const shared_ptr<Graph::Edge> edge) {
            return edge->id == "2";
        }));
        auto uc_edge = static_pointer_cast<RobustnessGraph::RobEdge>(*detected_edge);

        QCOMPARE_EQ(uc_edge->id, string("2"));
        QCOMPARE_EQ(uc_edge->name, string("edge text"));
        QCOMPARE_EQ(uc_edge->end.lock()->id, string("display_add_student_screen"));
        QCOMPARE_EQ(uc_edge->beg.lock()->id, string("MainWin"));
    }
}

// -----------------------------------------------------------------------

void Module::test_RobustnessGraph_read_puml_data()
{
    QTest::addColumn<string>("inn_fpath");
    QTest::addColumn<string>("expect_out_fpath");
    QTest::addColumn<string>("actual_out_fpath");

    // ***

    auto cur_dir{ QDir::current() };
    QCOMPARE_EQ(cur_dir.cdUp(), true); QCOMPARE_EQ(cur_dir.cdUp(), true);
    QCOMPARE_EQ(cur_dir.cd("converter_2/test/read_puml/rob_graph"), true);

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

void Module::test_RobustnessGraph_read_puml()
{
    QFETCH(string, inn_fpath);
    QFETCH(string, expect_out_fpath);
    QFETCH(string, actual_out_fpath);

    // ***

    ifstream fin_inn{ inn_fpath };
    QCOMPARE_EQ(fin_inn.is_open(), true);

    ifstream fin_expect{ expect_out_fpath };
    QCOMPARE_EQ(fin_inn.is_open(), true);

    ofstream fout_actual{ actual_out_fpath };
    QCOMPARE_EQ(fout_actual.is_open(), true);

    stringstream siout_actual;

    // ***

    // TODO: проверка синтаксиса инструментом PlantUML

    // ***

    RobustnessGraph rob_graph;
    QVERIFY_THROWS_NO_EXCEPTION(rob_graph.read_puml(fin_inn));
    QVERIFY_THROWS_NO_EXCEPTION(rob_graph.write_json(fout_actual));
    QVERIFY_THROWS_NO_EXCEPTION(rob_graph.write_json(siout_actual));

    // ***

    json actual;
    QVERIFY_THROWS_NO_EXCEPTION(siout_actual >> actual);

    json expected;
    QVERIFY_THROWS_NO_EXCEPTION(fin_expect >> expected);

    QCOMPARE_EQ(actual.is_object(), true);
    QCOMPARE_EQ(expected.is_object(), true);
    QCOMPARE_EQ((actual == expected), true);
}

// ClassGraph
// -----------------------------------------------------------------------
void Module::test_ClassGraph_read_okk()
{
    ClassGraph classG;
    istringstream sin{
        "control \"display add student screen\" as display_add_student_screen \n"
        "boundary \"Главное окно\" as MainWin \n"
        "\n"
        "display_add_student_screen -- MainWin : edge text \n"
        "\n"
    };
}

QTEST_APPLESS_MAIN(Module)
