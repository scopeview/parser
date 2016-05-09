#include "parser/lexer/lexer.hpp"

namespace parser_tools { namespace lexers {

lexer::lexer(std::list<char>& char_list) : char_stream(char_list)
{}
lexer::lexer(std::string& string) : char_stream(string)
{}
lexer::lexer(filename& filename_) : char_stream(filename_)
{}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
void lexer::add(lexer_filter_group& filter_group)
{
    filter_group_list&filter_group_list_ = *this;
    filter_group_list_.push_back(filter_group);
}

void lexer::set_filter_group(const lexer_filter_group_name& name)
{
    filter_group_name_stack& filter_group_name_stack_ = *this;
    filter_group_name_stack_.push(name);
}

////////////////////////////////////////////////////////////////
// abstract_token_stream_operation_common
////////////////////////////////////////////////////////////////
token lexer::peek_token()
{
    if (m_parsed_token_stream.size()) {
	return m_parsed_token_stream.peek_token();
    } else {
	lexer_filter_group& current = get_current_filter_group();
	return peek_token(current);
    }
}

token lexer::read_token()
{
    if (m_parsed_token_stream.size()) {
	return m_parsed_token_stream.read_token();
    } else {
	lexer_filter_group& current = get_current_filter_group();
	return read_token(current);
    }
}

void lexer::unread_token(const token& t)
{
    m_parsed_token_stream.unread_token(t);
}

void lexer::unread_token(const token_list& list)
{
    m_parsed_token_stream.unread_token(list);
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
token lexer::peek_token(lexer_filter_group& filter_group_)
{
    token t = read_token(filter_group_);
    if (!t.is_nil()) {
	unread_token(t);
    }
    return t;
}

token lexer::read_token(lexer_filter_group& filter_group_)
{
    streams::char_stream& char_stream_ = *this;
    lexer_filter::result_type result = filter_group_.filterate(char_stream_);
    if (result.first == filters::filter_status::eof) {
	return token();
    } else {
	assert(result.first == filters::filter_status::filtered);
	return result.second;
    }
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
lexer_filter_group& lexer::get_current_filter_group()
{
    filter_group_name_stack& filter_group_name_stack_ = *this;
    filter_group_list& filter_group_list_ = *this;
    lexer_filter_group_name filter_group_name_(filter_group_name_stack_.top());
    auto it = std::find_if(filter_group_list_.begin(), filter_group_list_.end(),
			   [&](lexer_filter_group& fg) {
			       lexer_filter_group_name& name = fg;
			       return name == filter_group_name_;
			   });
    assert(it != filter_group_list_.end());
    return *it;
}

}}

