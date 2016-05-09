#ifndef __PARSER_REDUCER_ABSTRACT_REDUCER_HPP__
#define __PARSER_REDUCER_ABSTRACT_REDUCER_HPP__

#include "parser/reducer/reducer_result.hpp"
#include "parser/stream/abstract_token_stream_operation.hpp"

namespace parser_tools { namespace reducers {

struct abstract_reducer {
    virtual reducer_result reduce(streams::abstract_token_stream_operation_common&) = 0;
};

}}
#endif	/* __PARSER_REDUCER_ABSTRACT_REDUCER_HPP__ */
