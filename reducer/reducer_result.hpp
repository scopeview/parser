#ifndef __PARSER_REDUCER_REDUCER_RESULT_HPP__
#define __PARSER_REDUCER_REDUCER_RESULT_HPP__

#include <list>
#include <cassert>
#include "parser/reducer/reducer_status.hpp"
#include "parser/token/token.hpp"

namespace parser_tools { namespace reducers {

class reducer_result {
    reducer_status m_reducer_status;
    token_list m_partial_reduced_token_list;
    token m_token_reduced;

public:
    reducer_result(const reducer_status&, const token&);
    reducer_result(const reducer_status&, const token_list&);

    reducer_status get_reducer_status();
    token& get_token();
    token_list& get_token_list();
};

}}
#endif	/* __PARSER_REDUCER_REDUCER_RESULT_HPP__ */
