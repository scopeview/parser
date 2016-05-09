#ifndef __PARSER_MATCHER_MATCHER_OF_LIST_TEST_HPP__
#define __PARSER_MATCHER_MATCHER_OF_LIST_TEST_HPP__

#include <functional>
#include "parser/matcher/abstract_matcher_operation.hpp"
#include "parser/matcher/matcher_status.hpp"

namespace parser_tools { namespace matchers {

class list_test_matcher : public abstract_matcher_operation
{
public:
    typedef std::function<matcher_status(const std::list<char>&)> test_function_type;

private:
    test_function_type m_test_function;

public:
    list_test_matcher(const test_function_type&);
    virtual matcher_status match(const std::list<char>&) override;
    virtual matcher_status match(const std::vector<char>::iterator&, const std::vector<char>::iterator&) override;

    template<typename IT>
    matcher_status __match(const IT& begin, const IT& end) {
	std::list<char> list;
	std::copy(begin, end, std::back_inserter(list));
	return match(list);
    }

    template<typename IT>
    matcher_status match(const IT& begin, const IT& end) {
	return __match(begin, end);
    }
};

}}
#endif	/* __PARSER_MATCHER_MATCHER_OF_LIST_TEST_HPP__ */
