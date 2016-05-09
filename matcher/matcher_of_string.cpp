#include <cassert>
#include "parser/matcher/matcher_of_string.hpp"

namespace parser_tools { namespace matchers {

string_matcher::string_matcher(const std::string& pattern) : m_pattern(pattern)
{
    assert(!pattern.empty());
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
matcher_status string_matcher::match(const std::list<char>& input)
{
    assert(!input.empty());
    return match(input.begin(), input.end());
}

matcher_status string_matcher::match(const std::vector<char>::iterator& begin, const std::vector<char>::iterator& end)
{
    assert(begin != end);
    return __match(begin, end);
}

}}
