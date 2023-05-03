#include <regex>

#include "graph.h"
#include "grapherror.h"
#include "constructhelper.h"

using namespace std;

Graph::Graph() : m_ch{ make_shared<ConstructHelper>() } {}

string Graph::read_line(istream& in) {
    string line;
    getline(in, line);
    m_ch->line_number++;
    return line;
}

void Graph::read_puml(std::istream& in) {
    m_ch->reset();

    while (in) {
        const auto line{ read_line(in) };
        if (!try_any(line, in)) {
            throw GraphError(m_ch->line_number, "unknown line");
        }
    }

    nodes = m_ch->to_nodes();
    edges = m_ch->to_edges();
}

// -----------------------------------------------------------------------

bool Graph::try_directive(const string& line) const {
    static const regex rx{ "^\\s*(@startuml|@enduml)\\s*$" };
    return regex_match(line, rx);
}

bool Graph::try_skinparam(const string& line) const {
    static const regex rx{ "^\\s*skinparam\\s+\\S+\\s+\\S+\\s*$" };
    return regex_match(line, rx);
}

bool Graph::try_direction(const string& line) const {
    static const regex rx{ "^\\s*((left\\s+to\\s+right\\s+direction)|(top\\s+to\\s+bottom\\s+direction))\\s*$" };
    return regex_match(line, rx);
}

bool Graph::try_whitespaces(const string& line) const {
    static const regex rx{ "^\\s*$" };
    return regex_match(line, rx);
}

// -----------------------------------------------------------------------

bool Graph::try_beg_grouping(const string& line) const {
    static const regex rx{ "^\\s*(rectangle|package)(\\s+((\\S+)|(\\\".+\\\")))?\\s*\\{\\s*$" };
    return regex_match(line, rx);
}

bool Graph::try_end_curly_brace(const string& line) const {
    static const regex rx{ "^\\s*\\}\\s*$" };
    return regex_match(line, rx);
}

// -----------------------------------------------------------------------

bool Graph::try_one_note(const string& line) const {
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

bool Graph::try_beg_note_with_id(const string& line) const {
    smatch match;
    static const regex rx{ "^\\s*note\\s+as\\s+(\\S+)\\s*$" };
    if (!regex_match(line, match, rx)) {
        return false;
    }

    // TODO: нужна ли вообще такая заметка?
    static_cast<void>(match);

    return true;
}

bool Graph::try_beg_multi_note(const string& line) const {
    smatch match;
    static const regex rx{ "^\\s*note\\s+(left|right|top|bottom)\\s+of\\s+(\\S+)\\s*$" };
    if (!regex_match(line, match, rx)) {
        return false;
    }

    const auto node_name{ match[2].str() };
    if (!m_ch->id_node.count(node_name)) {
        throw GraphError(m_ch->line_number, "node not defined");
    }
    return true;
}

bool Graph::try_end_multi_note(const string& line) const {
    static const regex rx{ "^\\s*end\\s+note\\s*$" };
    return regex_match(line, rx);
}

bool Graph::try_note(const string& line, istream& in) {
    if (try_one_note(line)) {
        return true;
    }

    if (!try_beg_multi_note(line) && !try_beg_note_with_id(line)) {
        return false;
    }

    while (in) {
        const auto line{ read_line(in) };
        if (try_end_multi_note(line)) {
            return true;
        }
    }
    throw GraphError(m_ch->line_number, "note is not closed");
}

// -----------------------------------------------------------------------

bool Graph::try_one_comment(const std::string& line) const {
    static const regex rx{ "^\\s*'.*$" };
    return regex_match(line, rx);
}

bool Graph::try_beg_multi_comment(const std::string& line) const {
    static const regex rx{ "^\\s*\\/'.*$" };
    return regex_match(line, rx);
}

bool Graph::try_end_multi_comment(const std::string& line) const {
    static const regex rx{ "^.*'\\/\\s*$" };
    return regex_match(line, rx);
}

bool Graph::try_comment(const std::string& line, std::istream& in) {
    if (try_one_comment(line)) {
        return true;
    }

    if (!try_beg_multi_comment(line)) {
        return false;
    }

    while (in) {
        const auto line{ read_line(in) };
        if (try_end_multi_comment(line)) {
            return true;
        }
    }
    throw GraphError(m_ch->line_number, "comment is not closed");
}

// -----------------------------------------------------------------------

bool Graph::try_any(const std::string& line, std::istream& in) {
    return
            (try_node(line, in) ||
             try_connection(line, in) ||
             try_whitespaces(line) ||
             try_grouping(line, in) ||

             try_note(line, in) ||
             try_comment(line, in) ||
             try_directive(line) ||
             try_skinparam(line) ||
             try_direction(line));
}

bool Graph::try_grouping(const std::string&, std::istream&) {
    return false;
}
