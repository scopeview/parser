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

std::list<char> input_empty;
std::list<char> input_a = {'a'};
std::list<char> input_b = {'b'};
std::list<char> input_c = {'c'};
std::list<char> input_d = {'d'};
std::list<char> input_ab = {'a', 'b'};
std::list<char> input_bc = {'b', 'c'};
std::list<char> input_abc = {'a', 'b', 'c'};
std::list<char> input_abcd = {'a', 'b', 'c', 'd'};
std::list<char> pattern_a = {'a'};
std::list<char> pattern_b = {'b'};
std::list<char> pattern_ab = {'a', 'b'};
std::list<char> pattern_bc = {'b', 'c'};
std::list<char> pattern_abc = {'a', 'b', 'c'};
std::string pattern_string_ab("ab");

char_stream char_stream_(input_a);

#define DEFINE_MATCHER_STREAM_FILTER_RESULT() ;char_stream char_stream_a(input_a); \
    char_stream char_stream_b(input_b);		\
    char_stream char_stream_c(input_c);		\
    char_stream char_stream_d(input_d);		\
    char_stream char_stream_ab(input_ab);	\
    char_stream char_stream_bc(input_bc);	\
    char_stream char_stream_abc(input_abc);	\
    char_stream char_stream_abcd(input_abcd);	\
    filter::result_type result			\
/***/

TEST(test_parser_filter, matcher_of_any_char)
{
    any_char_matcher matcher(pattern_abc);
    filter filter_(matcher)
    DEFINE_MATCHER_STREAM_FILTER_RESULT();

    result = filter_.filterate(char_stream_a);
    EXPECT_EQ(filter_status::filtered, result.first);
    EXPECT_EQ(input_a, result.second.to_list());

    result = filter_.filterate(char_stream_b);
    EXPECT_EQ(filter_status::filtered, result.first);
    EXPECT_EQ(input_b, result.second.to_list());

    result = filter_.filterate(char_stream_c);
    EXPECT_EQ(filter_status::filtered, result.first);
    EXPECT_EQ(input_c, result.second.to_list());

    result = filter_.filterate(char_stream_d);
    EXPECT_EQ(filter_status::not_filtered, result.first);
    EXPECT_EQ(std::list<char>(), result.second.to_list());

    result = filter_.filterate(char_stream_ab);
    EXPECT_EQ(filter_status::filtered, result.first);
    EXPECT_EQ(input_a, result.second.to_list());
    EXPECT_EQ('b', *char_stream_ab.current_iterator());
}

TEST(test_parser_filter, matcher_of_char)
{
    char_matcher matcher('b');
    filter filter_(matcher);
    DEFINE_MATCHER_STREAM_FILTER_RESULT();

    result = filter_.filterate(char_stream_a);
    EXPECT_EQ(filter_status::not_filtered, result.first);
    EXPECT_EQ(input_empty, result.second.to_list());
    
    result = filter_.filterate(char_stream_b);
    EXPECT_EQ(filter_status::filtered, result.first);
    EXPECT_EQ(input_b, result.second.to_list());
    
    result = filter_.filterate(char_stream_c);
    EXPECT_EQ(filter_status::not_filtered, result.first);
    EXPECT_EQ(input_empty, result.second.to_list());
    
    result = filter_.filterate(char_stream_ab);
    EXPECT_EQ(filter_status::not_filtered, result.first);
    EXPECT_EQ(input_empty, result.second.to_list());
    
    result = filter_.filterate(char_stream_bc);
    EXPECT_EQ(filter_status::filtered, result.first);
    EXPECT_EQ(input_b, result.second.to_list());
    EXPECT_EQ('c', *char_stream_bc.current_iterator());    
}

TEST(test_parser_filter, matcher_of_for_each_single_char_test)
{
    for_each_single_char_test_matcher::test_function_type f = [] (char input) {
	switch (input) {
	case 'a':
	case 'b':
	return true;
	break;
	}
	return false;
    };
    for_each_single_char_test_matcher matcher(f);
    filter filter_(matcher);
    DEFINE_MATCHER_STREAM_FILTER_RESULT();

    result = filter_.filterate(char_stream_a);
    EXPECT_EQ(filter_status::filtered, result.first);
    EXPECT_EQ(input_a, result.second.to_list());
    
    result = filter_.filterate(char_stream_b);
    EXPECT_EQ(filter_status::filtered, result.first);
    EXPECT_EQ(input_b, result.second.to_list());
    
    result = filter_.filterate(char_stream_c);
    EXPECT_EQ(filter_status::not_filtered, result.first);
    EXPECT_EQ(input_empty, result.second.to_list());
    
    result = filter_.filterate(char_stream_ab);
    EXPECT_EQ(filter_status::filtered, result.first);
    EXPECT_EQ(input_a, result.second.to_list());
    EXPECT_EQ('b', *char_stream_bc.current_iterator());
    
    result = filter_.filterate(char_stream_bc);
    EXPECT_EQ(filter_status::filtered, result.first);
    EXPECT_EQ(input_b, result.second.to_list());
    EXPECT_EQ('c', *char_stream_bc.current_iterator());
}

TEST(test_parser_filter, matcher_of_list)
{
    list_matcher matcher(pattern_ab);
    filter filter_(matcher);
    DEFINE_MATCHER_STREAM_FILTER_RESULT();

    result = filter_.filterate(char_stream_a);
    EXPECT_EQ(filter_status::not_filtered, result.first);
    EXPECT_EQ(input_empty, result.second.to_list());
    
    result = filter_.filterate(char_stream_b);
    EXPECT_EQ(filter_status::not_filtered, result.first);
    EXPECT_EQ(input_empty, result.second.to_list());
    
    result = filter_.filterate(char_stream_ab);
    EXPECT_EQ(filter_status::filtered, result.first);
    EXPECT_EQ(input_ab, result.second.to_list());
    
    result = filter_.filterate(char_stream_abc);
    EXPECT_EQ(filter_status::filtered, result.first);
    EXPECT_EQ(input_ab, result.second.to_list());
    EXPECT_EQ('c', *char_stream_abc.current_iterator());
}

TEST(test_parser_filter, matcher_of_list_test)
{
    list_test_matcher::test_function_type f = [] (const std::list<char>& input) {
	matcher_status result_status = matcher_status::not_match;
	auto it = input.begin();
	for (int index = 0; it != input.end(); ++it, ++index) {
	    switch (index) {
	    case 0:
	    if (*it == 'a')
		result_status = matcher_status::partial_match;
	    else 
		break;
	    break;
	    case 1:
	    if (*it == 'b')
		result_status = matcher_status::full_match;
	    else
		break;
	    break;
	    case 2:
		result_status = matcher_status::over_match;
		break;
	    default:
		assert(0);
		// result_status = matcher_status::not_match;
		break;
	    }
	}
	return result_status;
    };
    list_test_matcher matcher(f);
    filter filter_(matcher);
    DEFINE_MATCHER_STREAM_FILTER_RESULT();

    result = filter_.filterate(char_stream_a);
    EXPECT_EQ(filter_status::not_filtered, result.first);
    EXPECT_EQ(input_empty, result.second.to_list());

    result = filter_.filterate(char_stream_b);
    EXPECT_EQ(filter_status::not_filtered, result.first);
    EXPECT_EQ(input_empty, result.second.to_list());

    result = filter_.filterate(char_stream_ab);
    EXPECT_EQ(filter_status::filtered, result.first);
    EXPECT_EQ(input_ab, result.second.to_list());

    result = filter_.filterate(char_stream_bc);
    EXPECT_EQ(filter_status::not_filtered, result.first);
    EXPECT_EQ(input_empty, result.second.to_list());

    result = filter_.filterate(char_stream_abc);
    EXPECT_EQ(filter_status::filtered, result.first);
    EXPECT_EQ(input_ab, result.second.to_list());
    EXPECT_EQ('c', *char_stream_abc.current_iterator());
}

TEST(test_parser_filter, matcher_of_single_char_test)
{
    single_char_test_matcher::test_function_type f = [] (char input) {
	return input == 'a';
    };
    single_char_test_matcher matcher(f);
    filter filter_(matcher);
    DEFINE_MATCHER_STREAM_FILTER_RESULT();

    result = filter_.filterate(char_stream_a);
    EXPECT_EQ(filter_status::filtered, result.first);
    EXPECT_EQ(input_a, result.second.to_list());

    result = filter_.filterate(char_stream_b);
    EXPECT_EQ(filter_status::not_filtered, result.first);
    EXPECT_EQ(input_empty, result.second.to_list());
    EXPECT_EQ('b', *char_stream_b.current_iterator());

    result = filter_.filterate(char_stream_ab);
    EXPECT_EQ(filter_status::filtered, result.first);
    EXPECT_EQ(input_a, result.second.to_list());
    EXPECT_EQ('b', *char_stream_b.current_iterator());
}

TEST(test_parser_filter, matcher_of_string)
{
    string_matcher matcher(pattern_string_ab);
    filter filter_(matcher);
    DEFINE_MATCHER_STREAM_FILTER_RESULT();

    result = filter_.filterate(char_stream_a);
    EXPECT_EQ(filter_status::not_filtered, result.first);
    EXPECT_EQ(input_empty, result.second.to_list());
    EXPECT_EQ('a', *char_stream_a.current_iterator());

    result = filter_.filterate(char_stream_b);
    EXPECT_EQ(filter_status::not_filtered, result.first);
    EXPECT_EQ(input_empty, result.second.to_list());
    EXPECT_EQ('b', *char_stream_b.current_iterator());

    result = filter_.filterate(char_stream_ab);
    EXPECT_EQ(filter_status::filtered, result.first);
    EXPECT_EQ(input_ab, result.second.to_list());

    result = filter_.filterate(char_stream_abc);
    EXPECT_EQ(filter_status::filtered, result.first);
    EXPECT_EQ(input_ab, result.second.to_list());
    EXPECT_EQ('c', *char_stream_abc.current_iterator());
}


}
