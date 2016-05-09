#ifndef __PARSER_STREAM_ABSTRACT_TOKEN_STREAM_OPERATION_HPP__
#define __PARSER_STREAM_ABSTRACT_TOKEN_STREAM_OPERATION_HPP__

#include <memory>
#include "parser/token/token.hpp"

namespace parser_tools { namespace streams {

struct abstract_token_stream_operation_common {
    virtual token peek_token() = 0;
    virtual token read_token() = 0;
    virtual void unread_token(const token&) = 0;
    virtual void unread_token(const token_list&) = 0;
};

struct abstract_token_stream_operation_tail {
    virtual int add_tail_token(const token&) = 0;
    virtual token read_tail_token() = 0;
};

struct abstract_token_stream_operation_length {
    virtual int size() = 0;
};

}}
#endif	/* __PARSER_STREAM_ABSTRACT_TOKEN_STREAM_OPERATION_HPP__ */
