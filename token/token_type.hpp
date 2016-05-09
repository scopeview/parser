#ifndef __PARSER_TOKEN_TOKEN_TYPE_HPP__
#define __PARSER_TOKEN_TOKEN_TYPE_HPP__

#include <iostream>
#include <vector>
#include <string>

namespace parser_tools {

class token_type {
    static std::vector<std::string>* s_token_type_map;

    int m_type;

public:
    static token_type nil;
    static void register_token_type_map(std::vector<std::string>&);


    token_type();
    token_type(int);

    bool is_type(const token_type&);

    friend std::ostream& operator<<(std::ostream& os, const token_type& o) {
	if (o.m_type == token_type::nil)
	    os << "nil";
	else
	    os << (*s_token_type_map)[o.m_type];
	return os;
    }

    friend bool operator==(const token_type& l, const token_type& r) {
	return l.m_type == r.m_type;
    }
};

}
#endif	/* __PARSER_TOKEN_TOKEN_TYPE_HPP__ */
