var whole_node = /^\s*(boundary|entity|actor|control)\s+\"(.+)\"\s+as\s+([^\s#]+)\s*(#red)?\s*$/gm
var short_node = /^\s*(boundary|entity|actor|control)\s+([^\s#]+)\s*(#red)?\s*$/gm
var connection = /^\s*(\S+)\s+((<?)([-]+([lrdu]|left|right|up|down)[-]+|[-]+)(>?))\s+(\S+)\s*(:(.*))?\s*$/gm