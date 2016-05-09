#include <string>
#include <iostream>
#include "gtest/gtest.h"
#include "parser/matcher/_.hpp"

namespace {

using namespace std;
using namespace parser_tools::matchers;

TEST(test_parser_matcher, matcher_of_single_char_test)
{
    std::list<char> input_1 = {'a'};
    std::list<char> input_2 = {'b'};
    std::list<char> input_3 = {'c'};
    std::list<char> input_4 = {'a', 'b'};
    std::list<char> input_5 = {'a', 'b', 'c'};

    single_char_test_matcher::test_function_type f = [] (char input) {
	return input == 'a';
    };

    single_char_test_matcher matcher(f);
    EXPECT_EQ(matcher_status::full_match, matcher.match(input_1));
    EXPECT_EQ(matcher_status::not_match, matcher.match(input_2));
    EXPECT_EQ(matcher_status::not_match, matcher.match(input_3));
    EXPECT_EQ(matcher_status::over_match, matcher.match(input_4));
    EXPECT_EQ(matcher_status::over_match, matcher.match(input_5));
}

}
