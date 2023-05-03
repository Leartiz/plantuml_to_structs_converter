var three_dots = /^\s*[.]{3}\s*$/gm

var whole_node = /^\s*(boundary|entity|actor|control)\s+\"(.+)\"\s+as\s+([^\s#]+)\s*(#red)?\s*$/gm
var short_node = /^\s*(boundary|entity|actor|control)\s+([^\s#]+)\s*(#red)?\s*$/gm
var connection = /^\s*(\S+)\s+((<?)([-]+)(>?))\s+(\S+)\s*(:(.*))?\s*$/gm

var operand_start = /^\s*(alt|opt|loop)\s+(.*)$/gm
var operand_else = /^\s*else\s+(.*)$/gm
var operand_end = /^\s*end\s*$/gm

var beg_ref_over = /^\s*ref\s+over\s+(.*)$/gm
var end_ref = /^\s*end\s+ref\s*$/gm