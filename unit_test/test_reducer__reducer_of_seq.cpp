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
    EXPR,
    VAR,
    TOK_ASSIGN,
    VAL,
};

TEST(test_parser_reducer, reducer_of_seq)
{
    {
	buffer<char> b(std::string("hello"));
	buffer_view<char> bview(b, b.begin(), b.end());
	token t_var(token_type(VAR), bview);
	token t_assign(token_type(TOK_ASSIGN), bview);
	token t_val(token_type(VAL), bview);
	std::list<token> tlist;
	tlist.push_back(t_var);
	tlist.push_back(t_assign);
	tlist.push_back(t_val);
	token_stream token_stream_(tlist);
	reducers::seq seq_(token_type_reduced(token_type(EXPR)),
			   token_type_list_reduced_from({token_type(VAR), token_type(TOK_ASSIGN), token_type(VAL)}));
	parser_stream parser_stream_(token_stream_);

	reducer_result result = seq_.reduce(parser_stream_);
	EXPECT_EQ(reducer_status::full_reduced, result.get_reducer_status());
	EXPECT_EQ(token_type(EXPR), static_cast<token_type&>(result.get_token()));
	EXPECT_EQ(tlist, static_cast<token_list&>(result.get_token()));
    }
}

}
