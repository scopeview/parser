#ifndef __PARSER_REDUCER_REDUCER_OF_SINGLE_HPP__
#define __PARSER_REDUCER_REDUCER_OF_SINGLE_HPP__

#include "parser/reducer/abstract_reducer.hpp"
#include "parser/reducer/token_type_reduced.hpp"
#include "parser/reducer/token_type_reduced_from.hpp"

namespace parser_tools { namespace reducers {

class single : public abstract_reducer,
	       public token_type_reduced,
	       public token_type_reduced_from
{
public:
    single(const token_type_reduced&, const token_type_reduced_from&);

    ////////////////////////////////////////////////////////////////
    // abstract_reducer
    ////////////////////////////////////////////////////////////////
    virtual reducer_result reduce(streams::abstract_token_stream_operation_common&) override;
};

}}
#endif	/* __PARSER_REDUCER_REDUCER_OF_SINGLE_HPP__ */
