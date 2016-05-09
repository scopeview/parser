#ifndef __PARSER_REDUCER_TOKEN_TYPE_REDUCED_FROM_HPP__
#define __PARSER_REDUCER_TOKEN_TYPE_REDUCED_FROM_HPP__

#include "parser/token/token_type.hpp"

namespace parser_tools { namespace reducers {

class token_type_reduced_from : public token_type {
public:
    token_type_reduced_from(const token_type&);
};

}}
#endif	/* __PARSER_REDUCER_TOKEN_TYPE_REDUCED_FROM_HPP__ */
