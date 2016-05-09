#include "parser/filter/filter_composer_seq.hpp"

namespace parser_tools { namespace filters {

seq::seq(const std::list<filter>& filters) : std::list<filter>(filters)
{}

seq::result_type seq::filterate(streams::char_stream& char_stream_)
{
    filter::result_type single_result;
    filter::result_type result(filter_status::not_filtered, buffer_view<char>());
    
    bool complete = true;
    streams::char_stream::iterator begin_iterator(char_stream_.current_iterator());

    filter_list& list = *this;
    for (auto it = list.begin(); it != list.end(); ++it) {
	single_result = it->do_filterate(char_stream_);
	if (single_result.first == filter_status::filtered) {
	    if (result.first == filter_status::filtered) {
		result.second += single_result.second;
	    } else {
		result = single_result;
	    }
	} else {
	    complete = false;
	    break;
	}
    }
    if (!complete) {
	char_stream_.set_iterator(begin_iterator);
	return filter::result_type(filter_status::not_filtered, buffer_view<char>());
    }
    return result;
}

}}
