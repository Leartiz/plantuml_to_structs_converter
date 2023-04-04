#include <map>
#include <regex>
#include <iostream>
#include <stdexcept>

#include "graphclass.h"

// [+-#] Name : Type
GraphClass::Member GraphClass::Member::from_str(string& line) {
    std::smatch match;

    if (!std::regex_match (line, match, std::regex("\\s*([+#-]?)\\s*(\\S+)\\s*:\\s*(\\S+)")))
        throw std::runtime_error("member not found");

    Member field;
    if (match[1].str().empty()) {
        field.mark = Private;
    }
    else {
        switch (match[1].str()[0]) {
        case '#':
            field.mark = Protected;
            break;
        case '-':
            field.mark = Private;
            break;
        case '+':
            field.mark = Public;
            break;
        default:
            throw std::runtime_error("unknown mark");
        }
    }

    field.name = match[2];

    // TODO: types

    return field;
}

// Edge: A --> B
// Node: class Name {}
void GraphClass::read(istream& in) {
    stringstream stream_copy;
    stream_copy << in.rdbuf();
    string text{ stream_copy.str() };

    map<string, shared_ptr<NodeClass>> name_node;

    while (stream_copy) {
        string line;
        getline(stream_copy, line);

        std::smatch match;
        if (!regex_match(line, match, regex("\\s*class\\s+(\\S+)\\s*\\{\\s*"))) {
            if (is_allowed_line(line))
                continue;
        }

        auto node = make_shared<NodeClass>();
        node->id = node->name = match[1];

        while (stream_copy) {
            getline(stream_copy, line);
            if (!regex_match(line, match, regex("\\s*\\}\\s*")))
                node->datas.push_back(Member::from_str(line)); // throw!
            break;
        }

        nodes.push_back(node);
        if (name_node.count(node->id))
            throw std::runtime_error("node already defined");

        name_node[node->id] = node;
    }

    stream_copy.clear();
    stream_copy << text;
    while (stream_copy) {

        // A <|-- B
        // A <|- B

        // В другую строну

        string line;
        std::getline(stream_copy, line);

        std::smatch match;
        if (false == std::regex_match (line, match, std::regex("(\\S+)\\s*<|--*\\s+(\\S+)")))
            continue;

        //TODO: проверить что начало и конец созданы (есть в мапе)

        auto edge = shared_ptr<EdgeClass>();

        edge->beg = name_node[match[1]];
        edge->end = name_node[match[2]];

        edges.push_back(edge);
    }
}

bool GraphClass::is_allowed_line(string&)
{
    return true;
}

void GraphClass::write(ostream& sstr)
{

}
