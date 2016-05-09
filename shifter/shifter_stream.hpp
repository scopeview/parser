#ifndef __PARSER_SHIFTER_SHIFTER_STREAM_HPP__
#define __PARSER_SHIFTER_SHIFTER_STREAM_HPP__

#include "parser/stream/token_stream.hpp"

namespace parser_tools { namespace shifters {

class shifter_stream : public streams::token_stream
{
public:
    shifter_stream();
    shifter_stream(const token&);
};

}}
#endif	/* __PARSER_SHIFTER_SHIFTER_STREAM_HPP__ */
