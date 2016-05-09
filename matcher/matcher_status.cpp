#include <iostream>
#include "parser/matcher/matcher_status.hpp"

namespace parser_tools { namespace matchers {

std::ostream& operator<<(std::ostream& os, matcher_status status)
{
    switch (status) {
    case matcher_status::not_match:
	os << "matcher_status::not_match";
	break;
    case matcher_status::partial_match:
	os << "matcher_status::partial_match";
	break;
    case matcher_status::full_match:
	os << "matcher_status::full_match";
	break;
    case matcher_status::over_match:
	os << "matcher_status::over_match";
	break;
    }
    return os;
}

}}

