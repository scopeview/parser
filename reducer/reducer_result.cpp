#include "parser/reducer/reducer_result.hpp"

namespace parser_tools { namespace reducers {

reducer_result::reducer_result(const reducer_status& status, const token& t)
    : m_reducer_status(status), m_token_reduced(t)
{
    assert(status != reducer_status::partial_reduced);
}

reducer_result::reducer_result(const reducer_status& status, const token_list& tlist)
    : m_reducer_status(status), m_partial_reduced_token_list(tlist)
{
    assert(status == reducer_status::partial_reduced);
}

////////////////////////////////////////////////////////////////
reducer_status reducer_result::get_reducer_status()
{
    return m_reducer_status;
}

token& reducer_result::get_token()
{
    assert(m_reducer_status != reducer_status::partial_reduced);
    return m_token_reduced;
}

token_list& reducer_result::get_token_list()
{
    assert(m_reducer_status == reducer_status::partial_reduced);
    return m_partial_reduced_token_list;
}

}}
