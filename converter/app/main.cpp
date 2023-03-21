#include <iostream>
#include <memory>
#include <string>
#include <fstream>

#include "structures/use_case/uc_node.h"
#include "structures/use_case/uc_edge.h"


int main()
{
    std::shared_ptr<lenv::UC_edge> edge = std::make_shared<lenv::UC_edge>(
                lenv::UC_edge{ "1", lenv::UC_edge::ASSOCIATION }
                );
    lenv::UC_node node{ lenv::UC_node::USE_CASE, "dsd" };
    node.m_inn_edges.push_back(edge);


    std::cout << node.to_whole_json() << std::endl;



    return 0;
}
