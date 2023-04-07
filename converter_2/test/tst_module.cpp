#include <sstream>
#include <algorithm>

#include "tst_module.h"

#include "usecasegraph.h"
#include "classgraph.h"

#if QT_VERSION <= QT_VERSION_CHECK(6, 4, 0)
    #define QCOMPARE_EQ(lhs, rhs) QCOMPARE(lhs == rhs, true);
    #define QCOMPARE_NE(lhs, rhs) QCOMPARE(lhs == rhs, false);

    #define QVERIFY_THROWS_EXCEPTION(exceptiontype, expression) QVERIFY_EXCEPTION_THROWN(expression, exceptiontype);
    #define QVERIFY_THROWS_NO_EXCEPTION(expression) static_cast<void>(expression);
#endif

using namespace std;

namespace {

}

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
        "  usecase   (Последний прецедент)   as   UC1  \n"
        "  actor :Последний актор: as A1   \n"
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
        QCOMPARE_EQ(uc_node->name, string("Последний прецедент"));
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
        QCOMPARE_EQ(uc_node->name, string("Последний актор"));
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

}

void Module::test_UseCaseGraph_read_okks()
{

}

// -----------------------------------------------------------------------

void Module::test_UseCaseGraph_read_puml_data()
{

}

void Module::test_UseCaseGraph_read_puml()
{

}

QTEST_APPLESS_MAIN(Module)
