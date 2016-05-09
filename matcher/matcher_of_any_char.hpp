#ifndef __PARSER_MATCHER_MATCHER_OF_ANY_CHAR_HPP__
#define __PARSER_MATCHER_MATCHER_OF_ANY_CHAR_HPP__

#include <algorithm>
#include "parser/matcher/abstract_matcher_operation.hpp"
#include "parser/matcher/matcher_status.hpp"

namespace parser_tools { namespace matchers {

class any_char_matcher : public abstract_matcher_operation
{
    std::list<char> m_pattern;

public:
    any_char_matcher(const std::list<char>&);
    virtual matcher_status match(const std::list<char>&) override;
    virtual matcher_status match(const std::vector<char>::iterator&, const std::vector<char>::iterator&) override;

    template<typename IT>
    matcher_status __match(const IT& begin, const IT& end) {
	int input_size = std::distance(begin, end);
	assert(input_size >= 1);
	char input_first = *begin;
	matcher_status result_status = matcher_status::not_match;
	auto it = std::find(m_pattern.begin(), m_pattern.end(), input_first);
	if (it != m_pattern.end()) {
	    if (1 == input_size) {
		result_status = matcher_status::full_match;
	    } else {
		result_status = matcher_status::over_match;
	    }
	}
	return result_status;
    }

    template<typename IT>
    matcher_status match(const IT& begin, const IT& end) {
	return __match(begin, end);
    }
};

}}
#endif	/* __PARSER_MATCHER_MATCHER_OF_ANY_CHAR_HPP__ */
