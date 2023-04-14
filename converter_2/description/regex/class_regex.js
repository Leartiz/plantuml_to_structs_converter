
var any_node = /^\s*(class|enum|interface)\s+(\S+)\s*{\s*$/gm
var class_node = /^\s*class\s+\S+\s*{\s*$/gm
var enum_node = /^\s*enum\s+\S+\s*{\s*$/gm

var member = /^\s*([+#-]?)(\w+)\s*(\(([\w<>]+,?\s*)*\))?\s*:\s*([\w<>]+)\s*$/gm
var member_data = /^\s*([+#-]?)(\S+)\s*:\s*(\S+)\s*$/gm
var member_func = /^\s*([+#-]?)(\w+)\s*(\(([\w<>]+,?\s*)*\))\s*:\s*([\w<>]+)\s*$/gm