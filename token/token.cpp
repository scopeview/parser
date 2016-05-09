#include <sstream>
#include "parser/token/token.hpp"

namespace parser_tools {

token token::nil = token(token_type::nil);

////////////////////////////////////////////////////////////////
token::token()
{
    *this = token::nil;
}

token::token(const token_type& token_type_)
    : token_type(token_type_)
{}

token::token(const token_type& token_type_, const token_buf& token_buf_)
    : token_type(token_type_), token_buf(token_buf_)
{}

token::token(const token_type& token_type_, const token_list& token_list_)
    : token_type(token_type_), token_list(token_list_)
{}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
bool token::is_terminal() const {
    const token_list& tl = *this;
    return tl.empty();
}

bool token::is_nil() const {
    const token_type& type = *this;
    return type == token_type();
}

std::string token::to_string()
{
    token_buf& token_buf_ = *this;
    std::stringstream ss;
    ss << token_buf_;
    return ss.str();
}

std::string token::to_pprint_string()
{
    std::stringstream ss;
    pprint::ostream pss(ss);
    pss << *this;
    return ss.str();
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
token_list token::to_orignal_token_list() const
{
    if (is_terminal())
	return token_list({*this});

    token_list result;
    const token_list& list = *this;
    std::for_each(list.begin(), list.end(), [&](const token& t) {
	    token_list sub_result(t.to_orignal_token_list());
	    std::copy(sub_result.begin(), sub_result.end(), std::back_inserter(result));
	});
    return result;
}


}

