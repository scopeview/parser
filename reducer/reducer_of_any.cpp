#include "parser/reducer/reducer_of_any.hpp"

namespace parser_tools { namespace reducers {

any::any(const token_type_reduced& token_type_reduced_, 
	 const token_type_list_reduced_from& token_type_list_reduced_from_)
    : token_type_reduced(token_type_reduced_), token_type_list_reduced_from(token_type_list_reduced_from_)
{}

////////////////////////////////////////////////////////////////
// abstract_reducer
////////////////////////////////////////////////////////////////
reducer_result any::reduce(streams::abstract_token_stream_operation_common& stream_)
{
    token t(stream_.read_token());
    token token_reduced;
    reducer_status result_status = reducer_status::not_reduced;
    if (!t.is_nil()) {
	token_type_list_reduced_from& from_list = *this;
	auto it = std::find(from_list.begin(), from_list.end(), static_cast<token_type&>(t));
	if (it != from_list.end()) {
	    token_type_reduced& to = *this;
	    token_reduced = token(static_cast<token_type&>(to), token_list({t}));
	    result_status = reducer_status::full_reduced;
	} else {
	    stream_.unread_token(t);
	}
    }
    return reducer_result(result_status, token_reduced);
}

}}
