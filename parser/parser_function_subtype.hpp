#ifndef __PARSER_PARSER_PARSER_FUNCTION_SUBTYPE_HPP__
#define __PARSER_PARSER_PARSER_FUNCTION_SUBTYPE_HPP__

namespace parser_tools { namespace parsers {

enum class set_reducer_index_type {
    not_shifted,
    full_shifted,
    not_reduced,
    full_reduced,
    ignore_shifter,
};

enum class parser_undo_type {
    all,
	parsed_token_stream,
	current_reduced,
};

enum class parser_ignore_type {
    last_one_last_layer,
	last_one_current_layer,
	current_one,
	current_reduced,
	current_shifted,
	final_parsed,
	final_parsed_as_reduced,
	final_parsed_as_shifted,
};

}}
#endif	/* __PARSER_PARSER_PARSER_FUNCTION_SUBTYPE_HPP__ */
