#ifndef __PARSER_REDUCER_TOKEN_TYPE_REDUCED_HPP__
#define __PARSER_REDUCER_TOKEN_TYPE_REDUCED_HPP__

#include "parser/token/token_type.hpp"

namespace parser_tools { namespace reducers {

class token_type_reduced : public token_type {
public:
    token_type_reduced(const token_type&);
};

}}
#endif	/* __PARSER_REDUCER_TOKEN_TYPE_REDUCED_HPP__ */
