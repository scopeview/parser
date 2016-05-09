#ifndef __PARSER_SHIFTER_SHIFTER_RESULT_HPP__
#define __PARSER_SHIFTER_SHIFTER_RESULT_HPP__

#include "parser/shifter/shifter_status.hpp"
#include "parser/token/token.hpp"

namespace parser_tools { namespace shifters {

class shifter_result : public token
{
    shifter_status m_shifter_status;
public:
    shifter_result() {}
    shifter_result(shifter_status status, const token& t) : m_shifter_status(status), token(t) {}

    void set_shifter_status(shifter_status status) {m_shifter_status = status;}
    shifter_status get_shifter_status() {return m_shifter_status;}
};

}}
#endif	/* __PARSER_SHIFTER_SHIFTER_RESULT_HPP__ */
