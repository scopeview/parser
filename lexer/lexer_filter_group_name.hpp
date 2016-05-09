#ifndef __PARSER_LEXER_LEXER_FILTER_GROUP_NAME_HPP__
#define __PARSER_LEXER_LEXER_FILTER_GROUP_NAME_HPP__

#include <string>

namespace parser_tools { namespace lexers {

class lexer_filter_group_name : public std::string {
public:
    lexer_filter_group_name() {}
    lexer_filter_group_name(const std::string& name) : std::string(name) {}
};

}}
#endif	/* __PARSER_LEXER_LEXER_FILTER_GROUP_NAME_HPP__ */
