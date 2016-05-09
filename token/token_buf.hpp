#ifndef __PARSER_TOKEN_TOKEN_BUF_HPP__
#define __PARSER_TOKEN_TOKEN_BUF_HPP__

#include "parser/basic/buffer_view.hpp"

namespace parser_tools {

class token_buf : public basic::buffer_view<char> {
public:
    token_buf() {}
    token_buf(const basic::buffer_view<char>& view) : basic::buffer_view<char>(view) {}

    friend bool operator==(const token_buf& l, const token_buf& r) {
	return (basic::vector_view<char>(l)) == (basic::vector_view<char>(r));
    }

    friend std::ostream& operator<<(std::ostream& os, const token_buf& o) {
	return os << (const basic::buffer_view<char>&)o;
    }

};

}
#endif	/* __PARSER_TOKEN_TOKEN_BUF_HPP__ */
