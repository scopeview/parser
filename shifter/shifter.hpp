#ifndef __PARSER_SHIFTER_SHIFTER_HPP__
#define __PARSER_SHIFTER_SHIFTER_HPP__

#include <stack>
#include <tuple>
#include "libtemplate/_.hpp"
#include "parser/basic/dummy.hpp"
#include "parser/basic/unique_id.hpp"
#include "parser/reducer/_.hpp"
#include "parser/shifter/shifter_status.hpp"
#include "parser/shifter/shifter_result.hpp"
#include "parser/shifter/shifter_function_subtype.hpp"
#include "parser/shifter/shifter_stream.hpp"
#include "parser/parser/parser.hpp"
#include "parser/parser/parser_status.hpp"
#include "parser/parser/parser_policy.hpp"

namespace parser_tools { namespace parsers {
class parser;
}}

namespace parser_tools { namespace shifters {

using namespace parser_tools::parsers;

typedef libtemplate::bool_selector<dummy::is_active, dummy::is_ignore_final_shifted> shifter_bool_selector_type;
typedef libtemplate::enum_selector<shifter_status, self_parsed_policy> shifter_enum_selector_type;

class shifter : // config
	// status
	public shifter_bool_selector_type,
	public shifter_enum_selector_type,
	// debug
	public basic::unique_id<shifter>

{
    typedef std::stack<std::shared_ptr<parser>> parser_stack_type;

    shifter_stream m_shifter_stream;

    shifter_step_check_status m_shifter_step_check_status;
    shifter_step_dispatch_status m_shifter_step_dispatch_status;
    shifter_step_handle_status m_shifter_step_handle_status;

    
    reducers::seq m_shifter_reducer;
    token m_token_shifted;
    token m_token_parsed;
    int m_reducer_index;
    int m_reducer_index_stack_length;
    parsers::parser* m_parent_parser;

    parser_stack_type m_parser_stack;

    // debug
    int m_parser_shifter_level;

public:
    shifter(int, int, reducers::seq&, parser*, int);

    bool shifter_is_parser_stack_empty();
    bool shifter_is_shift_streamer_empty();
    bool shifter_current_parser_status_p(parser_status);
    bool shifter_has_token_shifted();
    bool shifter_has_token_parsed();
    bool shifter_activep();
    void shifter_activate();
    void shifter_deactivate();
    bool shifter_is_ignore_final_shifted();
    void shifter_set_ignore_final_shifted(bool);
    bool shifter_is_shift_streamer_filled();
    bool shifter_is_shift_streamer_not_filled();
////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////
    shifter_step_dispatch_status shifter_create_shifter_parser();
    std::shared_ptr<parser> shifter_make_parser();

////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////
    shifter_step_dispatch_status shifter_reducer_reduce();
////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////
    shifter_step_dispatch_status shifter_ignore(shifter_ignore_type);
    shifter_step_dispatch_status shifter_ignore_all();
    shifter_step_dispatch_status shifter_ignore_all_except_first_token();
    shifter_step_dispatch_status shifter_ignore_final_shifted();
////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////
    shifter_step_dispatch_status shifter_shift_parse();
    void shifter_full_parsed_handler();
    shifter_step_handle_status shifter_not_parsed_handler();
////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////
    void shifter_not_shifted_handler();
    void shifter_full_shifted_handler();
////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////
    shifter_step_check_status shifter_step_check();
    shifter_step_dispatch_status shifter_step_dispatch(shifter_step_check_status);
    shifter_step_handle_status shifter_step_handle(shifter_step_dispatch_status);
    shifter_step_handle_status shifter_step();
    shifter_result shifter_shift();
    token_type get_final_token_type();
    int get_reducer_index_stack_length();
};

}}
#endif	/* __PARSER_SHIFTER_SHIFTER_HPP__ */
