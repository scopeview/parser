#include "parser/lexer/lexer_filter.hpp"

namespace parser_tools { namespace lexers {

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
lexer_filter::result_type lexer_filter::lexer_filterate(streams::char_stream& char_stream_)
{
    token t;
    filters::filter::result_type pair = m_filter_op->filterate(char_stream_);
    if (pair.first == filters::filter_status::filtered) {
	t = token(m_token_type, token::token_buf(pair.second));
    }
    return result_type(pair.first, t);
}

}}
