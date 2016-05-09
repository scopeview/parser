#include "parser/parser/parser_result.hpp"

namespace parser_tools { namespace parsers {

parser_result::parser_result(parser_status status, token& t) 
    : m_parser_status(status), token(t)
{}

////////////////////////////////////////////////////////////////
parser_status parser_result::get_parser_status()
{
    return m_parser_status;
}

void parser_result::set_parser_status(parser_status status)
{
    m_parser_status = status;
}

}}
