#include <list>
#include "parser/lexer/lexer_filter_group.hpp"

namespace parser_tools { namespace lexers {

lexer_filter_group::lexer_filter_group() 
{}

lexer_filter_group::lexer_filter_group(const lexer_filter_group_name& name) 
    : lexer_filter_group_name(name)
{}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
void lexer_filter_group::push_back(const lexer_filter& filter)
{
    static_cast<filter_list&>(*this).push_back(filter);
}

lexer_filter_group::result_type lexer_filter_group::filterate(streams::char_stream& char_stream_)
{
    result_type result;
    while (1) {
	result = filterate_single_pass(char_stream_);
	if (result.first == filters::filter_status::eof)
	    break;

	if (result.first == filters::filter_status::filtered
	    && !result.second.is_nil()) {
	    break;
	}
    }
    return result;
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
lexer_filter_group::result_type lexer_filter_group::filterate_single_pass(streams::char_stream& char_stream_)
{
    result_type result;
    bool no_filter_filtered = true;
    filter_list& filter_list_ = *this;
    filters::filter_status filter_status_;
    
    for (auto it = filter_list_.begin(); it != filter_list_.end(); ++it) {
	result = it->lexer_filterate(char_stream_);
	if (result.first == filters::filter_status::filtered
	    || result.first == filters::filter_status::eof) {
	    no_filter_filtered = false;
	    break;
	}
    }
    if (no_filter_filtered) {
	int c = char_stream_.read_char();
	if (c == streams::char_stream::eof) {
	    result.first = filters::filter_status::eof;
	} else {
	    char_stream_.unread_char(c);
	    result.first = filters::filter_status::none;
	}
    }

    return result;
}

}}
