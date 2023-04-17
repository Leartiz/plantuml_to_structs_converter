#include <regex>

#include "graph.h"
#include "grapherror.h"
#include "constructhelper.h"

using namespace std;

Graph::Graph() : m_ch{ make_shared<ConstructHelper>() } {}

bool Graph::try_whitespaces(string& line) {
    static const regex rx{ "^\\s*$" };
    return regex_match(line, rx);
}

bool Graph::try_directive(string& line) {
    static const regex rx{ "^\\s*(@startuml|@enduml)\\s*$" };
    return regex_match(line, rx);
}

bool Graph::try_skinparam(string& line) {
    static const regex rx{ "^\\s*skinparam\\s+\\S+\\s+\\S+\\s*$" };
    return regex_match(line, rx);
}

bool Graph::try_direction(string& line) {
    static const regex rx{ "^\\s*((left\\s+to\\s+right\\s+direction)|(top\\s+to\\s+bottom\\s+direction))\\s*$" };
    return regex_match(line, rx);
}

bool Graph::try_beg_grouping(string& line) {
    static const regex rx{ "^\\s*(rectangle|package)(\\s+((\\S+)|(\\\".+\\\")))?\\s*\\{\\s*$" };
    return regex_match(line, rx);
}

bool Graph::try_end_curly_brace(string& line) {
    static const regex rx{ "^\\s*\\}\\s*$" };
    return regex_match(line, rx);
}

bool Graph::try_one_note(string& line) {
    smatch match;
    static const regex rx{ "^\\s*note\\s+(left|right|top|bottom)\\s*(\\s+of\\s+(\\S+))?\\s*:(.+)$" };
    if (!regex_match(line, match, rx)) {
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

bool Graph::try_beg_note_with_id(std::string& line) {
    smatch match;
    static const regex rx{ "^\\s*note\\s+as\\s+(\\S+)\\s*$" };
    if (!regex_match(line, match, rx)) {
        return false;
    }

    return true; // TODO: нужна ли вообще такая заметка?
}

bool Graph::try_end_multi_note(string& line) {
    static const regex rx{ "^\\s*end\\s+note\\s*$" };
    if (!regex_match(line, rx)) {
        return false;
    }
    return true;
}

bool Graph::try_note(string& line, istream& in) {
    if (try_one_note(line)) {
        return true;
    }

    if (!try_beg_multi_note(line) && !try_beg_note_with_id(line)) {
        return false;
    }

    bool is_closed{ false };
    while (in) {
        string line;
        getline(in, line);
        m_ch->line_number++;

        if (try_end_multi_note(line)) {
            is_closed = true;
            break;
        }
    }

    if (!is_closed) {
        throw GraphError(m_ch->line_number, "group is not closed");
    }

    return true;
}

// -----------------------------------------------------------------------

void Graph::read_puml(std::istream& in) {
    m_ch->reset();

    while (in) {
        string line;
        getline(in, line);
        m_ch->line_number++;

        if (
                !try_node(line, in) &&
                !try_connection(line, in) &&
                !try_whitespaces(line) &&
                !try_grouping(line, in) &&

                !try_note(line, in) &&
                !try_directive(line) &&
                !try_skinparam(line) &&
                !try_direction(line)) {
            throw GraphError(m_ch->line_number, "unknown line");
        }
    }

    nodes = m_ch->to_nodes();
    edges = m_ch->to_edges();
}
