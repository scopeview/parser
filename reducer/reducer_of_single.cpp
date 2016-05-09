#include "parser/reducer/reducer_of_single.hpp"

namespace parser_tools { namespace reducers {

single::single(const token_type_reduced& token_type_reduced_, 
	       const token_type_reduced_from& token_type_reduced_from_)
    : token_type_reduced(token_type_reduced_), token_type_reduced_from(token_type_reduced_from_)
{}

////////////////////////////////////////////////////////////////
// abstract_reducer
////////////////////////////////////////////////////////////////
reducer_result single::reduce(streams::abstract_token_stream_operation_common& stream_)
{
    token_type_reduced& to = *this;
    token_type_reduced_from& from = *this;
    token t = stream_.read_token();
    if (!t.is_nil() && (static_cast<token_type&>(t) == static_cast<token_type&>(from))) {
	return reducer_result(reducer_status::full_reduced, token(to, token_list({t})));
    }
    if (!t.is_nil()) {
	stream_.unread_token(t);
    }
    return reducer_result(reducer_status::not_reduced, token());
}

}}
