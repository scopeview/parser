#include "parser/shifter/shifter_stream.hpp"

namespace parser_tools { namespace shifters {

shifter_stream::shifter_stream()
{}

shifter_stream::shifter_stream(const token& t)
{
    streams::token_stream& token_stream_ = *this;
    token_stream_.add_tail_token(t);
}

}}
