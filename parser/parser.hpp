#ifndef __PARSER_PARSER_PARSER_HPP__
#define __PARSER_PARSER_PARSER_HPP__

#include <stack>
#include <tuple>
#include "libtemplate/_.hpp"
#include "parser/basic/dummy.hpp"
#include "parser/basic/unique_id.hpp"
#include "parser/reducer/_.hpp"
#include "parser/parser/parser_status.hpp"
#include "parser/parser/parser_result.hpp"
#include "parser/parser/parser_policy.hpp"
#include "parser/parser/parser_function_subtype.hpp"
#include "parser/parser/parser_stream.hpp"
#include "parser/shifter/_.hpp"

namespace parser_tools { namespace shifters {
class shifter;
}}

namespace parser_tools { namespace parsers {

using namespace shifters;

typedef libtemplate::bool_selector<dummy::is_active, dummy::is_ignore_final_parsed> parser_bool_selector_type;
typedef libtemplate::enum_selector<parser_status> parser_enum_selector_type;

class parser : public parser_bool_selector_type,
	       public parser_enum_selector_type,
	       public basic::unique_id<parser>
{
public:
    typedef std::list<reducers::abstract_reducer*> reducer_list_type;
    typedef std::stack<std::shared_ptr<shifter>> shifter_stack_type;

private:
// config
    token_type m_token_type_final;
    std::shared_ptr<parser_stream> m_parser_stream;
    reducer_list_type m_reducer_list;

    final_token_checker_policy m_final_token_checker_policy;
    self_parsed_policy m_self_parsed_policy;

// status
    token m_token_reduced_or_shifted;
    std::stack<int> m_reducer_index_stack;

    parser_step_check_status m_parser_step_check_status;
    parser_step_dispatch_status m_parser_step_dispatch_status;
    parser_step_handle_status m_parser_step_handle_status;
    parser_status m_parser_status;

    shifter_stack_type m_shifter_stack;

// debug
    int m_parser_shifter_level;

public:
    parser(std::shared_ptr<parser_stream>&, reducer_list_type&, const token_type&, final_token_checker_policy, self_parsed_policy, int);

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
    reducers::abstract_reducer* get_current_reducer();
    shifter* get_current_shifter();
    std::shared_ptr<parser_stream> get_parser_stream();
    shifter_stack_type& get_shifter_stack() {return m_shifter_stack;};
    reducer_list_type& get_reducer_list() {return m_reducer_list;}


////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
    bool is_current_reducer_seq_reducer();
    bool is_next_token_first_token_of_seq_reducer();
    bool is_current_reducer_end();
    bool is_reducer_index_stack_empty();
    bool is_current_shifter_active();
    bool has_token_reduced_or_shifted();
    bool is_next_token_final_token();
    bool is_token_reduced_or_shifted_final_token();
    bool is_ignore_final_parsed();
    void set_ignore_final_parsed();
    void clear_ignore_final_parsed();
    bool is_current_token_shifted_by_current_shifter();

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
    void set_reducer_index(set_reducer_index_type);
    bool status_p(parser_status);
    void set_status(parser_status);

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
    parser_step_dispatch_status parser_check_create_shifter();
    void parser_create_shifter_internal();
    parser_step_dispatch_status parser_shift();
    parser_step_dispatch_status parser_shift_internal();
    parser_step_dispatch_status parser_reduce();
    parser_step_dispatch_status parser_reduce_internal();

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
    void parser_undo (parser_undo_type);
    void parser_undo_all();
    void parser_undo_parsed_token_streamer();
    void parser_undo_current_reduced();

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
    parser_step_dispatch_status parser_ignore(parser_ignore_type);
    parser_step_dispatch_status parser_ignore_last_one_last_layer();
    parser_step_dispatch_status parser_ignore_last_one_current_layer();
    parser_step_dispatch_status parser_ignore_current_one();
    parser_step_dispatch_status parser_ignore_current_shifted();
    parser_step_dispatch_status parser_ignore_current_reduced();
    parser_step_dispatch_status parser_ignore_final_parsed();
    parser_step_dispatch_status parser_ignore_final_parsed_as_reduced();
    parser_step_dispatch_status parser_ignore_final_parsed_as_shifted();

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
    void parser_step_not_shifted_handler();
    void parser_step_full_shifted_handler();

    
////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
    void parser_step_not_reduced_handler();
    void parser_step_full_reduced_handler();

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
    parser_step_dispatch_status parser_step_final_token_checker();
    parser_step_dispatch_status parser_step_check_token_reduced_or_shifted_handler();
    parser_step_dispatch_status parser_step_check_not_parsed_handler();
    void parser_step_full_parsed_as_reduced_handler();
    void parser_step_full_parsed_as_shifted_handler();
    parser_step_dispatch_status parser_step_layer_not_reduced_handler();

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
    parser_step_dispatch_status parser_step_return_next_token_as_final_token_handler();

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
    parser_step_check_status parser_step_check();
    parser_step_dispatch_status parser_step_dispatch(parser_step_check_status);
    parser_step_handle_status parser_step_handle(parser_step_dispatch_status);
    parser_step_handle_status parser_step();
    parser_result parser_parse();
};

void parser_tools_init();

}}
#endif	/* __PARSER_PARSER_PARSER_HPP__ */
