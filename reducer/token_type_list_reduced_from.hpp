#ifndef __PARSER_REDUCER_TOKEN_TYPE_LIST_REDUCED_FROM_HPP__
#define __PARSER_REDUCER_TOKEN_TYPE_LIST_REDUCED_FROM_HPP__

#include <list>
#include "parser/token/token_type.hpp"

namespace parser_tools { namespace reducers {

class token_type_list_reduced_from : public std::list<token_type>
{
public:
    token_type_list_reduced_from(const std::list<token_type>&);
};

}}
#endif	/* __PARSER_REDUCER_TOKEN_TYPE_LIST_REDUCED_FROM_HPP__ */
