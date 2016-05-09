#ifndef __PARSER_PARSER_PARSER_STATUS_HPP__
#define __PARSER_PARSER_PARSER_STATUS_HPP__

#include <string>
#include <iostream>

namespace parser_tools { namespace parsers {

enum class parser_step_check_status {
    none,
    need_ignore_final_parsed,
	need_check_exit_as_not_parsed,
	need_check_token_reduced_or_shifted,
	need_check_final_token,
	need_check_layer_not_reduced,
	need_return_next_token_as_final_token,
	need_check_create_shifter,
	need_shift,
	need_reduce
	};

enum class parser_step_dispatch_status {
    none,
    final_parsed_ignored,
	not_parsed,
	self_parsed,
	just_full_reduced,
	just_full_shifted,
	full_parsed_as_reduced,
	full_parsed_as_shifted,
	shifted_ignored,
	reduced_ignored,
	ignore_nothing,
	next_token_is_final_token_type,
	shifter_created,
	shifter_neednot_created,
	not_shifted,
	full_shifted,
	not_reduced,
	full_reduced,
	};

enum class parser_step_handle_status {
    in_process,
	not_parsed,
	full_parsed,
	};


enum class parser_status {
    none,
    not_parsed,
	full_parsed,
	};

////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream&, parser_step_check_status);
std::ostream& operator<<(std::ostream&, parser_step_dispatch_status);
std::ostream& operator<<(std::ostream&, parser_step_handle_status);

}}
#endif	/* __PARSER_PARSER_PARSER_STATUS_HPP__ */
