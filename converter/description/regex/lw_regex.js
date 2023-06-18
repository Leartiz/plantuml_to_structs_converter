var pseudo_state = /^\s*\[\*\]\s*$/gm
var image_source = /^\s*<img\s*:\s*(.+?)\s*>\s*$/gm

// TODO: \w заменить на \S?
var empty_state_short_node = /^\s*state\s+(\w+)\s*$/gm
// Запускать после всех других проверок!
var empty_state_whole_node = /^\s*state\s+\"([^<>]+)\"\s+as\s+(\w+)\s*$/gm

var image_state_short_node = /^\s*state\s+\"\s*<img\s*:\s*(.+?)\s*>\s*\"\s+as\s+(\w+)\s*$/gm
var image_state_whole_node = /^\s*state\s+\"\s*(.+?)\s*\\n<img\s*:\s*(.+?)\s*>\s*\"\s+as\s+(\w+)\s*$/gm

var connection = /^\s*(\S+)\s+((<?)([-]+([lrdu]|left|right|up|down)[-]+|[-]+)(>?))\s+(\S+)\s*(:(.*))?\s*$/gm
var one_note_to_state = /^\s*(\w+)\s*(:(.*))$/gm