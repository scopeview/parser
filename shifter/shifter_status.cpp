#include "parser/shifter/shifter_status.hpp"

namespace parser_tools { namespace shifters {

const char* shifter_step_check_status_array[] = {
    "need_ignore_final_shifted",
    "need_check_exit_as_not_shifted",
    "need_check_final_token",
    "need_shifter_shift_parse",
    "need_create_shifter_parser"
};

const char* shifter_step_dispatch_status_array[] = {
    "all_ignored",
    "final_shifted_ignored",
    "all_except_first_token_ignored",
    "full_shifted",
    "not_parsed",
    "full_parsed",
    "parser_created"
};

const char* shifter_step_handle_status_array[] = {
    "in_process",
    "not_shifted",
    "full_shifted"
};

std::ostream& operator<<(std::ostream& os, shifter_step_check_status o)
{
    int index = static_cast<std::underlying_type<shifter_step_check_status>::type>(o);
    return os <<  shifter_step_check_status_array[index];
}

std::ostream& operator<<(std::ostream& os, shifter_step_dispatch_status o)
{
    int index = static_cast<std::underlying_type<shifter_step_dispatch_status>::type>(o);
    return os << shifter_step_dispatch_status_array[index];
}

std::ostream& operator<<(std::ostream& os, shifter_step_handle_status o)
{
    int index = static_cast<std::underlying_type<shifter_step_handle_status>::type>(o);
    return os << shifter_step_handle_status_array[index];
}


}}
