#include "parser/parser/parser_stream.hpp"

namespace parser_tools { namespace parsers {

parser_stream::parser_stream(streams::abstract_token_stream_operation_common& lexer_op)
    : m_lexer(&lexer_op)
{
    m_parsed_token_stream = std::shared_ptr<streams::token_stream>(new streams::token_stream);
    m_token_streamer_read_token_list = std::shared_ptr<token_list>(new token_list);
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
std::shared_ptr<parser_stream> parser_stream::copy_class()
{
    std::shared_ptr<parser_stream> new_parser_stream(new parser_stream(*this));
    new_parser_stream->m_token_streamer_read_token_list = std::shared_ptr<token_list>(new token_list);
    return new_parser_stream;
}


////////////////////////////////////////////////////////////////
// abstract_token_stream_operation_common
////////////////////////////////////////////////////////////////
token parser_stream::peek_token()
{
    if (!m_parsed_token_stream->size()) {
	token t = m_lexer->read_token();
	m_parsed_token_stream->add_tail_token(t);
    }
    return m_parsed_token_stream->peek_token();
}

token parser_stream::read_token()
{
    if (!m_parsed_token_stream->size()) {
	token t = m_lexer->read_token();
	m_parsed_token_stream->add_tail_token(t);
	m_token_streamer_read_token_list->push_back(t);
    }
    return m_parsed_token_stream->read_token();
}

void parser_stream::unread_token(const token& t)
{
    m_parsed_token_stream->unread_token(t);
}

void parser_stream::unread_token(const token_list& list)
{
    m_parsed_token_stream->unread_token(list);
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
void parser_stream::undo(parser_stream_undo_type type)
{
    switch (type) {
    case parser_stream_undo_type::parsed_token_stream:
    {
	token_list tlist(m_parsed_token_stream->read_token_all());
	token_list orignal_tlist;
	std::for_each(tlist.begin(), tlist.end(), [&](token& t) {
		token_list sub_list(t.to_orignal_token_list());
		std::for_each(sub_list.begin(), sub_list.end(), [&](const token& sub_t) {
			orignal_tlist.push_back(sub_t);
		    });
	    });
	m_lexer->unread_token(orignal_tlist);
    }
    break;
    case parser_stream_undo_type::all:
    {
	m_lexer->unread_token(*m_token_streamer_read_token_list);
    }
    break;
    }
}

}}
