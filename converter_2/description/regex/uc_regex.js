// Reads one line into the converter.

// TODO: \s --> [\r\t\f\v ] (???)

var usecase_node = /^\s*usecase\s+(\((.+)\)|\"(.+)\")\s+as\s+(\S+)\s*$/gm
var actor_node = /^\s*actor\s+(:(.+):|\"(.+)\")\s+as\s+(\S+)\s*$/gm
var connection = /^\s*(\S+)\s+((<|<\|)?([-\.]+([lrdu]|left|right|up|down)[-\.]+|[-\.]+)(\|>|>)?)\s+(\S+)\s*(:\s*(<<(include|extend)>>))?\s*$/gm