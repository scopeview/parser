#ifndef __PARSER_LEXER_LEXER_FILTER_HPP__
#define __PARSER_LEXER_LEXER_FILTER_HPP__

#include "parser/filter/filter_status.hpp"
#include "parser/filter/filter.hpp"
#include "parser/token/token_type.hpp"
#include "parser/token/token.hpp"
#include "parser/stream/char_stream.hpp"

namespace parser_tools { namespace lexers {

class lexer_filter {
    std::shared_ptr<filters::abstract_filter_operation> m_filter_op;
    token_type m_token_type;

public:
    typedef std::pair<filters::filter_status, token> result_type;
    
    template<typename T, typename ignore = 
	     typename std::enable_if<std::is_base_of<filters::abstract_filter_operation, T>::value, T>::type>
    lexer_filter(token_type type, const T& t) : m_token_type(type) {
	std::shared_ptr<T> t_ptr = std::shared_ptr<T>(new T(t));
	m_filter_op = std::static_pointer_cast<filters::abstract_filter_operation, T>(t_ptr);
    }

    result_type lexer_filterate(streams::char_stream&);
};

}}
#endif	/* __PARSER_LEXER_LEXER_FILTER_HPP__ */
