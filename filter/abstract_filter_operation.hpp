#ifndef __PARSER_FILTER_ABSTRACT_FILTER_OPERATION_HPP__
#define __PARSER_FILTER_ABSTRACT_FILTER_OPERATION_HPP__

#include "parser/basic/buffer_view.hpp"
#include "parser/stream/char_stream.hpp"
#include "parser/filter/filter_status.hpp"

namespace parser_tools { namespace filters {

struct abstract_filter_operation {
    typedef std::pair<filter_status, basic::buffer_view<char>> result_type;

    virtual result_type filterate(streams::char_stream&) = 0;
    // virtual result_type filterate_furthest(streams::char_stream&) = 0;
};

}}
#endif	/* __PARSER_FILTER_ABSTRACT_FILTER_OPERATION_HPP__ */
