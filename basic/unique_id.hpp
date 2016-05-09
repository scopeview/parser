#ifndef __PARSER_BASIC_UNIQUE_ID_HPP__
#define __PARSER_BASIC_UNIQUE_ID_HPP__

#include <iostream>

namespace parser_tools { namespace basic {

template<typename T>
class unique_id {
    static int s_last_id;

    int m_id;
public:
    typedef unique_id<T> self;
    unique_id() {
	m_id = unique_id::s_last_id++;
    }

    friend std::ostream& operator<<(std::ostream& os, const unique_id& o) {
	return os << o.m_id;
    }
    friend bool operator==(const self& l, const self& r) {
	return l.m_id == r.m_id;
    }
};

template<typename T>
int unique_id<T>::s_last_id = 0;

}}
#endif	/* __PARSER_BASIC_UNIQUE_ID_HPP__ */
