#include <cassert>
#include "parser/matcher/matcher_of_char.hpp"

namespace parser_tools { namespace matchers {

char_matcher::char_matcher(char pattern) : m_pattern(pattern)
{}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
matcher_status char_matcher::match(const std::list<char>& input) 
{
    assert(!input.empty());
    return match(input.begin(), input.end());
}

matcher_status char_matcher::match(const std::vector<char>::iterator& begin, const std::vector<char>::iterator& end)
{
    assert(begin != end);
    return __match(begin, end);
}

}}

