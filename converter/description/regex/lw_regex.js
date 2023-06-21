var pseudo_state = /^\s*\[\*\]\s*$/gm
var image_source = /^\s*<img\s*:\s*(.+?)\s*>\s*$/gm

var hide_empty_description = /^\s*hide\s+empty\s+description\s*$/gm

// TODO: \w заменить на \S?
var empty_state_short_node = /^\s*state\s+(\w+)\s*$/gm
// Запускать после всех других проверок! Когда текст без картинки
var empty_state_whole_node_v0 = /^\s*state\s+\"([^<>]+?)\"\s+as\s+(\w+)\s*$/gm
var empty_state_whole_node_v1 = /^\s*state\s+\"([^<>"]+)\"\s+as\s+(\w+)\s*$/gm

var image_state_short_node = /^\s*state\s+\"\s*<img\s*:\s*(.+?)\s*>\s*\"\s+as\s+(\w+)\s*$/gm
var image_state_whole_node_v0 = /^\s*state\s+\"\s*(.+?)\s*\\n<img\s*:\s*(.+?)\s*>\s*\"\s+as\s+(\w+)\s*$/gm
var image_state_whole_node_v1 = /^\s*state\s+\"\s*([^<>"]+?)\s*\\n<img\s*:\s*(.+?)\s*>\s*\"\s+as\s+(\w+)\s*$/gm

var connection_v0 = /^\s*(\S+)\s+((<?)([-]+([lrdu]|left|right|up|down)[-]+|[-]+)(>?))\s+(\S+)\s*(:(.*))?\s*$/gm
var connection_v1 = /^\s*((\[\*\])|([^\[\*\]\s]+))\s+((<?)([-]+([lrdu]|left|right|up|down)[-]+|[-]+)(>?))\s+((\[\*\])|([^\[\*\]\s]+))\s*(:(.*))?\s*$/gm
var desc_line_to_state = /^\s*(\w+)\s*(:(.*))$/gm