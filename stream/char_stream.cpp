#include "parser/stream/char_stream.hpp"

namespace parser_tools { namespace streams {

int char_stream::eof = std::char_traits<char>::eof();

char_stream::char_stream(std::list<char>& char_list)
    : buffer<char>(char_list)
{}

char_stream::char_stream(std::string& string)
    : buffer<char>(string)
{}

char_stream::char_stream(filename& filename_)
    : buffer<char>(filename_)
{}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
char_stream::container_type& char_stream::get_buffer()
{
    return *this;
}


////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
int char_stream::read_char()
{
    container_type& b = *this;
    char* ptr = b.read();
    if (!ptr) {
	return char_stream::eof;
    }
    return *ptr;
}

void char_stream::unread_char(char c)
{
    container_type& b = *this;
    assert(c != char_stream::eof);
    b.unread(c);
}

void char_stream::unread_char(const std::list<char>& char_list)
{
    std::for_each(char_list.rbegin(), char_list.rend(), [&](char c) {
	    this->unread_char(c);
	});
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
char_stream::container_type::iterator char_stream::read_iterator()
{
    container_type& b = *this;
    return b.read_iterator();
}

void char_stream::unread_iterator(container_type::iterator& it)
{
    container_type& b = *this;
    b.unread_iterator(it);
}

void char_stream::unread_iterator(iterator& begin, iterator& end)
{
    container_type& b = *this;
    b.unread_iterator(begin, end);
}

}}
