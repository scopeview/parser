#include "parser/parser/parser_status.hpp"

namespace parser_tools { namespace parsers {

const char* parser_step_check_status_array[] = {
    "none",
    "need_ignore_final_parsed",
    "need_check_exit_as_not_parsed",
    "need_check_token_reduced_or_shifted",
    "need_check_final_token",
    "need_check_layer_not_reduced",
    "need_return_next_token_as_final_token",
    "need_check_create_shifter",
    "need_shift",
    "need_reduce"
	};

const char* parser_step_dispatch_status_array[] {
    "none",
	"final_parsed_ignored",
	"not_parsed",
	"self_parsed",
	"just_full_reduced",
	"just_full_shifted",
	"full_parsed_as_reduced",
	"full_parsed_as_shifted",
	"shifted_ignored",
	"reduced_ignored",
	"ignore_nothing",
	"next_token_is_final_token_type",
	"shifter_created",
	"shifter_neednot_created",
	"not_shifted",
	"full_shifted",
	"not_reduced",
	"full_reduced",
	};

const char* parser_step_handle_status_array[] {
    "in_process",
	"not_parsed",
	"full_parsed",
	};

std::ostream& operator<<(std::ostream& os, parser_step_check_status o)
{
    int index = static_cast<std::underlying_type<parser_step_check_status>::type>(o);
    return os <<  parser_step_check_status_array[index];
}

std::ostream& operator<<(std::ostream& os, parser_step_dispatch_status o)
{
    int index = static_cast<std::underlying_type<parser_step_dispatch_status>::type>(o);
    return os << parser_step_dispatch_status_array[index];
}

std::ostream& operator<<(std::ostream& os, parser_step_handle_status o)
{
    int index = static_cast<std::underlying_type<parser_step_handle_status>::type>(o);
    return os << parser_step_handle_status_array[index];
}


}}
