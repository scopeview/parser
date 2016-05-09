#include <cassert>
#include "parser/matcher/matcher_of_single_char_test.hpp"

namespace parser_tools { namespace matchers {

single_char_test_matcher::single_char_test_matcher(const test_function_type& test_function)
    : m_test_function(test_function)
{}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
matcher_status single_char_test_matcher::match(const std::list<char>& input)
{
    assert(!input.empty());
    return match(input.begin(), input.end());
}

matcher_status single_char_test_matcher::match(const std::vector<char>::iterator& begin, const std::vector<char>::iterator& end)
{
    assert(begin != end);
    return __match(begin, end);
}

}}
