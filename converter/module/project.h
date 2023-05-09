#ifndef PROJECT_H
#define PROJECT_H

#include <map>
#include <memory>
#include <string>

#include "usecasegraph.h"
#include "robustnessgraph.h"
#include "sequencegraph.h"
#include "classgraph.h"

#include "extra_reqs.h"
#include "layoutflowgraph.h"


struct Project final
{
public:
    using UsecaseId = std::string;
    using BoundaryId = std::string;
    using EntityId = std::string;

public:
    void read_project(const std::string& project_root_path);

public:
    std::shared_ptr<UseCaseGraph> uc_graph;
    std::map<UsecaseId, std::shared_ptr<RobustnessGraph>> rob_graphs;
    std::map<UsecaseId, std::shared_ptr<SequenceGraph>> seq_graphs;
    std::shared_ptr<ClassGraph> class_graph;

public:
    std::shared_ptr<LayoutFlowGraph> lw_graph;
    std::map<UsecaseId, std::shared_ptr<Desc>> descs;
    std::map<BoundaryId, std::shared_ptr<Layout>> layouts;
    std::map<EntityId, std::shared_ptr<Format>> formats;

};

#endif // PROJECT_H
