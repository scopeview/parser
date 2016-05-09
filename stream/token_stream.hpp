#ifndef __PARSER_STREAM_TOKEN_STREAM_HPP__
#define __PARSER_STREAM_TOKEN_STREAM_HPP__

#include <list>
#include "parser/token/token.hpp"
#include "parser/stream/abstract_token_stream_operation.hpp"

namespace parser_tools { namespace streams {

class token_stream : public abstract_token_stream_operation_common,
		     public abstract_token_stream_operation_length,
		     public abstract_token_stream_operation_tail,
		     public std::list<token>
{
public:
    typedef std::list<token> container_type;

    token_stream();
    token_stream(const std::list<token>&);

    ////////////////////////////////////////////////////////////////
    // abstract_token_stream_operation_common
    ////////////////////////////////////////////////////////////////
    virtual token peek_token() override;
    virtual token read_token() override;
    virtual void unread_token(const token&) override;
    virtual void unread_token(const token_list&) override;

    ////////////////////////////////////////////////////////////////
    // abstract_token_stream_operation_length
    ////////////////////////////////////////////////////////////////
    virtual int size() override;

    ////////////////////////////////////////////////////////////////
    // abstract_token_stream_operation_tail
    ////////////////////////////////////////////////////////////////
    virtual int add_tail_token(const token&) override;
    virtual token read_tail_token() override;

    token_list read_token_all();
    token_list peek_token_all();
};

}}
#endif	/* __PARSER_STREAM_TOKEN_STREAM_HPP__ */
