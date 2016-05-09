#ifndef __PARSER_PARSER_PARSER_RESULT_HPP__
#define __PARSER_PARSER_PARSER_RESULT_HPP__

#include "parser/parser/parser_status.hpp"
#include "parser/token/token.hpp"

namespace parser_tools { namespace parsers {

class parser_result : public token {
    parser_status m_parser_status;

public:
    parser_result(parser_status, token&);
    parser_status get_parser_status();
    void set_parser_status(parser_status);
};

}}
#endif	/* __PARSER_PARSER_PARSER_RESULT_HPP__ */
