#include <glog/logging.h>
#include "parser/shifter/shifter.hpp"
#include "parser/parser/parser_result.hpp"

namespace parser_tools { namespace shifters {

using namespace parser_tools::reducers;
using namespace parser_tools::parsers;

shifter::shifter(int reducer_index, int reducer_index_stack_length, reducers::seq& shifter_reducer, parser* parent_parser, int parser_shifter_level)
    : m_reducer_index(reducer_index), m_reducer_index_stack_length(reducer_index_stack_length), m_shifter_reducer(shifter_reducer),
      m_parent_parser(parent_parser), m_parser_shifter_level(parser_shifter_level)
{
    set_bool<dummy::is_active>(true);
    set_bool<dummy::is_ignore_final_shifted>(false);
    set_enum<shifter_status>(shifter_status::none);
    set_enum<self_parsed_policy>(self_parsed_policy::allow_self_parsed);

    token first_token_of_seq_reducer(m_parent_parser->get_parser_stream()->read_token());
    assert(first_token_of_seq_reducer.is_type((static_cast<token_type_list_reduced_from&>(m_shifter_reducer)).front()));
    m_shifter_stream.add_tail_token(first_token_of_seq_reducer);
    m_parser_stack.push(shifter_make_parser());
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
token_type shifter::get_final_token_type()
{
    return libtemplate::static_cast_reference<reducers::seq, token_type_reduced, token_type>(m_shifter_reducer);
}

int shifter::get_reducer_index_stack_length()
{
    return m_reducer_index_stack_length;
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
bool shifter::shifter_is_parser_stack_empty()
{
    return m_parser_stack.empty();
}

bool shifter::shifter_is_shift_streamer_empty()
{
    return m_shifter_stream.empty();
}

bool shifter::shifter_current_parser_status_p(parser_status status)
{
    assert(!shifter_is_parser_stack_empty());
    auto parser_ = m_parser_stack.top();
    return parser_->is_enum<parsers::parser_status>(status);
}

bool shifter::shifter_has_token_shifted()
{
    return !m_token_shifted.is_nil();
}

bool shifter::shifter_has_token_parsed()
{
    return !m_token_parsed.is_nil();
}

bool shifter::shifter_activep()
{
    return is_true<dummy::is_active>();
}

void shifter::shifter_activate()
{
    set_bool<dummy::is_active>(true);
}

void shifter::shifter_deactivate()
{
    set_bool<dummy::is_active>(false);
}

bool shifter::shifter_is_ignore_final_shifted()
{
    return is_true<dummy::is_ignore_final_shifted>();
}

void shifter::shifter_set_ignore_final_shifted(bool status)
{
    set_bool<dummy::is_ignore_final_shifted>(status);
}

bool shifter::shifter_is_shift_streamer_filled()
{
    int size_1 = static_cast<token_type_list_reduced_from&>(m_shifter_reducer).size();
    int size_2 = m_shifter_stream.size();
    assert(size_1 >= size_2);
    return size_1 == size_2;
}

bool shifter::shifter_is_shift_streamer_not_filled()
{
    return !shifter_is_shift_streamer_filled();
}

////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////
shifter_step_dispatch_status shifter::shifter_create_shifter_parser()
{
    assert(!shifter_has_token_shifted());
    assert(!shifter_is_ignore_final_shifted());
    assert(!shifter_is_parser_stack_empty());
    assert(!shifter_is_shift_streamer_filled());
    assert(!shifter_current_parser_status_p(parser_status::none));
    assert(shifter_activep());
    
    shifter_step_dispatch_status result;
    m_parser_stack.push(shifter_make_parser());
    result = shifter_step_dispatch_status::parser_created;
    m_shifter_step_dispatch_status = result;
    return result;
}

std::shared_ptr<parser> shifter::shifter_make_parser()
{
    assert(!shifter_is_shift_streamer_filled());
    
    token_list shifted_token_list(m_shifter_stream.peek_token_all());
    token_type_list_reduced_from& token_type_list_reduced_from_ = m_shifter_reducer;
    auto it = token_type_list_reduced_from_.begin();
    std::advance(it, m_shifter_stream.size());
    token_type token_type_final(*it);
    std::shared_ptr<parser_stream> new_parser_stream(m_parent_parser->get_parser_stream()->copy_class());
	      
    assert(m_shifter_stream.size() < token_type_list_reduced_from_.size());
    // TODO
    // (assert (every #'utils:notnull (mapcar #'lexer:token_typep shifted_token_list token_type_list_reduced_from)))

    std::shared_ptr<parser> new_parser(new parsers::parser(new_parser_stream,
							   m_parent_parser->get_reducer_list(),
							   token_type_final,
							   final_token_checker_policy::return_immediately,
							   get_enum<self_parsed_policy>(),
							   m_parser_shifter_level+1));
    return new_parser;
}

////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////
shifter_step_dispatch_status shifter::shifter_reducer_reduce()
{
    assert(!shifter_has_token_shifted());
    assert(!shifter_is_ignore_final_shifted());
    assert(!shifter_is_parser_stack_empty());
    assert(shifter_activep());
    assert(shifter_is_shift_streamer_filled());
    
    reducer_result result(m_shifter_reducer.reduce(m_shifter_stream));
    assert(result.get_reducer_status() == reducer_status::full_reduced);
    m_token_shifted = result.get_token();

    m_shifter_step_dispatch_status = shifter_step_dispatch_status::full_shifted;
    return m_shifter_step_dispatch_status;
}

////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////
shifter_step_dispatch_status shifter::shifter_ignore(shifter_ignore_type type)
{
    shifter_step_dispatch_status result;
    switch (type) {
    case shifter_ignore_type::all:
	result = shifter_ignore_all();
	break;
    case shifter_ignore_type::all_except_first_token:
	result = shifter_ignore_all_except_first_token();
	break;
    case shifter_ignore_type::final_shifted:
	result = shifter_ignore_final_shifted();
	break;
    default:
	assert(0);
	break;
    }
    return result;
}

shifter_step_dispatch_status shifter::shifter_ignore_all()
{
    assert(!shifter_has_token_shifted());
    assert(!shifter_is_ignore_final_shifted());
    assert(shifter_is_parser_stack_empty());
    assert(shifter_activep());
    
    parser_stream& parser_stream_ = *m_parent_parser->get_parser_stream();
    token_list l(m_shifter_stream.read_token_all());
    std::for_each(l.rbegin(), l.rend(), [&](token& t) {
	    parser_stream_.unread_token(t.to_orignal_token_list());
	});
    m_shifter_step_dispatch_status = shifter_step_dispatch_status::all_ignored;
    return m_shifter_step_dispatch_status;
}

shifter_step_dispatch_status shifter::shifter_ignore_all_except_first_token()
{
    assert(!shifter_has_token_shifted());
    assert(!shifter_is_ignore_final_shifted());
    assert(shifter_is_parser_stack_empty());
    assert(shifter_activep());
    token_list tlist(m_shifter_stream.read_token_all());
    assert(!tlist.empty());
    token first_token(tlist.front());
    auto it_rend = tlist.rend();
    --it_rend;
    parser_stream& parser_stream_ = *m_parent_parser->get_parser_stream();
    std::for_each(tlist.rbegin(), it_rend, [&](token& t) {
	    parser_stream_.unread_token(t.to_orignal_token_list());
	});
    // TODO
    // parser_stream_.unread_token(first_token);
    m_shifter_stream.unread_token(first_token);
    
    m_shifter_step_dispatch_status = shifter_step_dispatch_status::all_except_first_token_ignored;
    return m_shifter_step_dispatch_status;
}

shifter_step_dispatch_status shifter::shifter_ignore_final_shifted()
{
    assert(!shifter_is_parser_stack_empty());
    LOG(INFO) << __func__ << "shifter unique id: " << (basic::unique_id<shifter>&)(m_parent_parser->get_shifter_stack().top())
	      << ", " << (basic::unique_id<shifter>&)(*this)
	      << "(m_parent_parser->get_shifter_stack().size()): " << (m_parent_parser->get_shifter_stack().size());
    assert((basic::unique_id<shifter>&)(*m_parent_parser->get_shifter_stack().top())
    	   == (basic::unique_id<shifter>&)(*this));

    auto last_parser = m_parser_stack.top();
    token token_shifted(last_parser->get_parser_stream()->read_token());
    assert(token_shifted.is_type(libtemplate::static_cast_reference<reducers::seq, token_type_reduced, token_type>(m_shifter_reducer)));
    assert(m_shifter_stream.empty());

    // put butlast token-list to shift-streamer
    token_list& token_list_reduced_from_ = token_shifted;
    auto it_end = token_list_reduced_from_.end();
    --it_end;
    std::for_each(token_list_reduced_from_.begin(), it_end, [&](const token& t) {
	    m_shifter_stream.add_tail_token(t);
	});

    // put last token to last parser and set ignore-final-parsed
    last_parser->get_parser_stream()->unread_token(*it_end);					       
    set_enum<self_parsed_policy>(self_parsed_policy::not_allow_self_parsed);
    last_parser->set_bool<dummy::is_ignore_final_parsed>(true);
    last_parser->set_enum<parser_status>(parser_status::none);
    set_bool<dummy::is_ignore_final_shifted>(false);

    m_shifter_step_dispatch_status = shifter_step_dispatch_status::final_shifted_ignored;
    return m_shifter_step_dispatch_status;
}

////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////
shifter_step_dispatch_status shifter::shifter_shift_parse()
{
    assert(!shifter_is_parser_stack_empty());
    assert(!shifter_has_token_parsed());
    assert(shifter_current_parser_status_p(parser_status::none));
    assert(shifter_is_shift_streamer_not_filled());
    
    shifter_step_dispatch_status result = shifter_step_dispatch_status::none;
    auto last_parser = m_parser_stack.top();
    parser_result parser_result_ = last_parser->parser_parse();
    switch (parser_result_.get_parser_status()) {
    case parser_status::not_parsed:
	result = shifter_step_dispatch_status::not_parsed;
	break;
    case parser_status::full_parsed:
	m_token_parsed = static_cast<token&>(parser_result_);
	result = shifter_step_dispatch_status::full_parsed;
	break;
    default:
	assert(0);
	break;
    }
    m_shifter_step_dispatch_status = result;
    return result;
}

void shifter::shifter_full_parsed_handler()
{
    assert(!shifter_has_token_shifted());
    assert(shifter_has_token_parsed());
    assert(shifter_is_shift_streamer_not_filled());

    m_shifter_stream.add_tail_token(m_token_parsed);
    m_token_parsed = token();
    set_enum<self_parsed_policy>(self_parsed_policy::allow_self_parsed);
}

shifter_step_handle_status shifter::shifter_not_parsed_handler()
{
    assert(!shifter_has_token_parsed());
    assert(!shifter_has_token_shifted());
    assert(shifter_is_shift_streamer_not_filled());

    shifter_step_handle_status result = shifter_step_handle_status::in_process;

    m_parser_stack.pop();
    if (m_parser_stack.empty()) {
	result = shifter_step_handle_status::not_shifted;
    } else {
	token last_token(m_shifter_stream.read_tail_token());
	auto last_parser = m_parser_stack.top();
	assert(m_shifter_stream.size() > 0);
	last_parser->get_parser_stream()->unread_token(last_token);
	last_parser->set_bool<dummy::is_ignore_final_parsed>(true);
	last_parser->set_enum<parser_status>(parser_status::none);
    }

    m_shifter_step_handle_status = result;
    return result;
}

////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////
void shifter::shifter_not_shifted_handler()
{
    assert(!shifter_has_token_parsed());
    assert(!shifter_has_token_shifted());
    assert(shifter_is_parser_stack_empty());
    // when creating shifter, the first token is exactly the first token of seq-reudcer
    // and is put to shift-streamer, so put it back
    assert(m_shifter_stream.size() == 1);
    token first_token(m_shifter_stream.read_token());
    m_parent_parser->get_parser_stream()->unread_token(first_token);
}

void shifter::shifter_full_shifted_handler()
{
    assert(!shifter_has_token_parsed());
    assert(!shifter_is_parser_stack_empty());
    assert(shifter_is_shift_streamer_empty());
    assert(shifter_has_token_shifted());
}

////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////
shifter_step_check_status shifter::shifter_step_check()
{
    auto is_need_ignore_final_shifted = [&]() {
	return shifter_is_ignore_final_shifted(); };
    auto is_need_check_exit_as_not_shifted = [&]() {
	return shifter_is_parser_stack_empty(); };
    auto is_need_check_final_token = [&]() {
	return shifter_is_shift_streamer_filled(); };
    auto is_need_shifter_shift_parse = [&]() {
	return shifter_current_parser_status_p(parser_status::none); };
    auto is_need_create_shifter_parser = [&]() {
	return shifter_is_shift_streamer_not_filled(); };
    
    shifter_step_check_status result;
    if (is_need_ignore_final_shifted()) {
	result = shifter_step_check_status::need_ignore_final_shifted;
    } else if (is_need_check_exit_as_not_shifted()) {
	result = shifter_step_check_status::need_check_exit_as_not_shifted;
    } else if (is_need_check_final_token()) { 
	result = shifter_step_check_status::need_check_final_token;
    } else if (is_need_shifter_shift_parse()) { 
	result = shifter_step_check_status::need_shifter_shift_parse;
    } else if (is_need_create_shifter_parser()) {
	result = shifter_step_check_status::need_create_shifter_parser;
    }
    m_shifter_step_check_status = result;
    return result;
}

shifter_step_dispatch_status shifter::shifter_step_dispatch(shifter_step_check_status status)
{
    shifter_step_dispatch_status result;
    switch (status) {
    case shifter_step_check_status::need_ignore_final_shifted:
	result = shifter_ignore(shifter_ignore_type::final_shifted);
	break;
    case shifter_step_check_status::need_check_exit_as_not_shifted:
	result = shifter_ignore(shifter_ignore_type::all_except_first_token);
	break;
    case shifter_step_check_status::need_check_final_token:
	result = shifter_reducer_reduce();
	break;
    case shifter_step_check_status::need_shifter_shift_parse:
	result = shifter_shift_parse();
	break;
    case shifter_step_check_status::need_create_shifter_parser:
	result = shifter_create_shifter_parser();
	break;
    default:
	assert(0);
	break;
    }
    m_shifter_step_dispatch_status = result;
    return result;
}

shifter_step_handle_status shifter::shifter_step_handle(shifter_step_dispatch_status status)
{
    shifter_step_handle_status result = shifter_step_handle_status::in_process;
    switch (status) {
	// (shifter_ignore shifter :type 'final_shifted)
    case shifter_step_dispatch_status::final_shifted_ignored:
	break;
	// (shifter_ignore shifter :type 'all_except_first_token)
    case shifter_step_dispatch_status::all_except_first_token_ignored:
	result = shifter_step_handle_status::not_shifted;
	break;
	// (shifter_reducer_reduce shifter)
    case shifter_step_dispatch_status::full_shifted:
	result = shifter_step_handle_status::full_shifted;
	break;
	// (shifter_shift_parse shifter)
    case shifter_step_dispatch_status::not_parsed:
	result = shifter_not_parsed_handler();
	break;
    case shifter_step_dispatch_status::full_parsed:
	shifter_full_parsed_handler();
	// result = shifter_step_handle_status::full_parsed;
	break;
	// (shifter_create_shifter_parser shifter)
    case shifter_step_dispatch_status::parser_created:
	break;
    default:
	assert(0);
	break;
    }
    m_shifter_step_handle_status = result;
    return result;
}

shifter_step_handle_status shifter::shifter_step()
{
    token_type token_type_final(libtemplate::static_cast_reference<reducers::seq, token_type_reduced, token_type>(m_shifter_reducer));


    shifter_step_check_status shifter_step_check_status_ = shifter_step_check();
    shifter_step_dispatch_status shifter_step_dispatch_status_ = shifter_step_dispatch(shifter_step_check_status_);
    shifter_step_handle_status shifter_step_handle_status_ = shifter_step_handle(shifter_step_dispatch_status_);

    LOG(INFO) << "shifter[" 
	      << ((unique_id<shifter>&)(*this)) 
	      << "," 
	      << m_parser_shifter_level 
	      << "," 
	      << token_type_final 
	      << ","
	      << m_shifter_stream.size() << "," << m_parser_stack.size()
    	      << "]::" << __func__ << " "
	      << "check(" << shifter_step_check_status_ << ") "
	      << "dispatch(" << shifter_step_dispatch_status_ << ") "
	      << "handle(" << shifter_step_handle_status_ << ") ";

    return shifter_step_handle_status_;
}

shifter_result shifter::shifter_shift()
{
    bool run = true;
    shifter_result result;
    shifter_step_handle_status status;
    while (run) {
	status = shifter_step();
	switch (status) {
	case shifter_step_handle_status::in_process:
	    break;
	case shifter_step_handle_status::not_shifted:
	    shifter_not_shifted_handler();
	    result.set_shifter_status(shifter_status::not_shifted);
	    run = false;
	    break;
	case shifter_step_handle_status::full_shifted:
	    LOG(INFO) << __func__ << "shifter_step_handle_status::full_shifted";
	    shifter_full_shifted_handler();
	    result.set_shifter_status(shifter_status::full_shifted);
	    static_cast<token&>(result) = m_token_shifted;
	    m_token_shifted = token();
	    run = false;
	    break;
	default:
	    assert(0);
	    break;
	}
    }

    set_enum<shifter_status>(result.get_shifter_status());
    return result;
}

}}
