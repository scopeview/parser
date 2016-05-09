#ifndef __PARSER_FILTER_FILTER_COMPOSER_SEQ_HPP__
#define __PARSER_FILTER_FILTER_COMPOSER_SEQ_HPP__

#include "parser/filter/filter.hpp"

namespace parser_tools { namespace filters {

class seq : public abstract_filter_operation,
	    public std::list<filter>
{
public:
    typedef std::list<filter> filter_list;
    seq(const std::list<filter>&);

    virtual result_type filterate(streams::char_stream&) override;
    // virtual result_type filterate_furthest(streams::char_stream&) override;
};

}}
#endif	/* __PARSER_FILTER_FILTER_COMPOSER_SEQ_HPP__ */
