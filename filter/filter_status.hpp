#ifndef __PARSER_FILTER_FILTER_STATUS_HPP__
#define __PARSER_FILTER_FILTER_STATUS_HPP__

#include <iostream>

namespace parser_tools { namespace filters {

enum class filter_status {
    none,
    eof,
	filtered,
	not_filtered,
};

std::ostream& operator<<(std::ostream& os, filter_status status);

}}
#endif	/* __PARSER_FILTER_FILTER_STATUS_HPP__ */
