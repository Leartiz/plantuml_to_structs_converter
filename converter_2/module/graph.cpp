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
    return regex_match(line, match, regex("^\\s*skinparam\\s*\\S*\\s*\\S*\\s*$"));
}
