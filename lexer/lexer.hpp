#ifndef __PARSER_LEXER_LEXER_HPP__
#define __PARSER_LEXER_LEXER_HPP__

#include <stack>
#include "parser/stream/char_stream.hpp"
#include "parser/stream/token_stream.hpp"
#include "parser/stream/abstract_token_stream_operation.hpp"
#include "parser/token/_.hpp"
#include "parser/filter/_.hpp"
#include "parser/lexer/lexer_filter_group_name.hpp"
#include "parser/lexer/lexer_filter_group.hpp"

namespace parser_tools { namespace lexers {

class lexer : public streams::char_stream,
	      public streams::abstract_token_stream_operation_common,
	      public std::list<lexer_filter_group>,
	      public std::stack<lexer_filter_group_name>
{
    streams::token_stream m_parsed_token_stream;
    lexer_filter_group& get_current_filter_group();

public:
    typedef std::list<lexer_filter_group> filter_group_list;
    typedef std::stack<lexer_filter_group_name> filter_group_name_stack;

    lexer(std::list<char>&);
    lexer(std::string&);
    lexer(filename&);

    void add(lexer_filter_group&);
    void set_filter_group(const lexer_filter_group_name&);

    ////////////////////////////////////////////////////////////////
    // abstract_token_stream_operation_common
    ////////////////////////////////////////////////////////////////
    virtual token peek_token() override;
    virtual token read_token() override;
    virtual void unread_token(const token&) override;
    virtual void unread_token(const token_list&) override;

    token peek_token(lexer_filter_group&);
    token read_token(lexer_filter_group&);
};

}}
#endif	/* __PARSER_LEXER_LEXER_HPP__ */
