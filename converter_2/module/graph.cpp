#include <regex>

#include "graph.h"

using namespace std;

bool Graph::try_whitespaces(string& line) {
    std::smatch match;
    return regex_match(line, match, regex("^\\s*$"));
}

bool Graph::try_directive(string& line) {
    std::smatch match;
    return regex_match(line, match, regex("^\\s*(@startuml|@enduml)\\s*$"));
}

bool Graph::try_skinparam(std::string& line) {
    std::smatch match;
    return regex_match(line, match, regex("^\\s*skinparam\\s+\\S+\\s+\\S+\\s*$"));
}

bool Graph::try_direction(std::string& line) {
    std::smatch match;
    return regex_match(line, match, regex("^\\s*((left\\s+to\\s+right\\s+direction)|(top\\s+to\\s+bottom\\s+direction))\\s*$"));
}

bool Graph::try_beg_grouping(std::string& line) {
    std::smatch match;
    return regex_match(line, match, regex("^\\s*rectangle|package\\s+\\S+\\s*\\{\\s*$"));
}

bool Graph::try_end_curly_brace(std::string& line) {
    std::smatch match;
    return regex_match(line, match, regex("^\\s*\\}\\s*$"));
}
