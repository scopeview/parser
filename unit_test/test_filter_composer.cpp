#include <string>
#include <iostream>
#include "gtest/gtest.h"
#include "parser/matcher/_.hpp"
#include "parser/filter/_.hpp"

namespace {

using namespace std;
using namespace parser_tools::matchers;
using namespace parser_tools::filters;
using namespace parser_tools::streams;

TEST(test_parser_filter, composer_many)
{
    std::string input("aabbcc");
    char_matcher matcher('a');
    char_stream char_stream_(input);
    filter filter_char(matcher);
    many_common many_common_(filter_char, false);

    filter::result_type result = many_common_.filterate(char_stream_);
    EXPECT_EQ(filter_status::filtered, result.first);
    EXPECT_EQ(std::list<char>({'a', 'a'}), result.second.to_list());
    EXPECT_EQ('b', *char_stream_.current_iterator());
}

TEST(test_parser_filter, composer_many_for_each_single_char_test)
{
    std::string input("ab c");
    char_stream char_stream_(input);

    for_each_single_char_test_matcher::test_function_type f = [] (char input) {
	return std::isalpha(input);
    };
    for_each_single_char_test_matcher matcher(f);
    filter filter_(matcher);
    many_common many_common_(filter_, false);

    filter::result_type result = many_common_.filterate(char_stream_);
    EXPECT_EQ(filter_status::filtered, result.first);
    EXPECT_EQ(std::list<char>({'a', 'b'}), result.second.to_list());
    EXPECT_EQ(' ', *char_stream_.current_iterator());
}

TEST(test_parser_filter, composer_any)
{
    std::string input("abc");
    char_matcher matcher_1('b');
    char_matcher matcher_2('a');
    char_stream char_stream_(input);
    filter filter_1(matcher_1);
    filter filter_2(matcher_2);
    any_common any_common_({filter_1, filter_2}, false);

    filter::result_type result = any_common_.filterate(char_stream_);
    EXPECT_EQ(filter_status::filtered, result.first);
    EXPECT_EQ(std::list<char>({'a'}), result.second.to_list());
    EXPECT_EQ('b', *char_stream_.current_iterator());
}

TEST(test_parser_filter, composer_seq)
{
    std::string input("abc");
    char_matcher matcher_1('a');
    char_matcher matcher_2('b');
    char_stream char_stream_(input);
    filter filter_1(matcher_1);
    filter filter_2(matcher_2);
    seq seq_({filter_1, filter_2});

    filter::result_type result = seq_.filterate(char_stream_);
    EXPECT_EQ(filter_status::filtered, result.first);
    EXPECT_EQ(std::list<char>({'a', 'b'}), result.second.to_list());
    EXPECT_EQ('c', *char_stream_.current_iterator());
}

////////////////////////////////////////////////////////////////
TEST(test_parser_filter, composer_many_filter_any_char_matcher)
{
    std::string input(" \t\na");
    char_stream char_stream_(input);

    auto filter_ = many(filter(any_char_matcher({' ', '\t', '\n'})));
    
    filter::result_type result = filter_.filterate(char_stream_);
    EXPECT_EQ(filter_status::filtered, result.first);
    EXPECT_EQ(std::list<char>({' ', '\t', '\n'}), result.second.to_list());
    EXPECT_EQ('a', *char_stream_.current_iterator());
}

}
