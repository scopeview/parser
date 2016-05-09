#include <string>
#include <iostream>
#include "gtest/gtest.h"
#include "parser/token/_.hpp"
#include "parser/stream/_.hpp"
#include "parser/parser/parser_stream.hpp"

namespace {

using namespace std;
using namespace parser_tools;
using namespace parser_tools::streams;
using namespace parser_tools::parsers;

enum test_token_type {
    TOK_1,
    TOK_2,
    TOK_3,
    TOK_4,
    TOK_5,
    TOK_ALL,
    TOK_234,
    TOK_234_3,
};

TEST(test_parser_parser, parser_stream)
{
    buffer<char> b("hello");
    auto it = b.begin();
    token t1(token_type(TOK_1), buffer_view<char>(b, it, ++it));
    token t2(token_type(TOK_2), buffer_view<char>(b, it, ++it));
    token t3(token_type(TOK_3), buffer_view<char>(b, it, ++it));
    token t4(token_type(TOK_4), buffer_view<char>(b, it, ++it));
    token t5(token_type(TOK_5), buffer_view<char>(b, it, ++it));
    token t_234_3(token_type(TOK_234_3), token_list({t3}));
    token t_234(token_type(TOK_234), token_list({t2, t_234_3, t4}));
    token t_all(token_type(TOK_ALL), token_list({t1, t_234, t5}));    

    {
	token_stream token_stream_(token_list({t1, t_234, t5}));
	parser_stream parser_stream_(token_stream_);

	EXPECT_EQ(t1, parser_stream_.read_token());
	EXPECT_EQ(t_234, parser_stream_.read_token());
	parser_stream_.unread_token(t_234);
	EXPECT_EQ(t_234, parser_stream_.read_token());
	EXPECT_EQ(t5, parser_stream_.read_token());
	EXPECT_EQ(token(), parser_stream_.read_token());
	EXPECT_EQ(0, token_stream_.size());
    }
    // undo
    {
	token_stream token_stream_(token_list({t1, t_234, t5}));
	parser_stream parser_stream_(token_stream_);
	parser_stream_.read_token();
	EXPECT_EQ(t_234, token_stream_.peek_token());
	parser_stream_.unread_token(t1);
	EXPECT_EQ(t_234, token_stream_.peek_token());
	parser_stream_.undo(parser_stream_undo_type::parsed_token_stream);
	EXPECT_EQ(t1, token_stream_.peek_token());
    }
    // undo
    {
	token_stream token_stream_(token_list({t1, t_234, t5}));
	parser_stream parser_stream_(token_stream_);
	parser_stream_.read_token();
	parser_stream_.read_token();
	parser_stream_.read_token();
	parser_stream_.undo(parser_stream_undo_type::all);
	EXPECT_EQ(t1, token_stream_.read_token());
	EXPECT_EQ(t_234, token_stream_.read_token());
	EXPECT_EQ(t5, token_stream_.read_token());
    }
}

}
