var directive = /^\s*(@startuml|@enduml)\s*$/gm
var skinparam = /^\s*skinparam\s+\S+\s+\S+\s*$/gm
var direction = /^\s*((left\s+to\s+right\s+direction)|(top\s+to\s+bottom\s+direction))\s*$/gm
var beg_grouping = /^\s*rectangle|package\s+\S+\s*\{\s*$/gm
var end_curly_brace = /^\s*\}\s*$/gm
var whitespaces = /^\s*$/gm
