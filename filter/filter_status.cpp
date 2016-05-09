#include "parser/filter/filter_status.hpp"

namespace parser_tools { namespace filters {

std::ostream& operator<<(std::ostream& os, filter_status status)
{
    switch (status) {
    case filter_status::eof:
	os << "filter_status::eof";
	break;
    case filter_status::filtered:
	os << "filter_status::filtered";
	break;
    case filter_status::not_filtered:
	os << "filter_status::not_filtered";
	break;
    }
    return os;
}

}}
