#ifndef __PARSER_REDUCER_REDUCER_OF_SEQ_HPP__
#define __PARSER_REDUCER_REDUCER_OF_SEQ_HPP__

#include "parser/reducer/abstract_reducer.hpp"
#include "parser/reducer/token_type_reduced.hpp"
#include "parser/reducer/token_type_reduced_from.hpp"
#include "parser/reducer/token_type_list_reduced_from.hpp"

namespace parser_tools { namespace reducers {

class seq : public abstract_reducer,
	    public token_type_reduced,
	    public token_type_list_reduced_from
{
public:
    seq(const token_type_reduced&, const token_type_list_reduced_from&);

    ////////////////////////////////////////////////////////////////
    // abstract_reducer
    ////////////////////////////////////////////////////////////////
    virtual reducer_result reduce(streams::abstract_token_stream_operation_common&) override;
};

}}
#endif	/* __PARSER_REDUCER_REDUCER_OF_SEQ_HPP__ */
