#include <list>
#include <string>
#include <iostream>
#include "gtest/gtest.h"
#include "parser/stream/_.hpp"
#include "parser/reducer/_.hpp"
#include "parser/parser/_.hpp"
// #include "estd/debug/_.hpp"

namespace {

using namespace std;
using namespace parser_tools;
using namespace parser_tools::basic;
using namespace parser_tools::reducers;
using namespace parser_tools::streams;
using namespace parser_tools::parsers;

enum {
    VAR,
    VAL,
    TOK_STRING,
    TOK_NUMBER,
    TOK_NOTHING,
};

TEST(test_parser_reducer, reducer_of_any)
{
    {
	buffer<char> b(std::string("hello"));
	buffer_view<char> bview(b, b.begin(), b.end());
	token_type token_type_(TOK_STRING);
	token_buf token_buf_(bview);
	token t_string(token_type_, token_buf_);
	std::list<token> tlist;
	tlist.push_back(t_string);
	token_stream token_stream_(tlist);
	reducers::any any_(token_type_reduced(token_type(VAL)),
			   token_type_list_reduced_from({token_type(TOK_STRING), token_type(TOK_NUMBER)}));
	parser_stream parser_stream_(token_stream_);

	reducer_result result = any_.reduce(parser_stream_);
	EXPECT_EQ(reducer_status::full_reduced, result.get_reducer_status());
	EXPECT_EQ(token_type(VAL), static_cast<token_type&>(result.get_token()));
	EXPECT_EQ(tlist, static_cast<token_list&>(result.get_token()));

    }
    {
	buffer<char> b(std::string("hello"));
	buffer_view<char> bview(b, b.begin(), b.end());
	token_type token_type_(TOK_NOTHING);
	token_buf token_buf_(bview);
	token t_1(token_type_, token_buf_);
	std::list<token> tlist;
	tlist.push_back(t_1);
	token_stream token_stream_(tlist);
	reducers::any any_(token_type_reduced(token_type(VAL)),
			   token_type_list_reduced_from({token_type(TOK_STRING), token_type(TOK_NUMBER)}));
	parser_stream parser_stream_(token_stream_);

	reducer_result result = any_.reduce(parser_stream_);
	EXPECT_EQ(reducer_status::not_reduced, result.get_reducer_status());
	EXPECT_EQ(token_type(), static_cast<token_type&>(result.get_token()));
	EXPECT_EQ(token_list(), static_cast<token_list&>(result.get_token()));

	token t_read = parser_stream_.read_token();
	EXPECT_EQ(token_type(TOK_NOTHING), static_cast<token_type&>(t_read));
	EXPECT_EQ(t_1, t_read);
    }
}

}
