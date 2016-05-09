#include "parser/reducer/reducer_status.hpp"

namespace parser_tools { namespace reducers {

std::ostream& operator<<(std::ostream& os, reducer_status status)
{
    switch (status) {
    case reducer_status::not_reduced:
	os << "reducer_status::not_reduced";
	break;
    case reducer_status::partial_reduced:
	os << "reducer_status::partial_reduced";
	break;
    case reducer_status::full_reduced:
	os << "reducer_status::full_reduced";
	break;
    }
    return os;
}

}}
