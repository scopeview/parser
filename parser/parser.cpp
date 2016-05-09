#include <glog/logging.h>
#include "parser/parser/parser.hpp"
#include "libtemplate/_.hpp"

namespace parser_tools { namespace parsers {

using namespace parser_tools::reducers;

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
parser::parser(std::shared_ptr<parser_stream>& parser_stream_, reducer_list_type& reducer_list_,
	       const token_type& token_type_final, final_token_checker_policy final_token_checker_policy_,
	       self_parsed_policy self_parsed_policy_, int parser_shifter_level)
    : m_parser_stream(parser_stream_), m_reducer_list(reducer_list_), m_token_type_final(token_type_final),
      m_final_token_checker_policy(final_token_checker_policy_), m_self_parsed_policy(self_parsed_policy_),
      m_parser_shifter_level(parser_shifter_level)
{
    m_reducer_index_stack.push(0);
    set_bool<dummy::is_active>(true);
    set_bool<dummy::is_ignore_final_parsed>(false);
    set_enum<parser_status>(parser_status::none);
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
reducers::abstract_reducer* parser::get_current_reducer()
{
    assert(!m_reducer_index_stack.empty());
    int reducer_index = m_reducer_index_stack.top();
    assert(reducer_index >= 0);
    assert(reducer_index < m_reducer_list.size());
    auto it = m_reducer_list.begin();
    std::advance(it, reducer_index);
    return *it;
}

shifter* parser::get_current_shifter()
{
    if (m_shifter_stack.empty())
	return nullptr;
    else 
	return m_shifter_stack.top().get();
}

std::shared_ptr<parser_stream> parser::get_parser_stream()
{
    return m_parser_stream;
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
bool parser::is_current_reducer_seq_reducer()
{
    bool flag = false;
    if (!is_reducer_index_stack_empty() && !is_current_reducer_end()) {
	reducers::abstract_reducer* abstract_reducer_ = get_current_reducer();
	if (dynamic_cast<reducers::seq*>(abstract_reducer_)) {
	    flag = true;
	}
    }
    return flag;
}

bool parser::is_next_token_first_token_of_seq_reducer()
{
    assert(is_current_reducer_seq_reducer());
    reducers::seq* seq_reducer = dynamic_cast<reducers::seq*>(get_current_reducer());
    token next_token(m_parser_stream->peek_token());
    bool flag = false;
    if (!next_token.is_nil()) {
	reducers::token_type_list_reduced_from& token_type_list_reduced_from_ = *seq_reducer;
	// TODO log_debug
	flag = next_token.is_type(token_type_list_reduced_from_.front());
    }
    return flag;
}

bool parser::is_current_reducer_end()
{
    return !m_reducer_index_stack.empty()
	&& m_reducer_index_stack.top() == m_reducer_list.size();
}

bool parser::is_reducer_index_stack_empty()
{
    return m_reducer_index_stack.empty();
}

bool parser::is_current_shifter_active()
{
    bool flag = false;
    shifter* shifter_ = get_current_shifter();
    if (shifter_ && shifter_->is_true<dummy::is_active>())
	flag = true;
    return flag;
}

bool parser::has_token_reduced_or_shifted()
{
    return !m_token_reduced_or_shifted.is_nil();
}

bool parser::is_next_token_final_token()
{
    bool flag = false;
    token next_token(m_parser_stream->peek_token());
    if (!next_token.is_nil()
	&& next_token.is_type(m_token_type_final)) {
	flag = true;
    }
    return flag;
}

bool parser::is_token_reduced_or_shifted_final_token()
{
    bool flag = false;
    if (!m_token_reduced_or_shifted.is_nil()
	&& m_token_reduced_or_shifted.is_type(m_token_type_final)) {
	flag = true;
    }
    return flag;
}

bool parser::is_ignore_final_parsed()
{
    return is_true<dummy::is_ignore_final_parsed>();
}

void parser::set_ignore_final_parsed()
{
    set_bool<dummy::is_ignore_final_parsed>(true);
}

void parser::clear_ignore_final_parsed()
{
    set_bool<dummy::is_ignore_final_parsed>(false);
}

bool parser::is_current_token_shifted_by_current_shifter()
{
    assert(is_current_reducer_seq_reducer());
    shifter* shifter_ = get_current_shifter();
    token t(m_parser_stream->peek_token());
    // return t.is_type((token_type&)(token_type_reduced&)(reducers::seq&)(shifter_reducer&)(*shifter_));
    return t.is_type(shifter_->get_final_token_type());
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
void parser::set_reducer_index(set_reducer_index_type type)
{
    assert(m_reducer_index_stack.size());
    assert(!is_current_reducer_end());

    int top = m_reducer_index_stack.top();
    switch (type) {
    case set_reducer_index_type::not_shifted:
	assert(is_current_reducer_seq_reducer());
	m_reducer_index_stack.pop();
	m_reducer_index_stack.push(top + 1);
	break;
    case set_reducer_index_type::full_shifted:
	assert(is_current_reducer_seq_reducer());
	m_reducer_index_stack.pop();
	m_reducer_index_stack.push(top + 1);
	m_reducer_index_stack.push(0);
	break;
    case set_reducer_index_type::not_reduced:
	assert(!is_current_reducer_seq_reducer());
	m_reducer_index_stack.pop();
	m_reducer_index_stack.push(top + 1);
	break;
    case set_reducer_index_type::full_reduced:
	assert(!is_current_reducer_seq_reducer());
	m_reducer_index_stack.pop();
	m_reducer_index_stack.push(top + 1);
	m_reducer_index_stack.push(0);
	break;
    case set_reducer_index_type::ignore_shifter:
	assert(is_current_reducer_seq_reducer());
	m_reducer_index_stack.pop();
	m_reducer_index_stack.push(top + 1);
	break;
    default:
	assert(0);
	break;
    }
}

bool parser::status_p(parser_status parser_status_)
{
    return get_enum<parser_status>() == parser_status_;
}

void parser::set_status(parser_status parser_status_)
{
    set_enum<parser_status>(parser_status_);
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
parser_step_dispatch_status parser::parser_check_create_shifter()
{
    assert(!is_current_shifter_active());
    assert(is_current_reducer_seq_reducer());
    parser_step_dispatch_status result = parser_step_dispatch_status::none;
    if (is_next_token_first_token_of_seq_reducer()) {
	parser_create_shifter_internal();
	result = parser_step_dispatch_status::shifter_created;
    } else {
	set_reducer_index(set_reducer_index_type::ignore_shifter);
	result = parser_step_dispatch_status::shifter_neednot_created;
    }
    m_parser_step_dispatch_status = result;
    return result;
}

void parser::parser_create_shifter_internal()
{
    reducers::seq& seq_ = *dynamic_cast<reducers::seq*>(get_current_reducer());
    token_type token_type_final(libtemplate::static_cast_reference<reducers::seq, token_type_reduced, token_type>(seq_));
    LOG(INFO) << __func__ << " " << token_type_final;

    m_shifter_stack.push(std::make_shared<shifter>(
			     m_reducer_index_stack.top(),
			     m_reducer_index_stack.size(),
			     seq_,
			     this,
			     m_parser_shifter_level + 1));
}

parser_step_dispatch_status parser::parser_shift()
{
    assert(is_current_shifter_active());
    assert(is_current_reducer_seq_reducer());
    assert(!has_token_reduced_or_shifted());
    return parser_shift_internal();
}

parser_step_dispatch_status parser::parser_shift_internal()
{
    shifter* shifter_ = get_current_shifter();
    shifter_result shifter_result_(shifter_->shifter_shift());
    parser_step_dispatch_status result = parser_step_dispatch_status::none;
    switch (shifter_result_.get_shifter_status()) {
    case shifter_status::not_shifted:
	result = parser_step_dispatch_status::not_shifted;
	break;
    case shifter_status::full_shifted:
	result = parser_step_dispatch_status::full_shifted;
	LOG(INFO) << __func__ << "parser_step_dispatch_status::full_shifted";
	m_token_reduced_or_shifted = shifter_result_;
	break;
    default:
	assert(0);
	break;
    }
    m_parser_step_dispatch_status = result;
    return result;
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
parser_step_dispatch_status parser::parser_reduce()
{
    assert(!is_current_reducer_seq_reducer());
    assert(!is_current_shifter_active());
    assert(!has_token_reduced_or_shifted());
    return parser_reduce_internal();
}

parser_step_dispatch_status parser::parser_reduce_internal()
{
    reducers::abstract_reducer* reducer = get_current_reducer();
    reducer_result reducer_result_ = reducer->reduce(*m_parser_stream);
    parser_step_dispatch_status result = parser_step_dispatch_status::none;
    switch (reducer_result_.get_reducer_status()) {
    case reducer_status::not_reduced:
	// do nothing
	result = parser_step_dispatch_status::not_reduced;
	break;
    case reducer_status::partial_reduced:
	assert(0);
	break;
    case reducer_status::full_reduced:
	result = parser_step_dispatch_status::full_reduced;
	m_token_reduced_or_shifted = reducer_result_.get_token();
	break;
    default:
	assert(0);
	break;
    }
    m_parser_step_dispatch_status = result;
    return result;
}

void parser::parser_undo(parser_undo_type type)
{
    switch (type) {
    case parser_undo_type::all:
	parser_undo_all();
	break;
    case parser_undo_type::parsed_token_stream:
	parser_undo_parsed_token_streamer();
	break;
    case parser_undo_type::current_reduced:
	parser_undo_current_reduced();
	break;
    default:
	assert(0);
	break;
    }
}

void parser::parser_undo_all()
{
    assert(is_reducer_index_stack_empty());
    // log_debug();
    m_parser_stream->undo(parser_stream_undo_type::all);
}

void parser::parser_undo_parsed_token_streamer()
{
    assert(is_reducer_index_stack_empty());
    // TODO
    // log_debug();
    m_parser_stream->undo(parser_stream_undo_type::parsed_token_stream);
}

void parser::parser_undo_current_reduced()
{
    assert(!is_current_reducer_seq_reducer());
    // TODO last-step-status should be full-reduced
    token token_reduced(m_parser_stream->read_token());
    assert(!token_reduced.is_nil());
    // :type 'to-reduced-from
    if (token_reduced.is_terminal()) {
	m_parser_stream->unread_token(token_reduced);
    } else {
	m_parser_stream->unread_token(static_cast<token_list&>(token_reduced));
    }
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
parser_step_dispatch_status parser::parser_ignore(parser_ignore_type type)
{
    parser_step_dispatch_status result = parser_step_dispatch_status::none;
    switch (type) {
    case parser_ignore_type::last_one_last_layer:
	result = parser_ignore_last_one_last_layer();
	break;
    case parser_ignore_type::last_one_current_layer:
	result = parser_ignore_last_one_current_layer();
	break;
    case parser_ignore_type::current_one:
	result = parser_ignore_current_one();
	break;
    case parser_ignore_type::current_reduced:
	result = parser_ignore_current_reduced();
	break;
    case parser_ignore_type::current_shifted:
	result = parser_ignore_current_shifted();
	break;
    case parser_ignore_type::final_parsed:
	result = parser_ignore_final_parsed();
	break;
    case parser_ignore_type::final_parsed_as_reduced:
	result = parser_ignore_final_parsed_as_reduced();
	break;
    case parser_ignore_type::final_parsed_as_shifted:
	result = parser_ignore_final_parsed_as_shifted();
	break;
    default:
	assert(0);
	break;
    }
    m_parser_step_dispatch_status = result;
    return result;
}

parser_step_dispatch_status parser::parser_ignore_last_one_last_layer()
{
    assert(is_current_reducer_end());
    parser_step_dispatch_status result = parser_step_dispatch_status::none;
    m_reducer_index_stack.pop();
    if (!m_reducer_index_stack.empty()) {
	result = parser_ignore(parser_ignore_type::last_one_current_layer);
    } else {
	result = parser_step_dispatch_status::ignore_nothing;
    }
    m_parser_step_dispatch_status = result;
    return result;
}

parser_step_dispatch_status parser::parser_ignore_last_one_current_layer()
{
    assert(!m_reducer_index_stack.empty());
    assert(m_reducer_index_stack.top() != 0);
    parser_step_dispatch_status result = parser_step_dispatch_status::none;

    int reducer_index = m_reducer_index_stack.top();
    m_reducer_index_stack.pop();
    m_reducer_index_stack.push(--reducer_index);
    result = parser_ignore(parser_ignore_type::current_one);

    m_parser_step_dispatch_status = result;
    return result;
}

parser_step_dispatch_status parser::parser_ignore_current_one()
{
    parser_step_dispatch_status result = parser_step_dispatch_status::none;
    if (is_current_reducer_seq_reducer()) {
	result = parser_ignore(parser_ignore_type::current_shifted);
    } else {
	result = parser_ignore(parser_ignore_type::current_reduced);
    }
    m_parser_step_dispatch_status = result;
    return result;
}

parser_step_dispatch_status parser::parser_ignore_current_shifted()
{
    assert(is_current_reducer_seq_reducer());
    assert(!is_current_shifter_active());
    assert(is_current_token_shifted_by_current_shifter());
    assert(!m_shifter_stack.empty());
    parser_step_dispatch_status result = parser_step_dispatch_status::none;
    shifter* shifter_ = get_current_shifter();
    assert(m_reducer_index_stack.size() == shifter_->get_reducer_index_stack_length());

    shifter_->set_bool<dummy::is_ignore_final_shifted>(true);
    shifter_->set_bool<dummy::is_active>(true);

    result = parser_step_dispatch_status::shifted_ignored;
    m_parser_step_dispatch_status = result;
    return result;
}

parser_step_dispatch_status parser::parser_ignore_current_reduced()
{
    assert(!is_current_reducer_seq_reducer());
    parser_step_dispatch_status result = parser_step_dispatch_status::none;
    parser_undo(parser_undo_type::current_reduced);

    int reducer_index = m_reducer_index_stack.top();
    m_reducer_index_stack.pop();
    m_reducer_index_stack.push(++reducer_index);

    result = parser_step_dispatch_status::reduced_ignored;
    m_parser_step_dispatch_status = result;
    return result;
}

parser_step_dispatch_status parser::parser_ignore_final_parsed()
{
    parser_step_dispatch_status result = parser_step_dispatch_status::none;
    if (is_current_reducer_seq_reducer()) {
	parser_ignore(parser_ignore_type::final_parsed_as_shifted);
    } else {
	parser_ignore(parser_ignore_type::final_parsed_as_reduced);
    }
    clear_ignore_final_parsed();

    result = parser_step_dispatch_status::final_parsed_ignored;
    m_parser_step_dispatch_status = result;
    return result;
}

parser_step_dispatch_status parser::parser_ignore_final_parsed_as_reduced()
{
    assert(!is_current_reducer_seq_reducer());
    assert(!is_current_shifter_active());
    parser_step_dispatch_status result = parser_step_dispatch_status::none;
    if (!m_reducer_index_stack.empty()) {
	set_reducer_index(set_reducer_index_type::full_reduced);
    } else {
	m_self_parsed_policy = self_parsed_policy::not_allow_self_parsed;
    }
    return result;
}

parser_step_dispatch_status parser::parser_ignore_final_parsed_as_shifted()
{
    assert(is_current_reducer_seq_reducer());
    assert(is_current_shifter_active());
    assert(!m_shifter_stack.empty());
    
    parser_step_dispatch_status result = parser_step_dispatch_status::none;
    shifter* shifter_ = get_current_shifter();
    assert(m_reducer_index_stack.size() == shifter_->get_reducer_index_stack_length());

    shifter_->set_bool<dummy::is_ignore_final_shifted>(true);
    shifter_->set_bool<dummy::is_active>(true);

    return result;
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
void parser::parser_step_not_shifted_handler()
{
    assert(is_current_reducer_seq_reducer());
    assert(is_current_shifter_active());
    assert(!has_token_reduced_or_shifted());

    m_shifter_stack.pop();
    set_reducer_index(set_reducer_index_type::not_shifted);
}

void parser::parser_step_full_shifted_handler()
{
    LOG(INFO) << __func__;
    assert(is_current_reducer_seq_reducer());
    assert(is_current_shifter_active());
    assert(has_token_reduced_or_shifted());
    m_parser_stream->unread_token(m_token_reduced_or_shifted);

    // clean
    m_token_reduced_or_shifted = token();
    shifter* shifter_ = get_current_shifter();
    shifter_->set_bool<dummy::is_active>(false);

    set_reducer_index(set_reducer_index_type::full_shifted);
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
void parser::parser_step_not_reduced_handler()
{
    assert(!is_current_reducer_seq_reducer());
    assert(!is_current_shifter_active());
    assert(!has_token_reduced_or_shifted());
    set_reducer_index(set_reducer_index_type::not_reduced);
}

void parser::parser_step_full_reduced_handler()
{
    assert(!is_current_reducer_seq_reducer());
    assert(!is_current_shifter_active());
    assert(has_token_reduced_or_shifted());
    
    m_parser_stream->unread_token(m_token_reduced_or_shifted);
    // clean
    m_token_reduced_or_shifted = token();
    set_reducer_index(set_reducer_index_type::full_reduced);
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
parser_step_dispatch_status parser::parser_step_final_token_checker()
{
    assert(has_token_reduced_or_shifted());
    assert(!is_current_reducer_end());
    assert(!is_reducer_index_stack_empty());
    assert(is_token_reduced_or_shifted_final_token());
    parser_step_dispatch_status result = parser_step_dispatch_status::none;

    switch (m_final_token_checker_policy) {
    case final_token_checker_policy::return_immediately:
    {
	if (is_current_reducer_seq_reducer()) {
	    result = parser_step_dispatch_status::full_parsed_as_shifted;
	} else {
	    result = parser_step_dispatch_status::full_parsed_as_reduced;
	}
    }
    break;
    case final_token_checker_policy::parse_until_eof:
    {
	token try_last_token(m_parser_stream->read_token());
	if (!try_last_token.is_nil()) {
	    m_parser_stream->unread_token(try_last_token);
	    if (is_current_reducer_seq_reducer()) {
		result = parser_step_dispatch_status::just_full_shifted;
	    } else {
		result = parser_step_dispatch_status::just_full_reduced;
	    }
	} else {
	    if (is_current_reducer_seq_reducer()) {
		result = parser_step_dispatch_status::full_parsed_as_shifted;
	    } else {
		result = parser_step_dispatch_status::full_parsed_as_reduced;
	    }
	}
    }
    break;
    default:
	assert(0);
	break;
    }
    m_parser_step_dispatch_status = result;
    return result;
}

parser_step_dispatch_status parser::parser_step_check_token_reduced_or_shifted_handler()
{
    assert(has_token_reduced_or_shifted());
    assert(!is_current_reducer_end());
    assert(!is_reducer_index_stack_empty());
    parser_step_dispatch_status result = parser_step_dispatch_status::none;

    if (is_token_reduced_or_shifted_final_token()) {
	result = parser_step_final_token_checker();
    } else {
	if (is_current_reducer_seq_reducer()) {
	    result = parser_step_dispatch_status::just_full_shifted;
	} else {
	    result = parser_step_dispatch_status::just_full_reduced;
	}
    }
    m_parser_step_dispatch_status = result;
    return result;
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
parser_step_dispatch_status parser::parser_step_check_not_parsed_handler()
{
    assert(is_reducer_index_stack_empty());
    assert(!has_token_reduced_or_shifted());
    parser_step_dispatch_status result = parser_step_dispatch_status::none;
    token last_token(m_parser_stream->read_token());

    if (m_self_parsed_policy == self_parsed_policy::allow_self_parsed
	&& !last_token.is_nil()
	&& last_token.is_type(m_token_type_final)) {
	result = parser_step_dispatch_status::self_parsed;
	m_token_reduced_or_shifted = last_token;
    } else {
	if (!last_token.is_nil()) {
	    m_parser_stream->unread_token(last_token);
	}
	m_parser_stream->undo(parser_stream_undo_type::parsed_token_stream);
	result = parser_step_dispatch_status::not_parsed;
    }
    m_parser_step_dispatch_status = result;
    return result;
}

void parser::parser_step_full_parsed_as_reduced_handler()
{
    assert(!is_current_reducer_seq_reducer());
    assert(!is_current_shifter_active());
    assert(has_token_reduced_or_shifted());
}

void parser::parser_step_full_parsed_as_shifted_handler()
{
    assert(is_current_reducer_seq_reducer());
    assert(is_current_shifter_active());
    assert(has_token_reduced_or_shifted());
}

parser_step_dispatch_status parser::parser_step_layer_not_reduced_handler()
{
    assert(!is_reducer_index_stack_empty());
    assert(is_current_reducer_end());
    assert(!is_current_shifter_active());
    return parser_ignore(parser_ignore_type::last_one_last_layer);
}


////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
parser_step_dispatch_status parser::parser_step_return_next_token_as_final_token_handler()
{
    parser_step_dispatch_status result = parser_step_dispatch_status::none;
    token next_token(m_parser_stream->read_token());
    assert(!next_token.is_nil());
    assert(next_token.is_type(m_token_type_final));
    m_token_reduced_or_shifted = next_token;

    result = parser_step_dispatch_status::next_token_is_final_token_type;
    m_parser_step_dispatch_status = result;
    return result;
}


////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
parser_step_check_status parser::parser_step_check()
{
    auto is_need_ignore_final_parsed = [&]() {return is_ignore_final_parsed();};
    auto is_need_check_exit_as_not_parsed = [&]() {return is_reducer_index_stack_empty();};
    auto is_need_check_layer_not_reduced = [&]() {return is_current_reducer_end();};
    auto is_need_check_token_reduced_or_shifted = [&]() {return has_token_reduced_or_shifted();};
    auto is_need_check_final_token = [&]() {return is_token_reduced_or_shifted_final_token();};
    auto is_need_check_create_shifter = [&]() {return is_current_reducer_seq_reducer()
					       && !is_current_shifter_active();};
    auto is_need_shift = [&]() {return is_current_shifter_active();};
    auto is_need_reduce = [&]() {return !is_current_shifter_active()
				 && !is_current_reducer_seq_reducer()
				 && !is_current_reducer_end();};

    parser_step_check_status result = parser_step_check_status::none;
    if (is_need_ignore_final_parsed())
	result = parser_step_check_status::need_ignore_final_parsed;
    else if (is_need_check_exit_as_not_parsed())
	result = parser_step_check_status::need_check_exit_as_not_parsed;
    else if (is_need_check_token_reduced_or_shifted())
	result = parser_step_check_status::need_check_token_reduced_or_shifted;
    else if (is_need_check_layer_not_reduced())
	result = parser_step_check_status::need_check_layer_not_reduced;
    else if (is_need_check_create_shifter())
	result = parser_step_check_status::need_check_create_shifter;
    else if (is_need_shift())
	result = parser_step_check_status::need_shift;
    else if (is_need_reduce())
	result = parser_step_check_status::need_reduce;
    else 
	assert(0);
    m_parser_step_check_status = result;
    return result;
}

parser_step_dispatch_status parser::parser_step_dispatch(parser_step_check_status status)
{
    parser_step_dispatch_status result = parser_step_dispatch_status::none;
    switch (status) {
    case parser_step_check_status::need_ignore_final_parsed:
	result = parser_ignore(parser_ignore_type::final_parsed);
	break;
    case parser_step_check_status::need_check_exit_as_not_parsed:
	result = parser_step_check_not_parsed_handler();
	break;
    case parser_step_check_status::need_check_token_reduced_or_shifted:
	result = parser_step_check_token_reduced_or_shifted_handler();
	break;
    case parser_step_check_status::need_check_layer_not_reduced:
	result = parser_step_layer_not_reduced_handler();
	break;
    case parser_step_check_status::need_return_next_token_as_final_token:
	result = parser_step_return_next_token_as_final_token_handler();
	break;
    case parser_step_check_status::need_check_create_shifter:
	result = parser_check_create_shifter();
	break;
    case parser_step_check_status::need_shift:
	result = parser_shift();
	break;
    case parser_step_check_status::need_reduce:
	result = parser_reduce();
	break;
    default:
	assert(0);
	break;
    }
    return result;
}

parser_step_handle_status parser::parser_step_handle(parser_step_dispatch_status status)
{
    int reducer_index_stack_top;
    if (m_reducer_index_stack.size()) {
	reducer_index_stack_top = m_reducer_index_stack.top();
    } else {
	reducer_index_stack_top = -1;
    }

    // LOG(INFO) << "parser[" 
    // 	      << ((unique_id<parser>&)(*this)) 
    // 	      << "," 
    // 	      << m_parser_shifter_level 
    // 	      << "," 
    // 	      << m_token_type_final 
    // 	      << "]::" << __func__ << " "
    // 	      << "parser_step_dispatch_status(" << status << ")";
    parser_step_handle_status result = parser_step_handle_status::in_process;
    switch (status) {
	// handle (parser_ignore parser :type 'final_parsed))
    case parser_step_dispatch_status::final_parsed_ignored:
	LOG(INFO) << "parser[" 
		  << ((unique_id<parser>&)(*this)) 
		  << "," 
		  << m_parser_shifter_level 
		  << "," 
		  << m_token_type_final 
		  << ","
		  << reducer_index_stack_top << "/" << m_reducer_list.size() << "@" << m_reducer_index_stack.size()
		  << "]::" << __func__ << " "
		  << "dispatch(" << status << ") ";
	// LOG(INFO) << "parser: " << ((unique_id<parser>&)(*this));
	// LOG(INFO) << "parser: " << (unique_id<parser>&)(*this) << status;
	break;
	// handle parser_step_check_not_parsed_handler
    case parser_step_dispatch_status::not_parsed:
	// LOG(INFO) << "parser: " << ((unique_id<parser>&)(*this));
	result = parser_step_handle_status::not_parsed;
	break;
    case parser_step_dispatch_status::self_parsed:
	// LOG(INFO) << "parser: " << ((unique_id<parser>&)(*this));
	result = parser_step_handle_status::full_parsed;
	break;
	// handle parser_step_check_token_reduced_or_shifted_handler
    case parser_step_dispatch_status::just_full_reduced:
	LOG(INFO) << "parser[" 
		  << ((unique_id<parser>&)(*this)) 
		  << "," 
		  << m_parser_shifter_level 
		  << "," 
		  << m_token_type_final 
		  << ","
		  << reducer_index_stack_top << "/" << m_reducer_list.size() << "@" << m_reducer_index_stack.size()
		  << "]::" << __func__ << " "
	    // << "dispatch(" << status << "," << static_cast<token_type&>(m_token_reduced_or_shifted) << ") ";
		  << "dispatch(" << status << "," << m_token_reduced_or_shifted.to_string() << ") ";
	parser_step_full_reduced_handler();
	break;
    case parser_step_dispatch_status::just_full_shifted:
	LOG(INFO) << "parser[" 
		  << ((unique_id<parser>&)(*this)) 
		  << "," 
		  << m_parser_shifter_level 
		  << "," 
		  << m_token_type_final 
		  << ","
		  << reducer_index_stack_top << "/" << m_reducer_list.size() << "@" << m_reducer_index_stack.size()
		  << "]::" << __func__ << " "
	    // << "dispatch(" << status << "," << static_cast<token_type&>(m_token_reduced_or_shifted) << ") ";
		  << "dispatch(" << status << "," << m_token_reduced_or_shifted.to_string() << ") ";
	parser_step_full_shifted_handler();
	break;
    case parser_step_dispatch_status::full_parsed_as_reduced:
	LOG(INFO) << "parser[" 
		  << ((unique_id<parser>&)(*this)) 
		  << "," 
		  << m_parser_shifter_level 
		  << "," 
		  << m_token_type_final 
		  << ","
		  << reducer_index_stack_top << "/" << m_reducer_list.size() << "@" << m_reducer_index_stack.size()
		  << "]::" << __func__ << " "
		  << "dispatch(" << status << ") ";
	parser_step_full_parsed_as_reduced_handler();
	result = parser_step_handle_status::full_parsed;
	break;
    case parser_step_dispatch_status::full_parsed_as_shifted:
	LOG(INFO) << "parser[" 
		  << ((unique_id<parser>&)(*this)) 
		  << "," 
		  << m_parser_shifter_level 
		  << "," 
		  << m_token_type_final 
		  << ","
		  << reducer_index_stack_top << "/" << m_reducer_list.size() << "@" << m_reducer_index_stack.size()
		  << "]::" << __func__ << " "
		  << "dispatch(" << status << ") ";
	parser_step_full_parsed_as_shifted_handler();
	LOG(INFO) << "parser: " << ((unique_id<parser>&)(*this));
	result = parser_step_handle_status::full_parsed;
	break;
	// handle parser_step_layer_not_reduced_handler
    case parser_step_dispatch_status::shifted_ignored:
    case parser_step_dispatch_status::reduced_ignored:
    case parser_step_dispatch_status::ignore_nothing:
	// parser_step_return_next_token_as_final_token_handler
    case parser_step_dispatch_status::next_token_is_final_token_type:
	// handle parser_create_shifter
    case parser_step_dispatch_status::shifter_created:
    case parser_step_dispatch_status::shifter_neednot_created:
	break;
	// handle parser_shift
    case parser_step_dispatch_status::not_shifted:
	parser_step_not_shifted_handler();
	break;
    case parser_step_dispatch_status::full_shifted:
	LOG(INFO) << "parser[" 
		  << ((unique_id<parser>&)(*this)) 
		  << "," 
		  << m_parser_shifter_level 
		  << "," 
		  << m_token_type_final 
		  << ","
		  << reducer_index_stack_top << "/" << m_reducer_list.size() << "@" << m_reducer_index_stack.size()
		  << "]::" << __func__ << " "
		  << "dispatch(" << status << ") ";
	// LOG(INFO) << __func__ << "parser_step_dispatch_status::full_shifted";
	break;
	// handle parser_reduce
    case parser_step_dispatch_status::not_reduced:
	parser_step_not_reduced_handler();
	break;
    case parser_step_dispatch_status::full_reduced:
	break;
    default:
	assert(0);
	break;
    }
    m_parser_step_handle_status = result;
    return result;
}

parser_step_handle_status parser::parser_step()
{
    parser_step_check_status parser_step_check_status_;
    parser_step_dispatch_status parser_step_dispatch_status_;
    parser_step_handle_status parser_step_handle_status_;

    int reducer_index_stack_top;
    if (m_reducer_index_stack.size()) {
	reducer_index_stack_top = m_reducer_index_stack.top();
    } else {
	reducer_index_stack_top = -1;
    }

    parser_step_check_status_		= parser_step_check();
    parser_step_dispatch_status_	= parser_step_dispatch(parser_step_check_status_);
    parser_step_handle_status_		= parser_step_handle(parser_step_dispatch_status_);

#if 1
    LOG(INFO) << "parser[" 
	      << ((unique_id<parser>&)(*this)) 
	      << "," 
	      << m_parser_shifter_level 
	      << "," 
	      << m_token_type_final 
	      << ","
	      << reducer_index_stack_top << "/" << m_reducer_list.size() << "@" << m_reducer_index_stack.size()
    	      << "]::" << __func__ << " "
	      << "check(" << parser_step_check_status_ << ") "
	      << "dispatch(" << parser_step_dispatch_status_ << ") "
	      << "handle(" << parser_step_handle_status_ << ") ";
#endif

    return parser_step_handle_status_;
}

parser_result parser::parser_parse()
{
    bool run = true;
    token t;
    parser_step_handle_status status;
    while (run) {
	status = parser_step();
	switch (status) {
	case parser_step_handle_status::in_process:
	    break;
	case parser_step_handle_status::not_parsed:
	    parser_undo(parser_undo_type::parsed_token_stream);
	    set_enum<parser_status>(parser_status::not_parsed);
	    run = false;
	    break;
	case parser_step_handle_status::full_parsed:
	    set_enum<parser_status>(parser_status::full_parsed);
	    t = m_token_reduced_or_shifted;
	    m_token_reduced_or_shifted = token();
	    run = false;
	    break;
	default:
	    assert(0);
	    break;
	}
    }
    return parser_result(get_enum<parser_status>(), t);
}

void parser_tools_init()
{
    const char* ptr = "parser";
    FLAGS_log_prefix = false;
    google::InitGoogleLogging(ptr);
}

}}
