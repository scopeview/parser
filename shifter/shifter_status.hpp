#ifndef __PARSER_SHIFTER_SHIFTER_STATUS_HPP__
#define __PARSER_SHIFTER_SHIFTER_STATUS_HPP__

#include <iostream>

namespace parser_tools { namespace shifters {

enum class shifter_step_check_status {
    none,
    need_ignore_final_shifted,
	need_check_exit_as_not_shifted,
	need_check_final_token,
	need_shifter_shift_parse,
	need_create_shifter_parser
};

enum class shifter_step_dispatch_status {
    none,
    all_ignored,
    final_shifted_ignored,
	all_except_first_token_ignored,
	full_shifted,
	not_parsed,
	full_parsed,
	parser_created
};

enum class shifter_step_handle_status {
    in_process,
	not_shifted,
	full_shifted
};

enum class shifter_status {
    none,
    not_shifted,
	full_shifted
};

////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream&, shifter_step_check_status);
std::ostream& operator<<(std::ostream&, shifter_step_dispatch_status);
std::ostream& operator<<(std::ostream&, shifter_step_handle_status);


}}
#endif	/* __PARSER_SHIFTER_SHIFTER_STATUS_HPP__ */
