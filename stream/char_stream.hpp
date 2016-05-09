#ifndef __PARSER_STREAM_CHAR_STREAM_HPP__
#define __PARSER_STREAM_CHAR_STREAM_HPP__

#include "parser/basic/_.hpp"
using namespace parser_tools::basic;

namespace parser_tools { namespace streams {

class char_stream : public buffer<char> {
public:
    static int eof;

    typedef buffer<char> container_type;

    char_stream(std::list<char>&);
    char_stream(std::string&);
    char_stream(filename&);

    container_type& get_buffer();

    int read_char();
    void unread_char(char);
    void unread_char(const std::list<char>&);

    container_type::iterator read_iterator();
    void unread_iterator(container_type::iterator&);
    void unread_iterator(container_type::iterator&, container_type::iterator&);
};

}}
#endif	/* __PARSER_STREAM_CHAR_STREAM_HPP__ */
