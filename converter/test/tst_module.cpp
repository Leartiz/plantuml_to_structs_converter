#include <sstream>
#include <fstream>
#include <algorithm>

#include "tst_module.h"

#include "usecasegraph.h"
#include "layoutflowgraph.h"
#include "robustnessgraph.h"
#include "sequencegraph.h"
#include "classgraph.h"
#include "grapherror.h"

#include "str_utils.h"
#include "nlohmann/json/json.hpp"

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

// -----------------------------------------------------------------------

void Module::test_str_utils_is_bracket_balance_err()
{
    QVERIFY_THROWS_EXCEPTION(std::runtime_error, str_utils::is_bracket_balance("(123(123))", 'A'));
    // ...
}

void Module::test_str_utils_is_bracket_balance_data()
{
    QTest::addColumn<std::string>("str");
    QTest::addColumn<char>("open_br");
    QTest::addColumn<bool>("result");

    // *** (
    {
        const std::string str{ "(123(123))" };
        QTest::newRow("(123(123))") << str << '(' << true;
    }
    {
        const std::string str{ "(123(123))(" };
        QTest::newRow("(123(123))(") << str << '(' << false;
    }
    {
        const std::string str{ "))(())" };
        QTest::newRow("))(())") << str << '(' << false;
    }
    {
        const std::string str{ "((()()))" };
        QTest::newRow("((()()))") << str << '(' << true;
    }

    // *** [
    {
        const std::string str{ "[[[x[]][][][]]]" };
        QTest::newRow("[[[x[]][][][]]]") << str << '[' << true;
    }
    {
        const std::string str{ "[][[]" };
        QTest::newRow("[][[]") << str << '[' << false;
    }

    // *** {
    {
        const std::string str{ "{}{}{{{}}}{{}}" };
        QTest::newRow("{}{}{{{}}}{{}}") << str << '{' << true;
    }
    {
        const std::string str{ "List{int}{}" };
        QTest::newRow("List{int}{}") << str << '{' << true;
    }
    {
        const std::string str{ "L{{}}{{" };
        QTest::newRow("L{{}}{{") << str << '{' << false;
    }

    // *** <
    {
        const std::string str{ "List<Number>" };
        QTest::newRow("List<Number>") << str << '<' << true;
    }
    {
        const std::string str{ "List<Pair<int;double>>" };
        QTest::newRow("List<Pair<int;double>>") << str << '<' << true;
    }
    {
        const std::string str{ "List<Pair>>" };
        QTest::newRow("List<Pair>>") << str << '<' << false;
    }
}

void Module::test_str_utils_is_bracket_balance()
{
    QFETCH(std::string, str);
    QFETCH(char, open_br);
    QFETCH(bool, result);

    auto got{ str_utils::is_bracket_balance(str, open_br) };
    QCOMPARE_EQ(got, result);
}

// common read_puml ...
// -----------------------------------------------------------------------

void Module::test_common_read_puml_data(const QString& basic_path)
{
    QTest::addColumn<string>("inn_fpath");
    QTest::addColumn<string>("expect_out_fpath");
    QTest::addColumn<string>("actual_out_fpath");

    // ***

    auto cur_dir{ QDir::current() };
    QCOMPARE_EQ(cur_dir.cdUp(), true);
    QCOMPARE_EQ(cur_dir.cdUp(), true);
    QCOMPARE_EQ(cur_dir.cd(basic_path), true);

    const auto test_catalogs{ cur_dir.entryList(QDir::NoDotAndDotDot | QDir::Dirs) };
    QCOMPARE_EQ(test_catalogs.isEmpty(), false); // no tests - this is a error!
    QCOMPARE_EQ(test_catalogs.contains(".."), false);
    QCOMPARE_EQ(test_catalogs.contains("."), false);

    // ***

    for (qsizetype i = 0; i < test_catalogs.size(); ++i) {
        const std::string tst_name{ "catalog name: /" + basic_path.toStdString() +
                    "/" + test_catalogs[i].toStdString() };

        if (test_catalogs[i].startsWith("_")) {
            qWarning() << "ignore... " << QString::fromStdString(tst_name);
            continue;
        }

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

        QTest::newRow(tst_name.c_str()) << inn_fpath << expect_out_fpath << actual_out_fpath;
    }
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

        QCOMPARE_EQ(uc_edge->id, string("edge_1"));
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

        QCOMPARE_EQ(uc_edge->id, string("edge_1"));
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

        QCOMPARE_EQ(uc_edge->id, string("edge_1"));
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

void Module::test_UseCaseGraph_read_puml_okks_data()
{
    test_common_read_puml_data("converter/test/read_puml/okks/uc_graph");
}

// TODO: сделать общий метод, возможно (?)
void Module::test_UseCaseGraph_read_puml_okks()
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
    QCOMPARE_EQ(robG.uc_node.expired(), true);

    // ...
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
    QCOMPARE_EQ(robG.uc_node.expired(), true);

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

        QCOMPARE_EQ(uc_edge->id, string("edge_1"));
        QCOMPARE_EQ(uc_edge->name, string("one line..."));
        QCOMPARE_EQ(uc_edge->end.lock()->id, string("MainWin"));
        QCOMPARE_EQ(uc_edge->beg.lock()->id, string("User"));
    }
}

void Module::test_RobustnessGraph_read_okk2()
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
    QCOMPARE_EQ(robG.uc_node.expired(), true);

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
            return edge->id == "edge_1";
        }));
        auto uc_edge = static_pointer_cast<RobustnessGraph::RobEdge>(*detected_edge);

        QCOMPARE_EQ(uc_edge->id, string("edge_1"));
        QCOMPARE_EQ(uc_edge->name, string("edge text"));
        QCOMPARE_EQ(uc_edge->beg.lock()->id, string("display_add_student_screen"));
        QCOMPARE_EQ(uc_edge->end.lock()->id, string("MainWin"));
    }
    // ***
    {
        auto detected_edge = robG.edges.begin();
        QVERIFY_THROWS_NO_EXCEPTION(detected_edge = find_if(begin(robG.edges), end(robG.edges),
                                                            [](const shared_ptr<Graph::Edge> edge) {
            return edge->id == "edge_2";
        }));
        auto uc_edge = static_pointer_cast<RobustnessGraph::RobEdge>(*detected_edge);

        QCOMPARE_EQ(uc_edge->id, string("edge_2"));
        QCOMPARE_EQ(uc_edge->name, string("edge text"));
        QCOMPARE_EQ(uc_edge->end.lock()->id, string("display_add_student_screen"));
        QCOMPARE_EQ(uc_edge->beg.lock()->id, string("MainWin"));
    }
}

// -----------------------------------------------------------------------

void Module::test_RobustnessGraph_read_puml_data()
{
    test_common_read_puml_data("converter/test/read_puml/okks/rob_graph");
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


// SequenceGraph
// -----------------------------------------------------------------------

void Module::test_SequenceGraph_read_okk()
{
    using SeqNode = SequenceGraph::SeqNode;
    using SeqEdge = SequenceGraph::SeqEdge;

    SequenceGraph seqG;
    istringstream sin{
        "@startuml\n"
        "\n"
        "actor      Bob\n"
        "boundary   MainWin\n"
        "\n"
        "Bob -> MainWin : on_clicked_menu()\n"
        "Bob <-- MainWin\n"
        "\n"
        "@enduml\n"
    };

    QVERIFY_THROWS_NO_EXCEPTION(seqG.read_puml(sin));
    QCOMPARE_EQ(seqG.nodes.size(), size_t(2));
    QCOMPARE_EQ(seqG.edges.size(), size_t(2));

    QCOMPARE_EQ(seqG.uc_node.expired(), true);
    QCOMPARE_EQ(seqG.stamps.size(), size_t(2));
    QCOMPARE_EQ(seqG.frags.size(), size_t(0));
    QCOMPARE_EQ(seqG.refs.size(), size_t(0));

    // ***

    {
        auto detected_node = seqG.nodes.begin();
        QVERIFY_THROWS_NO_EXCEPTION(detected_node = find_if(begin(seqG.nodes), end(seqG.nodes),
                                                            [](const shared_ptr<Graph::Node> node) {
            return node->id == "Bob";
        }));
        QCOMPARE_EQ((detected_node == seqG.nodes.end()), false);

        auto seq_node = static_pointer_cast<SeqNode>(*detected_node);
        QCOMPARE_EQ(seq_node->id, string("Bob"));
        QCOMPARE_EQ(seq_node->name, string("Bob"));
        QCOMPARE_EQ(seq_node->type, SeqNode::Actor);
        QCOMPARE_EQ(seq_node->is_error, false);
        QCOMPARE_EQ(seq_node->outs.size(), size_t(1));
        QCOMPARE_EQ(seq_node->inns.size(), size_t(1));
    }
    // ***
    {
        auto detected_node = seqG.nodes.begin();
        QVERIFY_THROWS_NO_EXCEPTION(detected_node = find_if(begin(seqG.nodes), end(seqG.nodes),
                                                            [](const shared_ptr<Graph::Node> node) {
            return node->id == "MainWin";
        }));
        QCOMPARE_EQ((detected_node == seqG.nodes.end()), false);

        auto seq_node = static_pointer_cast<SeqNode>(*detected_node);
        QCOMPARE_EQ(seq_node->id, string("MainWin"));
        QCOMPARE_EQ(seq_node->name, string("MainWin"));
        QCOMPARE_EQ(seq_node->type, SeqNode::Boundary);
        QCOMPARE_EQ(seq_node->is_error, false);
        QCOMPARE_EQ(seq_node->outs.size(), size_t(1));
        QCOMPARE_EQ(seq_node->inns.size(), size_t(1));
    }
    // ***
    {
        auto seq_edge = static_pointer_cast<SeqEdge>(seqG.edges[0]);
        QCOMPARE_EQ(seq_edge->id, string("edge_1"));
        QCOMPARE_EQ(seq_edge->name, string("on_clicked_menu()"));
        QCOMPARE_EQ(seq_edge->type, SeqEdge::Sync);
        QCOMPARE_EQ(seq_edge->beg.lock()->id, string("Bob"));
        QCOMPARE_EQ(seq_edge->end.lock()->id, string("MainWin"));
        QCOMPARE_EQ(seq_edge->opd.expired(), true);
    }
    // ***
    {
        auto seq_edge = static_pointer_cast<SeqEdge>(seqG.edges[1]);
        QCOMPARE_EQ(seq_edge->id, string("edge_2"));
        QCOMPARE_EQ(seq_edge->name, string(""));
        QCOMPARE_EQ(seq_edge->type, SeqEdge::Reply);
        QCOMPARE_EQ(seq_edge->beg.lock()->id, string("MainWin"));
        QCOMPARE_EQ(seq_edge->end.lock()->id, string("Bob"));
        QCOMPARE_EQ(seq_edge->opd.expired(), true);
    }
}

void Module::test_SequenceGraph_read_okk1()
{
    // TODO:
}

void Module::test_SequenceGraph_read_okk2()
{
    // TODO:
}

// -----------------------------------------------------------------------

void Module::test_SequenceGraph_read_err()
{
    SequenceGraph seqG;
    istringstream sin{
        "@startuml\n"
        "\n"
        "actor      Bob\n"
        "boundary   MainWin\n"
        "\n"
        "Bob ..> MainWin : on_clicked_menu()\n"
        "Bob <-- MainWin\n"
        "\n"
        "@enduml\n"
    };

    QVERIFY_THROWS_EXCEPTION(GraphError, seqG.read_puml(sin));
}

void Module::test_SequenceGraph_read_err1()
{
    SequenceGraph seqG;
    istringstream sin{
        "@startuml\n"
        "\n"
        "actor      Bob\n"
        "boundary   MainWin\n"
        "\n"
        "Bob <--> MainWin : on_clicked_menu()\n"
        "Bob <-- MainWin\n"
        "\n"
        "@enduml\n"
    };

    QVERIFY_THROWS_EXCEPTION(GraphError, seqG.read_puml(sin));
}

void Module::test_SequenceGraph_read_err2()
{
    SequenceGraph seqG;
    istringstream sin{
        "@startuml\n"
        "\n"
        "actor      Bob\n"
        "boundary   MainWin\n"
        "\n"
        "Bob -- MainWin : on_clicked_menu()\n"
        "Bob <-- MainWin\n"
        "\n"
        "@enduml\n"
    };

    QVERIFY_THROWS_EXCEPTION(GraphError, seqG.read_puml(sin));
}

void Module::test_SequenceGraph_read_err3()
{
    SequenceGraph seqG;
    istringstream sin{
        "@startuml\n"
        "\n"
        "skinparam dpi 200\n"
        "\n"
        "actor Alice \n"
        "actor Bob   \n"
        "\n"
        "ref over Alice, Bob, Log\n"
        "Инициализация\n"
        "end ref\n"
        "\n"
        "@enduml\n"
    };

    QVERIFY_THROWS_EXCEPTION(GraphError, seqG.read_puml(sin));
}

void Module::test_SequenceGraph_read_err4()
{
    SequenceGraph seqG;
    istringstream sin{
        "@startuml\n"
        "\n"
        "participant First\n"
        "participant Middle\n"
        "\n"
        "loop i < 30\n"
        "First--> Middle : some()\n"
        "else\n"
        "First --> Middle : some2()\n"
        "end\n"
        "\n"
        "@enduml\n"
    };

    QVERIFY_THROWS_EXCEPTION(GraphError, seqG.read_puml(sin));
}

// -----------------------------------------------------------------------

void Module::test_SequenceGraph_read_puml_data()
{
    test_common_read_puml_data("converter/test/read_puml/okks/seq_graph");
}

void Module::test_SequenceGraph_read_puml()
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

    SequenceGraph seq_graph;
    QVERIFY_THROWS_NO_EXCEPTION(seq_graph.read_puml(fin_inn));
    QVERIFY_THROWS_NO_EXCEPTION(seq_graph.write_json(fout_actual));
    QVERIFY_THROWS_NO_EXCEPTION(seq_graph.write_json(siout_actual));

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
    using Member = ClassGraph::ClassNode::Member;
    using ClassNode = ClassGraph::ClassNode;

    ClassGraph classG;
    istringstream sin{
        "@startuml\n"
        "\n"
        "class Foo{\n"
        "   +val : Number\n"
        "   -val1 : String\n"
        "}\n"
        "\n"
        "class Bar{\n"
        "\n"
        "}\n"
        "@enduml"
    };

    QVERIFY_THROWS_NO_EXCEPTION(classG.read_puml(sin));
    QCOMPARE_EQ(classG.nodes.size(), size_t(2));
    QCOMPARE_EQ(classG.edges.size(), size_t(0));

    // ***

    {
        auto detected_node = classG.nodes.begin();
        QVERIFY_THROWS_NO_EXCEPTION(detected_node = find_if(begin(classG.nodes), end(classG.nodes),
                                                            [](const shared_ptr<Graph::Node> node) {
            return node->id == "Foo";
        }));
        QCOMPARE_EQ((detected_node == classG.nodes.end()), false);

        auto class_node = static_pointer_cast<ClassNode>(*detected_node);
        QCOMPARE_EQ(class_node->id, string("Foo"));
        QCOMPARE_EQ(class_node->name, string("Foo"));
        QCOMPARE_EQ(class_node->outs.size(), size_t(0));
        QCOMPARE_EQ(class_node->inns.size(), size_t(0));
        QCOMPARE_EQ(class_node->type, ClassNode::Class);

        QCOMPARE_EQ(class_node->datas.size(), size_t(2));
        QCOMPARE_EQ(class_node->datas[0].mark, Member::Public);
        QCOMPARE_EQ(class_node->datas[0].name, "val");
        QCOMPARE_EQ(class_node->datas[0].type, "Number");
        QCOMPARE_EQ(class_node->datas[0].param_types.size(), size_t(0));

        QCOMPARE_EQ(class_node->datas[1].mark, Member::Private);
        QCOMPARE_EQ(class_node->datas[1].name, "val1");
        QCOMPARE_EQ(class_node->datas[1].type, "String");
        QCOMPARE_EQ(class_node->datas[1].param_types.size(), size_t(0));

        QCOMPARE_EQ(class_node->funcs.size(), size_t(0));
        QCOMPARE_EQ(class_node->enum_values.size(), size_t(0));
    }
    // ***
    {
        auto detected_node = classG.nodes.begin();
        QVERIFY_THROWS_NO_EXCEPTION(detected_node = find_if(begin(classG.nodes), end(classG.nodes),
                                                            [](const shared_ptr<Graph::Node> node) {
            return node->id == "Bar";
        }));
        QCOMPARE_EQ((detected_node == classG.nodes.end()), false);

        auto class_node = static_pointer_cast<ClassNode>(*detected_node);
        QCOMPARE_EQ(class_node->id, string("Bar"));
        QCOMPARE_EQ(class_node->name, string("Bar"));
        QCOMPARE_EQ(class_node->outs.size(), size_t(0));
        QCOMPARE_EQ(class_node->inns.size(), size_t(0));
        QCOMPARE_EQ(class_node->type, ClassNode::Class);

        QCOMPARE_EQ(class_node->datas.size(), size_t(0));
        QCOMPARE_EQ(class_node->funcs.size(), size_t(0));
        QCOMPARE_EQ(class_node->enum_values.size(), size_t(0));
    }
}

void Module::test_ClassGraph_read_okk1()
{
    using Member = ClassGraph::ClassNode::Member;
    using ClassNode = ClassGraph::ClassNode;

    ClassGraph classG;
    istringstream sin{
        "@startuml\n"
        "\n"
        "class Foo{\n"
        "   +fn(Int, Double) : String\n"
        "   #fn2() : void\n"
        "}\n"
        "\n"
        "@enduml"
    };

    QVERIFY_THROWS_NO_EXCEPTION(classG.read_puml(sin));
    QCOMPARE_EQ(classG.nodes.size(), size_t(1));
    QCOMPARE_EQ(classG.edges.size(), size_t(0));

    // ***

    {
        auto detected_node = classG.nodes.begin();
        QVERIFY_THROWS_NO_EXCEPTION(detected_node = find_if(begin(classG.nodes), end(classG.nodes),
                                                            [](const shared_ptr<Graph::Node> node) {
            return node->id == "Foo";
        }));
        QCOMPARE_EQ((detected_node == classG.nodes.end()), false);

        auto class_node = static_pointer_cast<ClassNode>(*detected_node);
        QCOMPARE_EQ(class_node->id, string("Foo"));
        QCOMPARE_EQ(class_node->name, string("Foo"));
        QCOMPARE_EQ(class_node->outs.size(), size_t(0));
        QCOMPARE_EQ(class_node->inns.size(), size_t(0));
        QCOMPARE_EQ(class_node->type, ClassNode::Class);

        QCOMPARE_EQ(class_node->funcs.size(), size_t(2));
        QCOMPARE_EQ(class_node->funcs[0].mark, Member::Public);
        QCOMPARE_EQ(class_node->funcs[0].name, "fn");
        QCOMPARE_EQ(class_node->funcs[0].type, "String");

        QCOMPARE_EQ(class_node->funcs[0].param_types.size(), size_t(2));
        QCOMPARE_EQ(class_node->funcs[0].param_types[0], "Int");
        QCOMPARE_EQ(class_node->funcs[0].param_types[1], "Double");

        QCOMPARE_EQ(class_node->funcs[1].mark, Member::Protected);
        QCOMPARE_EQ(class_node->funcs[1].name, "fn2");
        QCOMPARE_EQ(class_node->funcs[1].type, "void");
        QCOMPARE_EQ(class_node->funcs[1].param_types.size(), size_t(0));

        QCOMPARE_EQ(class_node->datas.size(), size_t(0));
        QCOMPARE_EQ(class_node->enum_values.size(), size_t(0));
    }
}

void Module::test_ClassGraph_read_okk2()
{
    using Member = ClassGraph::ClassNode::Member;
    using ClassNode = ClassGraph::ClassNode;

    ClassGraph classG;
    istringstream sin{
        "@startuml\n"
        "\n"
        "class Foo{\n"
        "   +fn(Int, Double) : List<String>\n"
        "   #fn2() : Number\n"
        "   -fn3(List<Number>, String) : None\n"
        "}\n"
        "\n"
        "@enduml"
    };

    QVERIFY_THROWS_NO_EXCEPTION(classG.read_puml(sin));
    QCOMPARE_EQ(classG.nodes.size(), size_t(1));
    QCOMPARE_EQ(classG.edges.size(), size_t(0));

    // ***

    {
        auto detected_node = classG.nodes.begin();
        QVERIFY_THROWS_NO_EXCEPTION(detected_node = find_if(begin(classG.nodes), end(classG.nodes),
                                                            [](const shared_ptr<Graph::Node> node) {
            return node->id == "Foo";
        }));
        QCOMPARE_EQ((detected_node == classG.nodes.end()), false);

        auto class_node = static_pointer_cast<ClassNode>(*detected_node);
        QCOMPARE_EQ(class_node->id, string("Foo"));
        QCOMPARE_EQ(class_node->name, string("Foo"));
        QCOMPARE_EQ(class_node->outs.size(), size_t(0));
        QCOMPARE_EQ(class_node->inns.size(), size_t(0));
        QCOMPARE_EQ(class_node->type, ClassNode::Class);

        QCOMPARE_EQ(class_node->funcs.size(), size_t(3));
        QCOMPARE_EQ(class_node->funcs[0].mark, Member::Public);
        QCOMPARE_EQ(class_node->funcs[0].name, "fn");
        QCOMPARE_EQ(class_node->funcs[0].type, "List<String>");
        QCOMPARE_EQ(class_node->funcs[0].param_types.size(), size_t(2));
        QCOMPARE_EQ(class_node->funcs[0].param_types[0], "Int");
        QCOMPARE_EQ(class_node->funcs[0].param_types[1], "Double");

        QCOMPARE_EQ(class_node->funcs[1].mark, Member::Protected);
        QCOMPARE_EQ(class_node->funcs[1].name, "fn2");
        QCOMPARE_EQ(class_node->funcs[1].type, "Number");
        QCOMPARE_EQ(class_node->funcs[1].param_types.size(), size_t(0));

        QCOMPARE_EQ(class_node->funcs[2].mark, Member::Private);
        QCOMPARE_EQ(class_node->funcs[2].name, "fn3");
        QCOMPARE_EQ(class_node->funcs[2].type, "None");
        QCOMPARE_EQ(class_node->funcs[2].param_types.size(), size_t(2));
        QCOMPARE_EQ(class_node->funcs[2].param_types[0], "List<Number>");
        QCOMPARE_EQ(class_node->funcs[2].param_types[1], "String");

        QCOMPARE_EQ(class_node->datas.size(), size_t(0));
        QCOMPARE_EQ(class_node->enum_values.size(), size_t(0));
    }
}

void Module::test_ClassGraph_read_okk3()
{
    //using Member = ClassGraph::ClassNode::Member;
    //using ClassNode = ClassGraph::ClassNode;

    ClassGraph classG;
    istringstream sin{
        "@startuml\n"
        "\n"
        "class Foo{\n"
        "}\n"
        "\n"
        "Foo --> Bar\n"
        "\n"
        "@enduml"
    };

    QVERIFY_THROWS_NO_EXCEPTION(classG.read_puml(sin));
    QCOMPARE_EQ(classG.nodes.size(), size_t(2));
    QCOMPARE_EQ(classG.edges.size(), size_t(1));

    // TODO:
}

void Module::test_ClassGraph_read_okk4()
{
    // TODO:
}

// -----------------------------------------------------------------------

void Module::test_ClassGraph_read_puml_data()
{
    test_common_read_puml_data("converter/test/read_puml/okks/class_graph");
}

void Module::test_ClassGraph_read_puml()
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

    ClassGraph class_graph;
    QVERIFY_THROWS_NO_EXCEPTION(class_graph.read_puml(fin_inn));
    QVERIFY_THROWS_NO_EXCEPTION(class_graph.write_json(fout_actual));
    QVERIFY_THROWS_NO_EXCEPTION(class_graph.write_json(siout_actual));

    // ***

    json actual;
    QVERIFY_THROWS_NO_EXCEPTION(siout_actual >> actual);

    json expected;
    QVERIFY_THROWS_NO_EXCEPTION(fin_expect >> expected);

    QCOMPARE_EQ(actual.is_object(), true);
    QCOMPARE_EQ(expected.is_object(), true);
    QCOMPARE_EQ((actual == expected), true);
}


// LayoutFlowGraph
// -----------------------------------------------------------------------
void Module::test_LayoutFlowGraph_read_puml_data()
{
    test_common_read_puml_data("converter/test/read_puml/okks/lw_graph");
}

void Module::test_LayoutFlowGraph_read_puml()
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

    LayoutFlowGraph lw_graph;
    QVERIFY_THROWS_NO_EXCEPTION(lw_graph.read_puml(fin_inn));
    QVERIFY_THROWS_NO_EXCEPTION(lw_graph.write_json(fout_actual));
    QVERIFY_THROWS_NO_EXCEPTION(lw_graph.write_json(siout_actual));

    // ***

    json actual;
    QVERIFY_THROWS_NO_EXCEPTION(siout_actual >> actual);

    json expected;
    QVERIFY_THROWS_NO_EXCEPTION(fin_expect >> expected);

    QCOMPARE_EQ(actual.is_object(), true);
    QCOMPARE_EQ(expected.is_object(), true);
    QCOMPARE_EQ((actual == expected), true);
}

QTEST_APPLESS_MAIN(Module)
