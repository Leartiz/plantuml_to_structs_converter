var directive = /^\s*(@startuml|@enduml)\s*$/gm
var skinparam = /^\s*skinparam\s+\S+\s+\S+\s*$/gm
var direction = /^\s*((left\s+to\s+right\s+direction)|(top\s+to\s+bottom\s+direction))\s*$/gm

var whitespace = /^\s*$/gm

var beg_grouping = /^\s*(rectangle|package)(\s+((\S+)|(\".+\")))?\s*\{\s*$/gm
var end_curly_brace = /^\s*\}\s*$/gm

var one_note_all = /^\s*note\s+(left|right|top|bottom)\s*(\s+of\s+(\S+))?\s*:(.+)$/gm
var one_note0 = /^\s*note\s+(left|right|top|bottom)\s+of\s+(\S+)\s*:(.+)$/gm
var one_note1 = /^\s*note\s+(left|right|top|bottom)\s*:(.+)$/gm
var beg_note_with_id = /^\s*note\s+as\s+(\S+)\s*$/gm
var beg_multi_note = /^\s*note\s+(left|right|top|bottom)\s+of\s+(\S+)\s*$/gm
var end_multi_note = /^\s*end\s+note\s*$/gm

var one_comment = /^\s*'.*$/gm
var beg_multi_comment = /^\s*\/'.*$/gm
var end_multi_comment = /^.*'\/\s*$/gm

