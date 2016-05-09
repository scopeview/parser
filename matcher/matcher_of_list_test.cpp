#include <cassert>
#include "parser/matcher/matcher_of_list_test.hpp"

namespace parser_tools { namespace matchers {

list_test_matcher::list_test_matcher(const test_function_type& test_function)
    : m_test_function(test_function)
{}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
matcher_status list_test_matcher::match(const std::list<char>& input)
{
    assert(!input.empty());
    return m_test_function(input);
}

matcher_status list_test_matcher::match(const std::vector<char>::iterator& begin, const std::vector<char>::iterator& end)
{
    assert(begin != end);
    return __match(begin, end);
}

}}
