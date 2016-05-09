#include <cassert>
#include "parser/matcher/matcher_of_list.hpp"

namespace parser_tools { namespace matchers {

list_matcher::list_matcher(const std::list<char>& pattern) : m_pattern(pattern)
{
    assert(!pattern.empty());
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
matcher_status list_matcher::match(const std::list<char>& input)
{
    assert(!input.empty());
    return match(input.begin(), input.end());
}

matcher_status list_matcher::match(const std::vector<char>::iterator& begin, const std::vector<char>::iterator& end)
{
    assert(begin != end);
    return __match(begin, end);
}

}}
