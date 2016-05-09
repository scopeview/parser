# source /home/dd/dd/computer/develop/c++/gdb/common.gdb
# source /home/dd/dd/computer/develop/c++/gdb/pretty-printer.gdb

set breakpoint pending on

set environment LD_LIBRARY_PATH=/home/dd/develop/c++/compiler/parser/.libs:/home/dd/project/gtest-1.7.0/lib/.libs/:/home/dd/develop/c++/estd/.libs
# file unit_test/test
file demo/demo

b main
b parser_tools::parsers::parser::parser_step_check
b parser_tools::parsers::parser::parser_step_dispatch
b parser_tools::parsers::parser::parser_step_handle

# b parser::filter::filter::filterate()
# b parser_tools::parsers::parser::parser_create_shifter_internal
# b parser_tools::shifters::shifter::shifter_not_shifted_handler
# b parser_tools::shifters::shifter_stream::read_token

r
