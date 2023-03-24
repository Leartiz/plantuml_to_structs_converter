#include <iostream>
#include <memory>
#include <string>
#include <fstream>

#include "structures/use_case/uc_node.h"
#include "structures/use_case/uc_edge.h"
#include "structures/use_case/use_case_dia.h"

#include "translator/direct_translator.h"

void uc_visual_test_0()
{
    /* the first step is to simply create the nodes */
    lenv::UC_node::Builder beg_uc_node_b("Student");
    auto beg_uc_node{ beg_uc_node_b.name("Student")
                .type(lenv::UC_node::ACTOR)
                .build_ptr() };

    lenv::UC_node::Builder end_uc_node_b("Registration");
    auto end_uc_node{ end_uc_node_b.name("Registration")
                .type(lenv::UC_node::USE_CASE)
                .build_ptr() };

    /* the second step is to fully create edges */
    lenv::UC_edge::Builder uc_edge_b("1");
    auto uc_edge = uc_edge_b.type(lenv::UC_edge::ASSOCIATION)
            .beg(beg_uc_node)
            .end(end_uc_node)
            .build_ptr();

    /* the third step is to complete the nodes with edges */
    lenv::UC_node::Adder beg_uc_node_a{ beg_uc_node };
    beg_uc_node_a.add_out_edge(uc_edge);

    lenv::UC_node::Adder end_uc_node_a{ end_uc_node };
    end_uc_node_a.add_inn_edge(uc_edge);    

    /* out whole json */
    std::cout << "beg_uc_node.whole_json: " << beg_uc_node->to_whole_json() << std::endl;
    std::cout << "end_uc_node.whole_json: " << end_uc_node->to_whole_json() << std::endl;
    std::cout << "uc_edge.whole_json: " << uc_edge->to_whole_json() << std::endl;

    /* out short json */
    std::cout << "beg_uc_node.short_json: " << beg_uc_node->to_short_json() << std::endl;
    std::cout << "end_uc_node.short_json: " << end_uc_node->to_short_json() << std::endl;
    std::cout << "uc_edge.short_json: " << uc_edge->to_short_json() << std::endl;

    /* out valid */
    std::cout << "beg_uc_node.is_valid: " << beg_uc_node->is_valid() << std::endl;
    std::cout << "end_uc_node.is_valid: " << end_uc_node->is_valid() << std::endl;
    std::cout << "uc_edge.is_valid: " << uc_edge->is_valid() << std::endl;
}

void uc_visual_test_1()
{
    lenv::Use_Case_dia dia;
    std::cout << dia.to_whole_json() << std::endl;
}

int main()
{
    uc_visual_test_0();
    uc_visual_test_1();



    return 0;
}
