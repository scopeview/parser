#ifndef __PARSER_MATCHER_MATCHER_OF_CHAR_HPP__
#define __PARSER_MATCHER_MATCHER_OF_CHAR_HPP__

#include "parser/matcher/abstract_matcher_operation.hpp"
#include "parser/matcher/matcher_status.hpp"

namespace parser_tools { namespace matchers {

class char_matcher : public abstract_matcher_operation
{
    char m_pattern;

public:
    char_matcher(char);
    virtual matcher_status match(const std::list<char>&) override;
    virtual matcher_status match(const std::vector<char>::iterator&, const std::vector<char>::iterator&) override;

    template<typename IT>
    matcher_status __match(const IT& begin, const IT& end) {
	int input_size = std::distance(begin, end);
	char input_first = *begin;
	matcher_status result_status = matcher_status::not_match;
    
	if (m_pattern == input_first) {
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
#endif	/* __PARSER_MATCHER_MATCHER_OF_CHAR_HPP__ */
