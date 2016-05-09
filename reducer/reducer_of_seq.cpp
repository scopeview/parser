#include "parser/reducer/reducer_of_seq.hpp"

namespace parser_tools { namespace reducers {
seq::seq(const token_type_reduced& token_type_reduced_,
	 const token_type_list_reduced_from& token_type_list_reduced_from_)
    : token_type_reduced(token_type_reduced_), token_type_list_reduced_from(token_type_list_reduced_from_)
{}

////////////////////////////////////////////////////////////////
// abstract_reducer
////////////////////////////////////////////////////////////////
reducer_result seq::reduce(streams::abstract_token_stream_operation_common& stream_)
{
    token_type_reduced& to = *this;
    token_type_list_reduced_from& from_list = *this;
    token_list list;
    token t;
    bool complete = true;
    for (auto it = from_list.begin(); it != from_list.end(); ++it) {
	token_type token_type_expected(static_cast<token_type&>(*it));
	t = stream_.read_token();
	if (!t.is_nil()) {
	    if (token_type_expected == static_cast<token_type&>(t)) {
		list.push_back(t);
	    } else {
		stream_.unread_token(t);
		complete = false;
	    }
	} else {
	    complete = false;
	}
    }
    if (complete) {
	return reducer_result(reducer_status::full_reduced, token(static_cast<token_type&>(to), list));
    } else if (list.size()) {
	return reducer_result(reducer_status::partial_reduced, list);
    } else {
	return reducer_result(reducer_status::not_reduced, token());
    }
}

}}
