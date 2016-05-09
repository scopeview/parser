#include "parser/filter/filter_composer_many.hpp"

namespace parser_tools { namespace filters {

many_common::many_common(const filter& filter_, bool is_match_empty)
    : m_filter(filter_), m_is_match_empty(is_match_empty)
{}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
filter::result_type many_common::filterate(streams::char_stream& char_stream_)
{
    buffer_view<char> view;
    filter::result_type single_result;
    filter::result_type result(filter_status::not_filtered, buffer_view<char>());
    if (m_is_match_empty) {
	result = filter::result_type(filter_status::filtered, buffer_view<char>());
    }

    while (1) {
	single_result = m_filter.do_filterate(char_stream_);
	if (single_result.first == filter_status::filtered) {
	    if (result.first == filter_status::filtered) {
		result.second += single_result.second;
	    } else {
		result = single_result;
	    }
	} else {
	    break;
	}
    }
    return result;
}

////////////////////////////////////////////////////////////////
// many/many*
////////////////////////////////////////////////////////////////
many::many(const filter& filter_) : many_common(filter_, false)
{}

many_or_ellipsis::many_or_ellipsis(const filter& filter_) : many_common(filter_, true)
{}


}}
