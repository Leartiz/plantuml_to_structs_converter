var whole_node = /^\s*(boundary|entity|actor|control)\s+\"(.+)\"\s+as\s+([^\s#]+)\s*(#red)?\s*$/gm
var short_node = /^\s*(boundary|entity|actor|control)\s+([^\s#]+)\s*(#red)?\s*$/gm
var connection = /^\s*(\S+)\s+((<?)([-]+)(>?))\s+(\S+)\s*(:(.*))?\s*$/gm

var group_start = /^\s*(alt|opt|loop)\s+(\S+)\s*$/gm

var beg_ref_over = /^\s*ref\s+over\s+((\S+?,?)+)$/gm
var end_ref = /^\s*end\s+ref\s*$/gm