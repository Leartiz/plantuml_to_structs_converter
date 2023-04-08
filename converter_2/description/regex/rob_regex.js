var whole_node = /^\s*(boundary|entity|actor|control)\s+\"(.+)\"\s+as\s+(\S+)\s*$/gm
var short_node = /^\s*(boundary|entity|actor|control)\s+(\S+)\s*$/gm
var connection = /^\s*(\S+)\s+((<?)([-\.]+([lrdu]|left|right|up|down)[-\.]+|[-\.]+)(>?))\s+(\S+)\s*(:\s*(.*))?\s*$/gm