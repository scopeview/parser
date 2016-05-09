#include <cassert>
#include <algorithm>
#include "parser/matcher/matcher_of_any_char.hpp"
#include "parser/basic/macro.hpp"

namespace parser_tools { namespace matchers {

any_char_matcher::any_char_matcher(const std::list<char>& pattern) : m_pattern(pattern)
{
    assert(!pattern.empty());
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
matcher_status any_char_matcher::match(const std::list<char>& input)
{
    assert(!input.empty());
    return match(input.begin(), input.end());
}

matcher_status any_char_matcher::match(const std::vector<char>::iterator& begin, const std::vector<char>::iterator& end)
{
    assert(begin != end);
    return __match(begin, end);
}

}}
