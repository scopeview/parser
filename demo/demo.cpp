#include <string>
#include <cctype>
#include "parser/parser.hpp"
#include "parser/lexer/_.hpp"
#include "parser/matcher/matcher_of_string.hpp"
#include "parser/matcher/matcher_of_any_char.hpp"
#include "parser/matcher/matcher_of_single_char_test.hpp"

using namespace parser_tools;
using namespace parser_tools::reducers;
using namespace parser_tools::lexers;
using namespace parser_tools::parsers;

std::string input_string(R"(char fun(int argc)
       {
       	double i = 1;
       	add(i, j);
       	return 0;
       })");
lexer_filter_group c_code(lexer_filter_group_name("c-code"));

////////////////////////////////////////////////////////////////
enum {
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
    t_var_or_val,
};

std::vector<std::string> token_type_string_map({
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

token_type function_parameter_single__token_type_reduced__token_type(t_function_parameter_single);
token_type_reduced function_parameter_single__token_type_reduced(function_parameter_single__token_type_reduced__token_type);
token_type function_parameter_single__token_type_reduced_from__token_type(t_function_parameter_declaration);
token_type_reduced_from function_parameter_single__token_type_reduced_from(function_parameter_single__token_type_reduced_from__token_type);
reducers::single function_parameter_single(function_parameter_single__token_type_reduced,
					   function_parameter_single__token_type_reduced_from);
// reducers::single function_parameter_single(token_type_reduced(token_type(t_function_parameter_single)),
// 					   token_type_reduced_from(token_type(t_function_parameter_declaration)));


reducers::seq function_parameter_multiple(token_type_reduced(token_type(t_function_parameter_multiple)),
					  token_type_list_reduced_from({token_type(t_function_parameter_list),
						      token_type(t_tok_comma),
						      token_type(t_function_parameter_single)}));

token_type function_name__token_type_reduced__token_type(t_function_name);
token_type_reduced function_name__token_type_reduced(function_name__token_type_reduced__token_type);
token_type function_name__token_type_reduced_from__token_type(t_string);
token_type_reduced_from function_name__token_type_reduced_from(function_name__token_type_reduced_from__token_type);
reducers::single function_name(function_name__token_type_reduced, function_name__token_type_reduced_from);
// reducers::single function_name(token_type_reduced(token_type(t_function_name)),
// 			       token_type_reduced_from(token_type(t_string)));

reducers::any function_parameter_declaration(token_type_reduced(token_type(t_function_parameter_declaration)),
					     token_type_list_reduced_from({token_type(t_int_declaration)}));
reducers::seq int_declaration(token_type_reduced(token_type(t_int_declaration)),
			      token_type_list_reduced_from({token_type(t_keyword_int),
					  token_type(t_var)}));
reducers::seq function_brace_body(token_type_reduced(token_type(t_function_brace_body)),
				  token_type_list_reduced_from({token_type(t_tok_lb),
					      token_type(t_stmt_list),
					      token_type(t_tok_rb)}));
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

reducers::any var_definition(token_type_reduced(token_type(t_var_definition)),
			     token_type_list_reduced_from({
				     token_type(t_double_definition)}));

reducers::seq double_definition(token_type_reduced(token_type(t_double_definition)),
				token_type_list_reduced_from({
					token_type(t_keyword_double), 
					    token_type(t_var), 
					    token_type(t_tok_equal), 
					    token_type(t_val), 
					    token_type(t_tok_semicomma)}));
//

reducers::seq function_call(token_type_reduced(token_type(t_function_call)),
			    token_type_list_reduced_from({
				    token_type(t_function_name), 
					token_type(t_function_call_argument_brace_list), 
					token_type(t_tok_semicomma)}));

reducers::seq function_call_argument_brace_list(token_type_reduced(token_type(t_function_call_argument_brace_list)),
						token_type_list_reduced_from({
							token_type(t_tok_lp), 
							    token_type(t_function_call_argument_list), 
							    token_type(t_tok_rp)}));

reducers::any function_call_argument_list(token_type_reduced(token_type(t_function_call_argument_list)),
					  token_type_list_reduced_from({
						  token_type(t_function_call_argument_single), 
						      token_type(t_function_call_argument_multiple)}));

token_type function_call_argument_single__token_type_reduced__token_type(t_function_call_argument_single);
token_type_reduced function_call_argument_single__token_type_reduced(function_call_argument_single__token_type_reduced__token_type);
token_type function_call_argument_single__token_type_reduced_from__token_type(t_var_or_val);
token_type_reduced_from function_call_argument_single__token_type_reduced_from(function_call_argument_single__token_type_reduced_from__token_type);
reducers::single function_call_argument_single(function_call_argument_single__token_type_reduced, function_call_argument_single__token_type_reduced_from);
// reducers::single function_call_argument_single(token_type_reduced(token_type(t_function_call_argument_single)),
// 					       token_type_reduced_from(token_type(t_var_or_val)));

reducers::seq function_call_argument_multiple(token_type_reduced(token_type(t_function_call_argument_multiple)),
					      token_type_list_reduced_from({
						      token_type(t_function_call_argument_list), 
							  token_type(t_tok_comma), 
							  token_type(t_function_call_argument_single)}));
//
reducers::seq return_stmt(token_type_reduced(token_type(t_return_stmt)),
			  token_type_list_reduced_from({
				  token_type(t_keyword_return), 
				      token_type(t_var_or_val), 
				      token_type(t_tok_semicomma)}));
//
reducers::any var_or_val(token_type_reduced(token_type(t_var_or_val)),
			 token_type_list_reduced_from({
				 token_type(t_var), 
				     token_type(t_val)}));

token_type var__token_type_reduced__token_type(t_var);
token_type_reduced var__token_type_reduced(var__token_type_reduced__token_type);
token_type var__token_type_reduced_from__token_type(t_string);
token_type_reduced_from var__token_type_reduced_from(var__token_type_reduced_from__token_type);
reducers::single var(var__token_type_reduced, var__token_type_reduced_from);
// reducers::single var(token_type_reduced(token_type(t_var)),
// 		     token_type_reduced_from(token_type(t_string)));

token_type val__token_type_reduced__token_type(t_val);
token_type_reduced val__token_type_reduced(val__token_type_reduced__token_type);
token_type val__token_type_reduced_from__token_type(t_string);
token_type_reduced_from val__token_type_reduced_from(val__token_type_reduced_from__token_type);
reducers::single val(val__token_type_reduced, val__token_type_reduced_from);
// reducers::single val(token_type_reduced(token_type(t_val)),
// 		     token_type_reduced_from(token_type(t_string)));
//
reducers::any type(token_type_reduced(token_type(t_type)),
		   token_type_list_reduced_from({
			   token_type(t_keyword_int), 
			       token_type(t_keyword_char), 
			       token_type(t_keyword_double)}));


std::list<abstract_reducer*> reducer_list({
	(abstract_reducer*)&function_parameter_single,
	    (abstract_reducer*)&stmt_single,
	    (abstract_reducer*)&function_name,
	    (abstract_reducer*)&function_call_argument_single,
	    (abstract_reducer*)&var,
	    (abstract_reducer*)&val,
	    (abstract_reducer*)&type,
	    (abstract_reducer*)&function_definition,
	    (abstract_reducer*)&function_parameter_brace_list,
	    (abstract_reducer*)&function_parameter_list,
	    (abstract_reducer*)&function_parameter_multiple,
	    (abstract_reducer*)&function_parameter_declaration,
	    (abstract_reducer*)&int_declaration,
	    (abstract_reducer*)&function_brace_body,
	    (abstract_reducer*)&stmt_list,
	    (abstract_reducer*)&stmt_multiple,
	    (abstract_reducer*)&var_definition,
	    (abstract_reducer*)&double_definition,
	    (abstract_reducer*)&function_call,
	    (abstract_reducer*)&function_call_argument_brace_list,
	    (abstract_reducer*)&function_call_argument_list,
	    (abstract_reducer*)&function_call_argument_multiple,
	    (abstract_reducer*)&return_stmt,
	    (abstract_reducer*)&var_or_val
	    });

void init()
{
    c_code.push_back(lexer_filter(token_type(t_keyword_char), filters::filter(matchers::string_matcher("char"))));
    c_code.push_back(lexer_filter(token_type(t_keyword_int), filters::filter(matchers::string_matcher("int"))));
    c_code.push_back(lexer_filter(token_type(t_keyword_double), filters::filter(matchers::string_matcher("double"))));
    c_code.push_back(lexer_filter(token_type(t_keyword_return), filters::filter(matchers::string_matcher("return"))));
    c_code.push_back(lexer_filter(token_type(t_tok_equal), filters::filter(matchers::string_matcher("="))));
    c_code.push_back(lexer_filter(token_type(t_tok_comma), filters::filter(matchers::string_matcher(","))));
    c_code.push_back(lexer_filter(token_type(t_tok_semicomma), filters::filter(matchers::string_matcher(";"))));
    c_code.push_back(lexer_filter(token_type(t_tok_lp), filters::filter(matchers::string_matcher("("))));
    c_code.push_back(lexer_filter(token_type(t_tok_rp), filters::filter(matchers::string_matcher(")"))));
    c_code.push_back(lexer_filter(token_type(t_tok_lb), filters::filter(matchers::string_matcher("{"))));
    c_code.push_back(lexer_filter(token_type(t_tok_rb), filters::filter(matchers::string_matcher("}"))));

    // TODO
    c_code.push_back(lexer_filter(token_type(), filters::many(filters::filter(matchers::any_char_matcher({' ', '\t', '\n'})))));
    c_code.push_back(lexer_filter(token_type(t_string), 
				  filters::many(filters::filter(matchers::single_char_test_matcher(
								    [](char c) {return std::isalnum(c);})))));
    // others
    c_code.push_back(lexer_filter(token_type(), filters::filter(matchers::single_char_test_matcher([](char) {return true;}))));

    token_type::register_token_type_map(token_type_string_map);
}

////////////////////////////////////////////////////////////////
/// simple c language function definition (with lexer)
////////////////////////////////////////////////////////////////
void test()
{
    init();

    // lexer
    lexers::lexer l(input_string);
    l.add(c_code);
    l.set_filter_group(lexers::lexer_filter_group_name("c-code"));
    
    // parser_stream
    std::shared_ptr<parsers::parser_stream> parser_stream(new parsers::parser_stream(l));

    // parser
    parsers::parser p(parser_stream, reducer_list, 
		      token_type(t_function_definition), 
		      parsers::final_token_checker_policy::parse_until_eof,
		      parsers::self_parsed_policy::allow_self_parsed,
		      0);
    
    parser_result result = p.parser_parse();
    assert(result.get_parser_status() == parser_status::full_parsed);
    std::cout << ((token&)result).to_pprint_string();
}

extern void test_simple_calculation();
extern void test_recursive_rule();
extern void test_two_recursive_rule();

int main(int argc, char * argv[])
{
    parser_tools_init();

    test();
    // test_simple_calculation();
    // test_recursive_rule();
    // test_two_recursive_rule();
    return 0;
}

