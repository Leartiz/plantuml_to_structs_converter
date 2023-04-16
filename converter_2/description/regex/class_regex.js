var any_node = /^\s*(class|enum|interface)\s+(\S+)\s*{\s*$/gm
var class_node = /^\s*class\s+\S+\s*{\s*$/gm
var enum_node = /^\s*enum\s+\S+\s*{\s*$/gm
var interface_node = /^\s*interface\s+\S+\s*{\s*$/gm

var enum_value = /^$/gm
var member = /^\s*([+#-]?)(\w+)\s*(\(([\w<>]+,?\s*)*\))?\s*:\s*([\w<>]+)\s*$/gm
var member_data = /^\s*([+#-]?)(\w+)\s*:\s*([\w<>]+)\s*$/gm
var member_func = /^\s*([+#-]?)(\w+)\s*(\(([\w<>]+,?\s*)*\))\s*:\s*([\w<>]+)\s*$/gm

var connection = /^\s*(\S+)\s+((<|<\|o|\*)?([-\.]+([lrdu]|left|right|up|down)[-\.]+|[-\.]+)(\|>|>|o|\*)?)\s+(\S+)\s*(:(.+))?$/gm