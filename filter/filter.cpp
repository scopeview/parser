#include "parser/filter/filter.hpp"

namespace parser_tools { namespace filters {

filter::filter()
{}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
filter::result_type filter::filterate(streams::char_stream& char_stream_)
{
    return do_filterate(char_stream_);
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
filter::result_type filter::do_filterate(streams::char_stream& char_stream_)
{
    streams::char_stream::iterator input_begin = char_stream_.current_iterator();
    streams::char_stream::iterator input_end = input_begin;
    streams::char_stream::iterator end = char_stream_.end();
    matchers::matcher_status match_status;
    filter_status filter_status_ = filter_status::not_filtered;
    if (input_begin == end) {
	return result_type(filter_status::eof, buffer_view<char>(char_stream_.get_buffer(), input_begin, input_end));
    }

    do {
	if (input_end == end) {
	    return result_type(filter_status::not_filtered, buffer_view<char>(char_stream_.get_buffer(), input_begin, input_begin));
	}

	++input_end;
	match_status = m_matcher_op->match(input_begin, input_end);
	if (match_status == matcher_status::partial_match) {
	    continue;
	}

	if (match_status == matcher_status::full_match) {
	    filter_status_ = filter_status::filtered;
	    char_stream_.set_iterator(input_end);
	    break;
	} else if (match_status == matcher_status::not_match) {
	    input_end = input_begin;
	    break;
	} else {
	    assert(0);
	}
    } while (1);
    return result_type(filter_status_, buffer_view<char>(char_stream_.get_buffer(), input_begin, input_end));
}

}}

