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

TEST(test_stream, token_stream)
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

    // peek_token
    // read_token
    // unread_token
    // size
    {
	token t;
	token_stream token_stream_({t1, t2, t3});
	EXPECT_EQ(3, token_stream_.size());
	EXPECT_EQ(t1, token_stream_.peek_token());
	t = token_stream_.read_token();
	EXPECT_EQ(t1, t);
	token_stream_.unread_token(t);
	EXPECT_EQ(t1, token_stream_.peek_token());
    }
    // unread_token(list)
    {
	token_stream token_stream_;
	token_list tlist({t1, t2, t3});
	token_stream_.unread_token(tlist);
	EXPECT_EQ(t1, token_stream_.read_token());
	EXPECT_EQ(t2, token_stream_.read_token());
	EXPECT_EQ(t3, token_stream_.read_token());
    }
    // read_token_all
    // peek_token_all
    {
	token_stream token_stream_({t1, t2, t3});
	token_list expect_tlist({t1, t2, t3});
	EXPECT_EQ(expect_tlist, token_stream_.peek_token_all());
	EXPECT_EQ(t1, token_stream_.peek_token());
	EXPECT_EQ(expect_tlist, token_stream_.read_token_all());	
	EXPECT_EQ(token(), token_stream_.read_token());	
    }
    // add_tail_token
    // read_tail_token
    {
	token_stream token_stream_;
	token_list expect_tlist({t1, t2, t3});
	token_list expect_tlist_2({t1, t2});
	token_stream_.add_tail_token(t1);
	token_stream_.add_tail_token(t2);
	token_stream_.add_tail_token(t3);
	EXPECT_EQ(expect_tlist, token_stream_.peek_token_all());
	EXPECT_EQ(t3, token_stream_.read_tail_token());
	EXPECT_EQ(expect_tlist_2, token_stream_.peek_token_all());	
    }
}

}
