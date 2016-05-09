#include <list>
#include <string>
#include <iostream>
#include "gtest/gtest.h"
#include "parser/stream/_.hpp"
#include "parser/reducer/_.hpp"
#include "parser/parser/_.hpp"
// #include "estd/debug/_.hpp"

using namespace std;
using namespace parser_tools;
using namespace parser_tools::basic;
using namespace parser_tools::reducers;
using namespace parser_tools::streams;
using namespace parser_tools::parsers;
// using parser::token;
// using parser::token_type;
// using parser::token_buf;
// using parser::token_list;

namespace {


enum {
    VAR,
    VAL,
    TOK_STRING,
    TOK_NUMBER,
};

TEST(test_parser_reducer, reducer_of_single)
{
    buffer<char> b(std::string("hello"));
    buffer_view<char> bview(b, b.begin(), b.end());
    token_type token_type_(TOK_STRING);
    token_buf token_buf_(bview);
    token t_1(token_type_, token_buf_);
    std::list<token> tlist;
    tlist.push_back(t_1);
    token_stream token_stream_(tlist);
    token_type token_type_1(VAL);
    token_type token_type_2(TOK_STRING);
    token_type_reduced token_type_reduced_(token_type_1);
    token_type_reduced_from token_type_reduced_from_(token_type_2);
    single single_(token_type_reduced_, token_type_reduced_from_);
    parser_stream parser_stream_(token_stream_);

    reducer_result result = single_.reduce(parser_stream_);
    EXPECT_EQ(reducer_status::full_reduced, result.get_reducer_status());
    EXPECT_EQ(token_type(VAL), static_cast<token_type&>(result.get_token()));
    EXPECT_EQ(token_list({t_1}), static_cast<token_list&>(result.get_token()));
}

}
