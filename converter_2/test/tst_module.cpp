#include <sstream>

#include "tst_module.h"

#include "usecasegraph.h"
#include "classgraph.h"

using namespace std;

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

void Module::test_UseCaseGraph_read_puml_data()
{

}

void Module::test_UseCaseGraph_read_puml()
{

}

QTEST_APPLESS_MAIN(Module)
