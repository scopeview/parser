#include <string>
#include <iostream>
#include "gtest/gtest.h"
#include "parser/matcher/_.hpp"

namespace {

using namespace std;
using namespace parser_tools::matchers;

TEST(test_parser_matcher, matcher_of_string)
{
    std::list<char> input_1 = {'a'};
    std::list<char> input_2 = {'b'};
    std::list<char> input_3 = {'c'};
    std::list<char> input_4 = {'a', 'b'};
    std::list<char> input_5 = {'a', 'b', 'c'};
    std::string pattern("ab");

    string_matcher matcher(pattern);
    EXPECT_EQ(matcher_status::partial_match, matcher.match(input_1));
    EXPECT_EQ(matcher_status::not_match, matcher.match(input_2));
    EXPECT_EQ(matcher_status::not_match, matcher.match(input_3));
    EXPECT_EQ(matcher_status::full_match, matcher.match(input_4));
    EXPECT_EQ(matcher_status::over_match, matcher.match(input_5));
}

}
