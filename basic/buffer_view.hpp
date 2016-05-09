#ifndef __PARSER_BASIC_BUFFER_VIEW_HPP__
#define __PARSER_BASIC_BUFFER_VIEW_HPP__

#include <cassert>
#include "parser/basic/vector_view.hpp"
#include "parser/basic/buffer.hpp"

namespace parser_tools { namespace basic {

template<typename T>
class buffer_view : public vector_view<T> 
{
    buffer<T>* m_buffer;

public:
    typedef buffer_view<T> self;
    typedef typename buffer<T>::iterator iterator;

    buffer_view() {}
    buffer_view(buffer<T>& buf, const iterator& begin, const iterator& end) 
	: m_buffer(&buf), vector_view<T>(begin, end) {}


    ////////////////////////////////////////////////////////////////
    // 
    ////////////////////////////////////////////////////////////////
    friend self operator+(const self& l, const self& r) {
	assert(l.m_buffer == r.m_buffer);
	return self(*l.m_buffer, l.first, r.second);
    }

    friend self& operator+=(self& l, const self& r) {
	assert(l.m_buffer == r.m_buffer);
	vector_view<T>& l_view = l;
	const vector_view<T>& r_view = r;
	l_view = l_view + r_view;
	return l;
    }

    friend std::ostream& operator<<(std::ostream& os, const self& o) {
	return os << (const vector_view<T>&)o;
    }
};

}}
#endif	/* __PARSER_BASIC_BUFFER_VIEW_HPP__ */
