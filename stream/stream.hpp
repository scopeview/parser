#ifndef __PARSER_STREAM_STREAM_HPP__
#define __PARSER_STREAM_STREAM_HPP__

namespace parser_tools { namespace streams {

// traits
// - element type char
// - need keep consistence
// - head only
// - no cache

// consider return a std::unique_ptr<token>
class abstract_token_stream {
    operator<<();
    operator>>();
};

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
struct stream_traits;

template<>
struct stream_traits<token_stream> {
    typedef is_keep_consistence std::false_type;
    typedef is_enable_tail_operation std::true_type;
    typedef is_cached std::false_type;
};


template<>
struct stream_traits<lexer_stream> {
    
};


}}
#endif	/* __PARSER_STREAM_STREAM_HPP__ */
