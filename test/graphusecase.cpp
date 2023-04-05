#include <map>
#include <regex>
#include <stdexcept>
#include <iostream>

#include "graphusecase.h"

GraphUseCase::NodeUseCase::NodeUseCase(string id, string name) {
    this->id = std::move(id);
    this->name = std::move(name);
}

// -----------------------------------------------------------------------

namespace {

map<string, shared_ptr<GraphUseCase::NodeUseCase>> id_and_node{};
map<string, shared_ptr<GraphUseCase::EdgeUseCase>> id_and_edge{};

}

void GraphUseCase::read_puml(istream& in) {
    stringstream stream_copy;
    stream_copy << in.rdbuf();
    auto all_text{ stream_copy.str() };

    while (stream_copy) {
        string line;
        getline(stream_copy, line);

        if (
                !try_directive(line) &&
                !try_actor_node(line) &&
                !try_usecase_node(line) &&
                !try_connection(line) &&
                !try_whitespaces(line)) {
            throw std::runtime_error("unknown line");
        }
    }

    id_and_node.clear();
}

void GraphUseCase::write_json(ostream& out) {

}

// -----------------------------------------------------------------------

bool GraphUseCase::try_actor_node(string& line) {
    std::smatch match;
    if (!regex_match(line, match, regex("^\\s*actor\\s*(:.+:|\".+\")\\s*as\\s*(\\S*)\\s*$"))) {
        return false;
    }

    auto node = make_shared<NodeUseCase>(match[2].str(), match[1].str());
    node->type = NodeUseCase::Actor;

    id_and_node[node->id] = node;
    return true;
}

bool GraphUseCase::try_usecase_node(string& line) {
    std::smatch match;
    if (!regex_match(line, match, regex("^\\s*usecase\\s*(\\(.+\\)|\\\".+\\\")\\s*as\\s*(\\S*)\\s*$"))) {
        return false;
    }

    auto node = make_shared<NodeUseCase>(match[2].str(), match[1].str());
    node->type = NodeUseCase::UseCase;

    id_and_node[node->id] = node;
    return true;
}

bool GraphUseCase::try_connection(string& line)
{
    std::smatch match;
    if (!regex_match(line, match, regex("^\\s*(\\S*)\\s+((<|<\\|)?([-\\.]+)(>|\\|>)?)\\s+(\\S*)\\s*(:\\s*(<<(include|extend)>>))?$"))) {
        return false;
    }

    const auto lname = match[1].str();
    if (id_and_node.count(lname) == 0) {
        auto node = make_shared<NodeUseCase>(lname, lname);
        node->type = NodeUseCase::Actor;
        id_and_node.insert({ lname, node });
    }

    const auto rname = match[6].str();
    if (id_and_node.count(rname) == 0) {
        auto node = make_shared<NodeUseCase>(rname, rname);
        node->type = NodeUseCase::Actor;
        id_and_node.insert({ rname, node });
    }

    auto lhead = match[3].str();

    for (auto one : match) {
        std::cout << one.str() << std::endl;
    }


    return true;
}


