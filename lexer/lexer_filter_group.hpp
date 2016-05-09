#ifndef __PARSER_LEXER_LEXER_FILTER_GROUP_HPP__
#define __PARSER_LEXER_LEXER_FILTER_GROUP_HPP__

#include <list>
#include "parser/lexer/lexer_filter_group_name.hpp"
#include "parser/lexer/lexer_filter.hpp"
#include "parser/filter/filter.hpp"

namespace parser_tools { namespace lexers {

class lexer_filter_group : public lexer_filter_group_name,
			   public std::list<lexer_filter>
{
public:
    typedef lexer_filter::result_type result_type;
    typedef std::list<lexer_filter> filter_list;

private:
    result_type filterate_single_pass(streams::char_stream&);
    
public:
    lexer_filter_group();
    lexer_filter_group(const lexer_filter_group_name&);

    void push_back(const lexer_filter&);

    result_type filterate(streams::char_stream&);
};

}}
#endif	/* __PARSER_LEXER_LEXER_FILTER_GROUP_HPP__ */
