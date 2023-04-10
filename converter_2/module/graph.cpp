#include <regex>

#include "graph.h"
#include "grapherror.h"
#include "constructhelper.h"

using namespace std;

Graph::Graph() : m_ch{ make_shared<ConstructHelper>() } {}

bool Graph::try_whitespaces(string& line) {
    smatch match;
    return regex_match(line, match, regex("^\\s*$"));
}

bool Graph::try_directive(string& line) {
    smatch match;
    return regex_match(line, match, regex("^\\s*(@startuml|@enduml)\\s*$"));
}

bool Graph::try_skinparam(string& line) {
    smatch match;
    return regex_match(line, match, regex("^\\s*skinparam\\s+\\S+\\s+\\S+\\s*$"));
}

bool Graph::try_direction(string& line) {
    smatch match;
    return regex_match(line, match, regex("^\\s*((left\\s+to\\s+right\\s+direction)|(top\\s+to\\s+bottom\\s+direction))\\s*$"));
}

bool Graph::try_beg_grouping(string& line) {
    smatch match;
    return regex_match(line, match, regex("^\\s*(rectangle|package)(\\s+((\\S+)|(\\\".+\\\")))?\\s*\\{\\s*$"));
}

bool Graph::try_end_curly_brace(string& line) {
    smatch match;
    return regex_match(line, match, regex("^\\s*\\}\\s*$"));
}

bool Graph::try_one_note(string& line) {
    smatch match;
    if (!regex_match(line, match, regex("^\\s*note\\s+(left|right|top|bottom)\\s*(\\s+of\\s+(\\S+))?\\s*:(.+)$"))) {
        return false;
    }

    const auto node_name{ match[2].str() };
    if (!m_ch->id_node.count(node_name)) {
        throw GraphError(m_ch->line_number, "node not defined");
    }
    return true;
}

bool Graph::try_beg_multi_note(string&) {
    return false; // TODO: многострочная заметка?
}

bool try_beg_note_with_id(std::string&) {
    return false;
}

bool Graph::try_end_multi_note(string&) {
    return false;
}

// -----------------------------------------------------------------------

void Graph::read_puml(std::istream& in) {
    m_ch->reset();

    while (in) {
        string line;
        getline(in, line);
        m_ch->line_number++;

        if (
                !try_node(line) &&
                !try_connection(line) &&
                !try_whitespaces(line) &&
                !try_grouping(line, in) &&

                !try_one_note(line) &&
                !try_directive(line) &&
                !try_skinparam(line) &&
                !try_direction(line)) {
            throw GraphError(m_ch->line_number, "unknown line");
        }
    }

    nodes = m_ch->to_nodes();
    edges = m_ch->to_edges();
}
