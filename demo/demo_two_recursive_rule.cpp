#include <string>
#include <cctype>
#include <sstream>
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
    t_double_definition,
    t_function_brace_body,
    t_function_call,
    t_function_call_argument_brace_list,
    t_function_call_argument_list,
    t_function_call_argument_multiple,
    t_function_call_argument_single,
    t_function_definition,
    t_function_name,
    t_function_parameter_brace_list,
    t_function_parameter_declaration,
    t_function_parameter_list,
    t_function_parameter_multiple,
    t_function_parameter_single,
    t_int_declaration,
    t_keyword_char,
    t_keyword_double,
    t_keyword_int,
    t_keyword_return,
    t_return_stmt,
    t_stmt_list,
    t_stmt_multiple,
    t_stmt_single,
    t_string,
    t_tok_comma,
    t_tok_equal,
    t_tok_lb,
    t_tok_lp,
    t_tok_rb,
    t_tok_rp,
    t_tok_semicomma,
    t_type,
    t_val,
    t_var,
    t_var_definition,
    t_var_or_val
};

static std::vector<std::string> token_type_enum_string_array({
	"t_double_definition",
	    "t_function_brace_body",
	    "t_function_call",
	    "t_function_call_argument_brace_list",
	    "t_function_call_argument_list",
	    "t_function_call_argument_multiple",
	    "t_function_call_argument_single",
	    "t_function_definition",
	    "t_function_name",
	    "t_function_parameter_brace_list",
	    "t_function_parameter_declaration",
	    "t_function_parameter_list",
	    "t_function_parameter_multiple",
	    "t_function_parameter_single",
	    "t_int_declaration",
	    "t_keyword_char",
	    "t_keyword_double",
	    "t_keyword_int",
	    "t_keyword_return",
	    "t_return_stmt",
	    "t_stmt_list",
	    "t_stmt_multiple",
	    "t_stmt_single",
	    "t_string",
	    "t_tok_comma",
	    "t_tok_equal",
	    "t_tok_lb",
	    "t_tok_lp",
	    "t_tok_rb",
	    "t_tok_rp",
	    "t_tok_semicomma",
	    "t_type",
	    "t_val",
	    "t_var",
	    "t_var_definition",
	    "t_var_or_val"
	    });

void test_two_recursive_rule()
{
    buffer<char> b_lp("(");
    buffer<char> b_rp(")");
    buffer<char> b_string(R"("%d\n")");
    buffer<char> b_string_j("j");
    buffer<char> b_comma(",");
    token tok_lp(token_type(t_tok_lp), token_buf(buffer_view<char>(b_lp, b_lp.begin(), b_lp.end())));
    token tok_rp(token_type(t_tok_rp), token_buf(buffer_view<char>(b_rp, b_rp.begin(), b_rp.end())));
    token tok_string(token_type(t_string), token_buf(buffer_view<char>(b_string, b_string.begin(), b_string.end())));
    token tok_string_j(token_type(t_string), token_buf(buffer_view<char>(b_string_j, b_string_j.begin(), b_string_j.end())));
    token tok_comma(token_type(t_tok_comma), token_buf(buffer_view<char>(b_comma, b_comma.begin(), b_comma.end())));

    token_type::register_token_type_map(token_type_enum_string_array);

    // ("%d\n", j);
    token_list tlist({tok_lp, tok_string, tok_comma, tok_string_j, tok_rp});

    token_stream token_stream_(tlist);
    std::shared_ptr<parsers::parser_stream> parser_stream_(new parser_stream(token_stream_));

    // reducer
    reducers::seq function_definition(token_type_reduced(token_type(t_function_definition)), 
				      token_type_list_reduced_from({token_type(t_type),
						  token_type(t_function_name),
						  token_type(t_function_parameter_brace_list),
						  token_type(t_function_brace_body)}));

    reducers::seq function_parameter_brace_list(token_type_reduced(token_type(t_function_parameter_brace_list)), 
						token_type_list_reduced_from({token_type(t_tok_lp),
							    token_type(t_function_parameter_list),
							    token_type(t_tok_rp)}));

    reducers::any function_parameter_list(token_type_reduced(token_type(t_function_parameter_list)), 
					  token_type_list_reduced_from({token_type(t_function_parameter_single),
						      token_type(t_function_parameter_multiple)}));

    reducers::single function_parameter_single(token_type_reduced(token_type(t_function_parameter_single)), 
					       token_type_reduced_from(token_type(t_function_parameter_declaration)));

    reducers::seq function_parameter_multiple(token_type_reduced(token_type(t_function_parameter_multiple)), 
					      token_type_list_reduced_from({token_type(t_function_parameter_list),
							  token_type(t_tok_comma),
							  token_type(t_function_parameter_single)}));

    reducers::single function_name(token_type_reduced(token_type(t_function_name)), 
				   token_type_reduced_from(token_type(t_string)));
    ;; 
    reducers::any function_parameter_declaration(token_type_reduced(token_type(t_function_parameter_declaration)), 
						 token_type_list_reduced_from({token_type(t_int_declaration)}));

    reducers::seq int_declaration(token_type_reduced(token_type(t_int_declaration)), 
				  token_type_list_reduced_from({token_type(t_keyword_int),
					      token_type(t_var)}));

    ;;
    reducers::seq function_brace_body(token_type_reduced(token_type(t_function_brace_body)), 
				      token_type_list_reduced_from({token_type(t_tok_lb),
						  token_type(t_stmt_list),
						  token_type(t_tok_rb)}));
    ;;

    reducers::any stmt_list(token_type_reduced(token_type(t_stmt_list)), 
			    token_type_list_reduced_from({token_type(t_stmt_single),
					token_type(t_stmt_multiple)}));

    reducers::any stmt_single(token_type_reduced(token_type(t_stmt_single)), 
			      token_type_list_reduced_from({token_type(t_var_definition),
					  token_type(t_function_call),
					  token_type(t_return_stmt)}));

    reducers::seq stmt_multiple(token_type_reduced(token_type(t_stmt_multiple)), 
				token_type_list_reduced_from({token_type(t_stmt_list),
					    token_type(t_stmt_single)}));
    ;;
    reducers::any var_definition(token_type_reduced(token_type(t_var_definition)), 
				 token_type_list_reduced_from({token_type(t_double_definition)}));

    reducers::seq double_definition(token_type_reduced(token_type(t_double_definition)), 
				    token_type_list_reduced_from({token_type(t_keyword_double),
						token_type(t_var),
						token_type(t_tok_equal),
						token_type(t_val),
						token_type(t_tok_semicomma)}));

    ;;
    reducers::seq function_call(token_type_reduced(token_type(t_function_call)), 
				token_type_list_reduced_from({token_type(t_function_name),
					    token_type(t_function_call_argument_brace_list),
					    token_type(t_tok_semicomma)}));

    reducers::seq function_call_argument_brace_list(token_type_reduced(token_type(t_function_call_argument_brace_list)), 
						    token_type_list_reduced_from({token_type(t_tok_lp),
								token_type(t_function_call_argument_list),
								token_type(t_tok_rp)}));

    reducers::any function_call_argument_list(token_type_reduced(token_type(t_function_call_argument_list)), 
					      token_type_list_reduced_from({token_type(t_function_call_argument_single),
							  token_type(t_function_call_argument_multiple)}));


    token_type single_reducer_function_call_argument_single__token_type_reduced__token_type(t_function_call_argument_single);
    token_type_reduced single_reducer_function_call_argument_single__token_type_reduced(single_reducer_function_call_argument_single__token_type_reduced__token_type);
    token_type single_reducer_function_call_argument_single__token_type_reduced_from__token_type(t_var_or_val);
    token_type_reduced single_reducer_function_call_argument_single__token_type_reduced_from(single_reducer_function_call_argument_single__token_type_reduced_from__token_type);
    reducers::single function_call_argument_single(single_reducer_function_call_argument_single__token_type_reduced, 
						   single_reducer_function_call_argument_single__token_type_reduced_from);

    reducers::seq function_call_argument_multiple(token_type_reduced(token_type(t_function_call_argument_multiple)), 
						  token_type_list_reduced_from({token_type(t_function_call_argument_list),
							      token_type(t_tok_comma),
							      token_type(t_function_call_argument_single)}));

    ;;
    reducers::seq return_stmt(token_type_reduced(token_type(t_return_stmt)), 
			      token_type_list_reduced_from({token_type(t_keyword_return),
					  token_type(t_var_or_val),
					  token_type(t_tok_semicomma)}));

    ;;
    reducers::any var_or_val(token_type_reduced(token_type(t_var_or_val)), 
			     token_type_list_reduced_from({token_type(t_var),
					 token_type(t_val)}));

    token_type single_reducer_var__token_type_reduced__token_type(t_var);
    token_type_reduced single_reducer_var__token_type_reduced(single_reducer_var__token_type_reduced__token_type);
    token_type single_reducer_var__token_type_reduced_from__token_type(t_string);
    token_type_reduced single_reducer_var__token_type_reduced_from(single_reducer_var__token_type_reduced_from__token_type);
    reducers::single var(single_reducer_var__token_type_reduced, 
			 single_reducer_var__token_type_reduced_from);

    token_type single_reducer_val__token_type_reduced__token_type(t_val);
    token_type_reduced single_reducer_val__token_type_reduced(single_reducer_val__token_type_reduced__token_type);
    token_type single_reducer_val__token_type_reduced_from__token_type(t_string);
    token_type_reduced single_reducer_val__token_type_reduced_from(single_reducer_val__token_type_reduced_from__token_type);
    reducers::single val(single_reducer_val__token_type_reduced, 
			 single_reducer_val__token_type_reduced_from);
    ;;
    reducers::any type(token_type_reduced(token_type(t_type)), 
		       token_type_list_reduced_from({token_type(t_keyword_int),
				   token_type(t_keyword_char),
				   token_type(t_keyword_double)}));

    std::list<abstract_reducer*> reducer_list({
	    (abstract_reducer*)&function_parameter_brace_list,
		(abstract_reducer*)&function_call_argument_brace_list,
		(abstract_reducer*)&function_call_argument_list,
		(abstract_reducer*)&function_call_argument_single,
		(abstract_reducer*)&function_call_argument_multiple,
		(abstract_reducer*)&var,
		(abstract_reducer*)&val,
		(abstract_reducer*)&type,
		(abstract_reducer*)&var_or_val,
		(abstract_reducer*)&var_definition,
		(abstract_reducer*)&double_definition
		});
    // 
    parser parser_(parser_stream_, reducer_list,
		   token_type(t_function_call_argument_brace_list),
		   final_token_checker_policy::parse_until_eof,
		   self_parsed_policy::allow_self_parsed,
		   0);

    parser_result result = parser_.parser_parse();
    assert(result.get_parser_status() == parser_status::full_parsed);
    // pprint::cout << (token&)result;
    std::cout << ((token&)result).to_pprint_string();
}
