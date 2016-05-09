#ifndef __PARSER_MATCHER_MATCHER_STATUS_HPP__
#define __PARSER_MATCHER_MATCHER_STATUS_HPP__

#include <iostream>

namespace parser_tools { namespace matchers {

enum class matcher_status {
    not_match,
	partial_match,
	full_match,
	over_match,
};

std::ostream& operator<<(std::ostream& os, matcher_status status);

}}
#endif	/* __PARSER_MATCHER_MATCHER_STATUS_HPP__ */
