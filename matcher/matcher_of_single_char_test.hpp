#ifndef __PARSER_MATCHER_MATCHER_OF_SINGLE_CHAR_TEST_HPP__
#define __PARSER_MATCHER_MATCHER_OF_SINGLE_CHAR_TEST_HPP__

#include <functional>
#include "parser/matcher/abstract_matcher_operation.hpp"
#include "parser/matcher/matcher_status.hpp"

namespace parser_tools { namespace matchers {

class single_char_test_matcher : public abstract_matcher_operation
{
public:
    typedef std::function<bool(char)> test_function_type;

private:
    test_function_type m_test_function;

public:
    single_char_test_matcher(const test_function_type&);
    virtual matcher_status match(const std::list<char>&) override;
    virtual matcher_status match(const std::vector<char>::iterator&, const std::vector<char>::iterator&) override;

    template<typename IT>
    matcher_status __match(const IT& begin, const IT& end) {
	matcher_status result_status = matcher_status::not_match;
	char input_first = *begin;
	int input_size = std::distance(begin, end);
	assert(input_size >= 1);

	if (m_test_function(input_first)) {
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
#endif	/* __PARSER_MATCHER_MATCHER_OF_SINGLE_CHAR_TEST_HPP__ */
