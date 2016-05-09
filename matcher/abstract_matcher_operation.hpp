#ifndef __PARSER_MATCHER_ABSTRACT_MATCHER_OPERATION_HPP__
#define __PARSER_MATCHER_ABSTRACT_MATCHER_OPERATION_HPP__

#include <list>
#include <vector>
#include "parser/matcher/matcher_status.hpp"

namespace parser_tools { namespace matchers {

struct abstract_matcher_operation {
    virtual matcher_status match(const std::list<char>&) = 0;
    virtual matcher_status match(const std::vector<char>::iterator&, const std::vector<char>::iterator&) = 0;
};

}}
#endif	/* __PARSER_MATCHER_ABSTRACT_MATCHER_OPERATION_HPP__ */
