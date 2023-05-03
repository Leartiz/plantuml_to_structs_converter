// Reads one line into the converter.

// TODO: \s --> [\r\t\f\v ]

var usecase_whole_node = /^\s*usecase\s+(\((.+)\)|\"(.+)\")\s+as\s+(\S+)\s*$/gm
var actor_whole_node = /^\s*actor\s+(:(.+):|\"(.+)\")\s+as\s+(\S+)\s*$/gm

var usecase_short_node = /^\s*\((.+)\)\s+as\s+(\S+)\s*$/gm
var actor_short_node = /^\s*:(.+):\s+as\s+(\S+)\s*$/gm

var connection = /^\s*(:.+:|\(.+\))\s+((<|<\|)?([-\.]+([lrdu]|left|right|up|down)[-\.]+|[-\.]+)(\|>|>)?)\s+(\S+)\s*(:\s*(<<(include|extend)>>))?\s*$/gm