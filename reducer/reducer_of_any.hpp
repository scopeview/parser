#ifndef __PARSER_REDUCER_REDUCER_OF_ANY_HPP__
#define __PARSER_REDUCER_REDUCER_OF_ANY_HPP__

#include "parser/reducer/abstract_reducer.hpp"
#include "parser/reducer/token_type_reduced.hpp"
#include "parser/reducer/token_type_list_reduced_from.hpp"

namespace parser_tools { namespace reducers {

class any : public abstract_reducer,
	    public token_type_reduced,
	    public token_type_list_reduced_from
{
public:
    any(const token_type_reduced&, const token_type_list_reduced_from&);

    ////////////////////////////////////////////////////////////////
    // abstract_reducer
    ////////////////////////////////////////////////////////////////
    virtual reducer_result reduce(streams::abstract_token_stream_operation_common&) override;
};

}}
#endif	/* __PARSER_REDUCER_REDUCER_OF_ANY_HPP__ */
