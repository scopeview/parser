#include <string>
#include <iostream>
#include "gtest/gtest.h"
#include "parser/token/_.hpp"
#include "parser/matcher/_.hpp"
#include "parser/filter/_.hpp"
#include "parser/lexer/_.hpp"

using namespace parser_tools;
using namespace parser_tools::lexers;

namespace {

std::string input_string(R"(char fun(int argc)
       {
       	double i = 1;
       	add(i, j);
       	return 0;
       })");
lexer_filter_group c_code(lexer_filter_group_name("c-code"));

////////////////////////////////////////////////////////////////
enum {
    t_keyword_char,
    t_keyword_double,
    t_keyword_int,
    t_keyword_return,
    t_string,
    t_tok_comma,
    t_tok_equal,
    t_tok_lb,
    t_tok_lp,
    t_tok_rb,
    t_tok_rp,
    t_tok_semicomma
};

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
    c_code.push_back(lexer_filter(token_type(t_string), 
				  filters::many(filters::filter(matchers::single_char_test_matcher([](char c) {return std::isalnum(c);})))));
    c_code.push_back(lexer_filter(token_type(), filters::many(filters::filter(matchers::any_char_matcher({' ', '\t', '\n'})))));
    // others
    c_code.push_back(lexer_filter(token_type(), filters::filter(matchers::single_char_test_matcher([](char) {return true;}))));
}

TEST(test_parser_lexer, lexer_complex)
{
    init();

    lexer l(input_string);
    l.add(c_code);
    l.set_filter_group(lexer_filter_group_name("c-code"));
    token t;
    
    // char
    t = l.read_token();
    EXPECT_EQ(token_type(t_keyword_char), static_cast<token_type&>(t));
    EXPECT_EQ(std::string("char"), static_cast<token_buf&>(t).to_string());
    // fun
    t = l.read_token();
    EXPECT_EQ(token_type(t_string), static_cast<token_type&>(t));
    EXPECT_EQ(std::string("fun"), static_cast<token_buf&>(t).to_string());
    // (
    t = l.read_token();
    EXPECT_EQ(token_type(t_tok_lp), static_cast<token_type&>(t));
    EXPECT_EQ(std::string("("), static_cast<token_buf&>(t).to_string());
    // int
    t = l.read_token();
    EXPECT_EQ(token_type(t_keyword_int), static_cast<token_type&>(t));
    EXPECT_EQ(std::string("int"), static_cast<token_buf&>(t).to_string());
    // argc
    t = l.read_token();
    EXPECT_EQ(token_type(t_string), static_cast<token_type&>(t));
    EXPECT_EQ(std::string("argc"), static_cast<token_buf&>(t).to_string());
    // )
    t = l.read_token();
    EXPECT_EQ(token_type(t_tok_rp), static_cast<token_type&>(t));
    EXPECT_EQ(std::string(")"), static_cast<token_buf&>(t).to_string());
    // {
    t = l.read_token();
    EXPECT_EQ(token_type(t_tok_lb), static_cast<token_type&>(t));
    EXPECT_EQ(std::string("{"), static_cast<token_buf&>(t).to_string());
    // double
    t = l.read_token();
    EXPECT_EQ(token_type(t_keyword_double), static_cast<token_type&>(t));
    EXPECT_EQ(std::string("double"), static_cast<token_buf&>(t).to_string());
    // i
    t = l.read_token();
    EXPECT_EQ(token_type(t_string), static_cast<token_type&>(t));
    EXPECT_EQ(std::string("i"), static_cast<token_buf&>(t).to_string());
    // =
    t = l.read_token();
    EXPECT_EQ(token_type(t_tok_equal), static_cast<token_type&>(t));
    EXPECT_EQ(std::string("="), static_cast<token_buf&>(t).to_string());
    // 1
    t = l.read_token();
    EXPECT_EQ(token_type(t_string), static_cast<token_type&>(t));
    EXPECT_EQ(std::string("1"), static_cast<token_buf&>(t).to_string());
    // ;
    t = l.read_token();
    EXPECT_EQ(token_type(t_tok_semicomma), static_cast<token_type&>(t));
    EXPECT_EQ(std::string(";"), static_cast<token_buf&>(t).to_string());
    // add
    t = l.read_token();
    EXPECT_EQ(token_type(t_string), static_cast<token_type&>(t));
    EXPECT_EQ(std::string("add"), static_cast<token_buf&>(t).to_string());
    // (
    t = l.read_token();
    EXPECT_EQ(token_type(t_tok_lp), static_cast<token_type&>(t));
    EXPECT_EQ(std::string("("), static_cast<token_buf&>(t).to_string());
    // i
    t = l.read_token();
    EXPECT_EQ(token_type(t_string), static_cast<token_type&>(t));
    EXPECT_EQ(std::string("i"), static_cast<token_buf&>(t).to_string());
    // ,
    t = l.read_token();
    EXPECT_EQ(token_type(t_tok_comma), static_cast<token_type&>(t));
    EXPECT_EQ(std::string(","), static_cast<token_buf&>(t).to_string());
    // j
    t = l.read_token();
    EXPECT_EQ(token_type(t_string), static_cast<token_type&>(t));
    EXPECT_EQ(std::string("j"), static_cast<token_buf&>(t).to_string());
    // )
    t = l.read_token();
    EXPECT_EQ(token_type(t_tok_rp), static_cast<token_type&>(t));
    EXPECT_EQ(std::string(")"), static_cast<token_buf&>(t).to_string());
    // ;
    t = l.read_token();
    EXPECT_EQ(token_type(t_tok_semicomma), static_cast<token_type&>(t));
    EXPECT_EQ(std::string(";"), static_cast<token_buf&>(t).to_string());
    // return
    t = l.read_token();
    EXPECT_EQ(token_type(t_keyword_return), static_cast<token_type&>(t));
    EXPECT_EQ(std::string("return"), static_cast<token_buf&>(t).to_string());
    // 0
    t = l.read_token();
    EXPECT_EQ(token_type(t_string), static_cast<token_type&>(t));
    EXPECT_EQ(std::string("0"), static_cast<token_buf&>(t).to_string());
    // ;
    t = l.read_token();
    EXPECT_EQ(token_type(t_tok_semicomma), static_cast<token_type&>(t));
    EXPECT_EQ(std::string(";"), static_cast<token_buf&>(t).to_string());
    // }
    t = l.read_token();
    EXPECT_EQ(token_type(t_tok_rb), static_cast<token_type&>(t));
    EXPECT_EQ(std::string("}"), static_cast<token_buf&>(t).to_string());
    // nil
    t = l.read_token();
    EXPECT_EQ(token_type(), static_cast<token_type&>(t));
}

}
