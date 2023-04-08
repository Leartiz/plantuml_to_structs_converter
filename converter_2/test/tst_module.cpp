#include <sstream>
#include <fstream>
#include <algorithm>

#include "tst_module.h"

#include "usecasegraph.h"
#include "classgraph.h"
#include "grapherror.h"

#include "nlohmann/json.hpp"

#if QT_VERSION <= QT_VERSION_CHECK(6, 4, 0)
    #define QCOMPARE_EQ(lhs, rhs) QCOMPARE(lhs == rhs, true);
    #define QCOMPARE_NE(lhs, rhs) QCOMPARE(lhs == rhs, false);

    #define QVERIFY_THROWS_EXCEPTION(exceptiontype, expression) QVERIFY_EXCEPTION_THROWN(expression, exceptiontype);
    #define QVERIFY_THROWS_NO_EXCEPTION(expression) static_cast<void>(expression);
#endif

using namespace std;
using namespace nlohmann;

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
        QCOMPARE_EQ(detected_node == ucg.nodes.end(), false);

        auto uc_node = static_pointer_cast<UseCaseGraph::UcNode>(*detected_node);
        QCOMPARE_EQ(uc_node->id, string("UC1"));
        QCOMPARE_EQ(uc_node->name, string("UC1"));
        QCOMPARE_EQ(uc_node->inns.size(), size_t(1));
        QCOMPARE_EQ(uc_node->type, UseCaseGraph::UcNode::Actor);
    }
    // ***
    {
        auto detected_node = ucg.nodes.begin();
        QVERIFY_THROWS_NO_EXCEPTION(detected_node = find_if(begin(ucg.nodes), end(ucg.nodes),
                                                            [](const shared_ptr<Graph::Node> node) {
            return node->id == "A1";
        }));
        QCOMPARE_EQ(detected_node == ucg.nodes.end(), false);

        auto uc_node = static_pointer_cast<UseCaseGraph::UcNode>(*detected_node);
        QCOMPARE_EQ(uc_node->id, string("A1"));
        QCOMPARE_EQ(uc_node->name, string("A1"));
        QCOMPARE_EQ(uc_node->outs.size(), size_t(1));
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
        QCOMPARE_EQ(detected_node == ucg.nodes.end(), false);

        auto uc_node = static_pointer_cast<UseCaseGraph::UcNode>(*detected_node);
        QCOMPARE_EQ(uc_node->id, string("UC1"));
        QCOMPARE_EQ(uc_node->name, string("Последний прецедент"));
        QCOMPARE_EQ(uc_node->outs.size(), size_t(1));
        QCOMPARE_EQ(uc_node->type, UseCaseGraph::UcNode::UseCase);
    }
    // ***
    {
        auto detected_node = ucg.nodes.begin();
        QVERIFY_THROWS_NO_EXCEPTION(detected_node = find_if(begin(ucg.nodes), end(ucg.nodes),
                                                            [](const shared_ptr<Graph::Node> node) {
            return node->id == "A1";
        }));
        QCOMPARE_EQ(detected_node == ucg.nodes.end(), false);

        auto uc_node = static_pointer_cast<UseCaseGraph::UcNode>(*detected_node);
        QCOMPARE_EQ(uc_node->id, string("A1"));
        QCOMPARE_EQ(uc_node->name, string("A1"));
        QCOMPARE_EQ(uc_node->inns.size(), size_t(1));
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
        QCOMPARE_EQ(detected_node == ucg.nodes.end(), false);

        auto uc_node = static_pointer_cast<UseCaseGraph::UcNode>(*detected_node);
        QCOMPARE_EQ(uc_node->id, string("UC1"));
        QCOMPARE_EQ(uc_node->name, string("Last precedent"));
        QCOMPARE_EQ(uc_node->inns.size(), size_t(1));
        QCOMPARE_EQ(uc_node->type, UseCaseGraph::UcNode::UseCase);
    }
    // ***
    {
        auto detected_node = ucg.nodes.begin();
        QVERIFY_THROWS_NO_EXCEPTION(detected_node = find_if(begin(ucg.nodes), end(ucg.nodes),
                                                            [](const shared_ptr<Graph::Node> node) {
            return node->id == "A1";
        }));
        QCOMPARE_EQ(detected_node == ucg.nodes.end(), false);

        auto uc_node = static_pointer_cast<UseCaseGraph::UcNode>(*detected_node);
        QCOMPARE_EQ(uc_node->id, string("A1"));
        QCOMPARE_EQ(uc_node->name, string("Last actor"));
        QCOMPARE_EQ(uc_node->outs.size(), size_t(1));
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
    QCOMPARE_EQ(actual == expected, true);
}

QTEST_APPLESS_MAIN(Module)
