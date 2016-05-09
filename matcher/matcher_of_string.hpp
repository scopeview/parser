#ifndef __PARSER_MATCHER_MATCHER_OF_STRING_HPP__
#define __PARSER_MATCHER_MATCHER_OF_STRING_HPP__

#include "parser/matcher/abstract_matcher_operation.hpp"
#include "parser/matcher/matcher_status.hpp"

namespace parser_tools { namespace matchers {

class string_matcher : public abstract_matcher_operation
{
    std::string m_pattern;

public:
    string_matcher(const std::string&);
    virtual matcher_status match(const std::list<char>&) override;
    virtual matcher_status match(const std::vector<char>::iterator&, const std::vector<char>::iterator&) override;

    template<typename IT>
    matcher_status __match(const IT& begin, const IT& end) {
	int input_size = std::distance(begin, end);
	assert(input_size >= 1);
	int pattern_size = m_pattern.size();
	matcher_status result_status = matcher_status::full_match;
    
	auto it_pattern = m_pattern.begin();
	auto it_input = begin;
	for (; it_pattern != m_pattern.end() && it_input != end; ++it_pattern, ++it_input) {
	    if (*it_pattern != *it_input) {
		result_status = matcher_status::not_match;
	    }
	}
	if (matcher_status::full_match == result_status) {
	    if (pattern_size > input_size) {
		result_status = matcher_status::partial_match;
	    } else if (pattern_size < input_size) {
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
#endif	/* __PARSER_MATCHER_MATCHER_OF_STRING_HPP__ */
