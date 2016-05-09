#include <string>
#include <iostream>
#include "gtest/gtest.h"
#include "parser/token/_.hpp"

namespace {

using namespace std;
using namespace parser_tools;
using namespace parser_tools::basic;

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

TEST(test_parser, token)
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
    token_list expected_token_list({t1, t2, t3, t4, t5});

    EXPECT_EQ(token_list({t3}), t_234_3.to_orignal_token_list());
    EXPECT_EQ(expected_token_list, t_all.to_orignal_token_list());
}

}
