#include "parser/filter/filter_composer_any.hpp"

namespace parser_tools { namespace filters {

any_common::any_common(const filter_list& filters, bool is_match_empty)
    : std::list<filter>(filters), m_is_match_empty(is_match_empty)
{}

any_common::result_type any_common::filterate(streams::char_stream& char_stream_)
{
    filter::result_type single_result;
    filter::result_type result(filter_status::not_filtered, buffer_view<char>());
    if (m_is_match_empty) {
	result = filter::result_type(filter_status::filtered, buffer_view<char>());
    }

    filter_list& list = *this;
    for (auto it = list.begin(); it != list.end(); ++it) {
	single_result = it->filterate(char_stream_);
	if (single_result.first == filter_status::filtered
	    || single_result.first == filter_status::eof) {
	    result = single_result;
	    break;
	}
    }
    return result;
}

////////////////////////////////////////////////////////////////
// any/any*
////////////////////////////////////////////////////////////////
any::any(const std::list<filter>& filters) : any_common(filters, true)
{}

any_or_ellipsis::any_or_ellipsis(const std::list<filter>& filters) : any_common(filters, false)
{}

}}
