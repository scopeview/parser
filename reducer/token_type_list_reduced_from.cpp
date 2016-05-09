#include "parser/reducer/token_type_list_reduced_from.hpp"

namespace parser_tools { namespace reducers {

token_type_list_reduced_from::token_type_list_reduced_from(const std::list<token_type>& list)
    : std::list<token_type>(list)
{}

}}
