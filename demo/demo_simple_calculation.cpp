#include <string>
#include <cctype>
#include "parser/parser.hpp"
#include "parser/stream/_.hpp"
#include "parser/matcher/matcher_of_string.hpp"
#include "parser/matcher/matcher_of_any_char.hpp"
#include "parser/matcher/matcher_of_single_char_test.hpp"

using namespace parser_tools;
using namespace parser_tools::reducers;
using namespace parser_tools::parsers;
using namespace parser_tools::streams;

enum token_type_enum {
    t_tok_number,
    t_tok_add,
    t_tok_lp,
    t_tok_rp,
    t_number,
    t_p_expr,
    t_add_expr,
    t_expr
};

std::vector<std::string> token_type_enum_string_array({
    "t_tok_number",
    "t_tok_add",
    "t_tok_lp",
    "t_tok_rp",
    "t_number",
    "t_p_expr",
    "t_add_expr",
    "t_expr"
	});

void test_simple_calculation()
{
    buffer<char> b("1234+()");
    auto it_begin = b.begin();
    auto it_end = it_begin;
    token tok_number_1(token_type(t_tok_number), token_buf(buffer_view<char>(b, it_begin, ++it_end)));
    token tok_number_2(token_type(t_tok_number), token_buf(buffer_view<char>(b, ++it_begin, ++it_end)));
    token tok_number_3(token_type(t_tok_number), token_buf(buffer_view<char>(b, ++it_begin, ++it_end)));
    token tok_number_4(token_type(t_tok_number), token_buf(buffer_view<char>(b, ++it_begin, ++it_end)));
    token tok_add(token_type(t_tok_add), token_buf(buffer_view<char>(b, ++it_begin, ++it_end)));
    token tok_lp(token_type(t_tok_lp), token_buf(buffer_view<char>(b, ++it_begin, ++it_end)));
    token tok_rp(token_type(t_tok_rp), token_buf(buffer_view<char>(b, ++it_begin, ++it_end)));

    token_type::register_token_type_map(token_type_enum_string_array);

    // 1+(2+3)+4
    token_list tlist({tok_number_1, tok_add, tok_lp, tok_number_2, tok_add, tok_number_3, tok_rp, tok_add, tok_number_4});

    token_stream token_stream_(tlist);
    std::shared_ptr<parsers::parser_stream> parser_stream_(new parser_stream(token_stream_));

    // reducer
    token_type single_reducer_number__token_type_reduced__token_type(t_number);
    token_type_reduced single_reducer_number__token_type_reduced(single_reducer_number__token_type_reduced__token_type);
    token_type single_reducer_number__token_type_reduced_from__token_type(t_tok_number);
    token_type_reduced single_reducer_number__token_type_reduced_from(single_reducer_number__token_type_reduced_from__token_type);
    reducers::single single_reducer_number(single_reducer_number__token_type_reduced,
					   single_reducer_number__token_type_reduced_from);
    // reducers::single single_reducer_number(token_type_reduced(token_type(t_number)),
    // 					   token_type_reduced_from(token_type(t_tok_number)));
    reducers::seq seq_reducer_p_expr(token_type_reduced(token_type(t_p_expr)),
				     token_type_list_reduced_from({token_type(t_tok_lp),
						 token_type(t_expr),
						 token_type(t_tok_rp)
						 }));
    reducers::seq seq_reducer_add_expr(token_type_reduced(token_type(t_add_expr)),
				       token_type_list_reduced_from({token_type(t_expr),
						   token_type(t_tok_add),
						   token_type(t_expr)}));
    reducers::any any_reducer_expr(token_type_reduced(token_type(t_expr)),
				   token_type_list_reduced_from({token_type(t_number),
					       token_type(t_add_expr),
					       token_type(t_p_expr)}));
    std::list<abstract_reducer*> reducer_list({
	    (abstract_reducer*)&single_reducer_number,
		(abstract_reducer*)&seq_reducer_p_expr,
		(abstract_reducer*)&seq_reducer_add_expr,
		(abstract_reducer*)&any_reducer_expr});
    // 
    parser parser_(parser_stream_, reducer_list,
		   token_type(t_expr),
		   final_token_checker_policy::parse_until_eof,
		   self_parsed_policy::allow_self_parsed,
		   0);

    parser_result result = parser_.parser_parse();
    assert(result.get_parser_status() == parser_status::full_parsed);
    // pprint::cout << (token&)result;
    std::cout << ((token&)result).to_pprint_string();

    // (lisp-unit2:assert-true (lexer:token-typep token-parsed 'expr))
}
