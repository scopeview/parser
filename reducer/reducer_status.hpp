#ifndef __PARSER_REDUCER_REDUCER_STATUS_HPP__
#define __PARSER_REDUCER_REDUCER_STATUS_HPP__

#include <iostream>

namespace parser_tools { namespace reducers {

enum class reducer_status {
    not_reduced,
	partial_reduced,
	full_reduced,
};

std::ostream& operator<<(std::ostream& os, reducer_status status);

}}
#endif	/* __PARSER_REDUCER_REDUCER_STATUS_HPP__ */
