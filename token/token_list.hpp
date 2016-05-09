#ifndef __PARSER_TOKEN_TOKEN_LIST_HPP__
#define __PARSER_TOKEN_TOKEN_LIST_HPP__

#include <list>
#include <iostream>
#include "parser/token/token.hpp"

namespace parser_tools {

class token;
class token_list : public std::list<token> 
{
public:
    typedef token_list self;

    token_list() {}
    token_list(const std::list<token>& list) : std::list<token>(list) {};

    friend std::ostream& operator<<(std::ostream& os, const self& o) {
	os << "(";
	std::for_each(o.begin(), o.end(), [&](const token& t) {
		os << t << " ";
	    });
	os << ")";
	return os;
    }
};

}
#endif	/* __PARSER_TOKEN_TOKEN_LIST_HPP__ */
