#include <string>
#include <iostream>
#include "gtest/gtest.h"
#include "parser/token/_.hpp"
#include "parser/matcher/_.hpp"
#include "parser/filter/_.hpp"
#include "parser/lexer/_.hpp"

namespace {

using namespace std;
using namespace parser_tools;
using namespace parser_tools::matchers;
using namespace parser_tools::filters;
using namespace parser_tools::streams;
using namespace parser_tools::lexers;

std::string input = "int fun(i, j);";
std::string pattern_string_keyword_int("int");
std::string pattern_string_tok_lp("(");
std::string pattern_string_tok_rp(")");
std::string pattern_string_tok_comma(",");
std::string pattern_string_tok_semicomma(";");
std::list<char> pattern_deliminator_list({' ', '\t'});

enum test_token_type {
    TOK_KEYWORD_INT,
    TOK_LP,
    TOK_RP,
    TOK_COMMA,
    TOK_SEMICOMMA,
    TOK_DELIMINATORS,
    TOK_STRING,
    TOK_OTHERS,
};

std::list<char> string_to_list(const std::string& str)
{
    std::list<char> list;
    std::copy(str.begin(), str.end(), std::back_inserter(list));
    return list;
}

TEST(test_parser_lexer, lexer)
{
    string_matcher matcher_string_keyword_int(pattern_string_keyword_int);
    string_matcher matcher_string_tok_lp(pattern_string_tok_lp);
    string_matcher matcher_string_tok_rp(pattern_string_tok_rp);
    string_matcher matcher_string_tok_comma(pattern_string_tok_comma);
    string_matcher matcher_string_tok_semicomma(pattern_string_tok_semicomma);
    any_char_matcher matcher_deliminator_list(pattern_deliminator_list);
    for_each_single_char_test_matcher::test_function_type f = [](char c) {
	return std::isalpha(c);
    };
    for_each_single_char_test_matcher matcher_for_each_single_char_test(f);

    filter filter_string_keyword_int(matcher_string_keyword_int);
    filter filter_string_tok_lp(matcher_string_tok_lp);
    filter filter_string_tok_rp(matcher_string_tok_rp);
    filter filter_string_tok_comma(matcher_string_tok_comma);
    filter filter_string_tok_semicomma(matcher_string_tok_semicomma);
    filter filter_deliminator({matcher_deliminator_list});
    filter filter_for_each_single_char(matcher_for_each_single_char_test);
    many filter_many_any_deliminator(filter_deliminator);
    many filter_many_single_char(filter_for_each_single_char);
    

    lexer_filter lexer_filter_string_keyword_int(token_type(TOK_KEYWORD_INT), filter_string_keyword_int);
    lexer_filter lexer_filter_string_tok_lp(token_type(TOK_LP), filter_string_tok_lp);
    lexer_filter lexer_filter_string_tok_rp(token_type(TOK_RP), filter_string_tok_rp);
    lexer_filter lexer_filter_string_tok_comma(token_type(TOK_COMMA), filter_string_tok_comma);
    lexer_filter lexer_filter_string_tok_semicomma(token_type(TOK_SEMICOMMA), filter_string_tok_semicomma);
    lexer_filter lexer_filter_string_tok_deliminator(token_type(TOK_DELIMINATORS), filter_many_any_deliminator);
    lexer_filter lexer_filter_string(token_type(TOK_STRING), filter_many_single_char);

    lexer_filter_group lexer_filter_group_(lexer_filter_group_name("c_code"));
    static_cast<lexer_filter_group::filter_list&>(lexer_filter_group_).push_back(lexer_filter_string_tok_deliminator);
    static_cast<lexer_filter_group::filter_list&>(lexer_filter_group_).push_back(lexer_filter_string_keyword_int);
    static_cast<lexer_filter_group::filter_list&>(lexer_filter_group_).push_back(lexer_filter_string_tok_lp);
    static_cast<lexer_filter_group::filter_list&>(lexer_filter_group_).push_back(lexer_filter_string_tok_rp);
    static_cast<lexer_filter_group::filter_list&>(lexer_filter_group_).push_back(lexer_filter_string_tok_comma);
    static_cast<lexer_filter_group::filter_list&>(lexer_filter_group_).push_back(lexer_filter_string_tok_semicomma);
    static_cast<lexer_filter_group::filter_list&>(lexer_filter_group_).push_back(lexer_filter_string);

    lexer l(input);
    l.add(lexer_filter_group_);
    l.set_filter_group(lexer_filter_group_name("c_code"));
    lexer_filter::result_type result;
    token t;
    
    // int
    t = l.read_token();
    EXPECT_EQ(token_type(TOK_KEYWORD_INT), static_cast<token_type&>(t));
    EXPECT_EQ(std::string("int"), static_cast<token_buf&>(t).to_string());
    // deliminator
    t = l.read_token();
    EXPECT_EQ(token_type(TOK_DELIMINATORS), static_cast<token_type&>(t));
    EXPECT_EQ(std::string(" "), static_cast<token_buf&>(t).to_string());
    // fun
    t = l.read_token();
    EXPECT_EQ(token_type(TOK_STRING), static_cast<token_type&>(t));
    EXPECT_EQ(std::string("fun"), static_cast<token_buf&>(t).to_string());
    // (
    t = l.read_token();
    EXPECT_EQ(token_type(TOK_LP), static_cast<token_type&>(t));
    EXPECT_EQ(std::string("("), static_cast<token_buf&>(t).to_string());
    // i
    t = l.read_token();
    EXPECT_EQ(token_type(TOK_STRING), static_cast<token_type&>(t));
    EXPECT_EQ(std::string("i"), static_cast<token_buf&>(t).to_string());
    // ,
    t = l.read_token();
    EXPECT_EQ(token_type(TOK_COMMA), static_cast<token_type&>(t));
    EXPECT_EQ(std::string(","), static_cast<token_buf&>(t).to_string());
    // deliminator
    t = l.read_token();
    EXPECT_EQ(token_type(TOK_DELIMINATORS), static_cast<token_type&>(t));
    EXPECT_EQ(std::string(" "), static_cast<token_buf&>(t).to_string());
    // j
    t = l.read_token();
    EXPECT_EQ(token_type(TOK_STRING), static_cast<token_type&>(t));
    EXPECT_EQ(std::string("j"), static_cast<token_buf&>(t).to_string());
    // )
    t = l.read_token();
    EXPECT_EQ(token_type(TOK_RP), static_cast<token_type&>(t));
    EXPECT_EQ(std::string(")"), static_cast<token_buf&>(t).to_string());
    // ;
    t = l.read_token();
    EXPECT_EQ(token_type(TOK_SEMICOMMA), static_cast<token_type&>(t));
    EXPECT_EQ(std::string(";"), static_cast<token_buf&>(t).to_string());
    // eof
    t = l.read_token();
    EXPECT_TRUE(t.is_nil());
}

}
