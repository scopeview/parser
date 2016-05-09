#ifndef __PARSER_BASIC_INDEXED_VECTOR_HPP__
#define __PARSER_BASIC_INDEXED_VECTOR_HPP__

#include <algorithm>
#include <cstdlib>
#include <vector>
#include <list>
#include <iterator>
#include <cassert>

namespace parser_tools { namespace basic {

template<typename T>
class indexed_vector : public std::vector<T> {
public:
    typedef indexed_vector<T> self;
    typedef std::vector<T> container_type;
    typedef typename container_type::iterator iterator;

private:
    iterator m_it;
    bool m_is_keep_consistence;

public:
    indexed_vector() {}
    indexed_vector(T* ptr, size_t size) {
	init(ptr, size);
    }
    indexed_vector(std::list<T>& l) {
	init(l);
    }
    void init(T* ptr, size_t size) {
	std::vector<T>& v = *this;
	std::copy(ptr, ptr + size, std::back_inserter(v));
	m_it =this->begin();
    }
    void init(std::list<T>& l) {
	std::vector<T>& v = *this;
	std::copy(BE(l), std::back_inserter(v));
	m_it = this->begin();
    }
    template<typename IT, typename ignore = typename std::enable_if<
			      std::is_same<T, typename std::iterator_traits<IT>::value_type>::value>::type>
    void init(const IT& begin, const IT& end) {
	std::vector<T>& v = *this;
	std::copy(begin, end, std::back_inserter(v));
    }

    ////////////////////////////////////////////////////////////////
    // 
    ////////////////////////////////////////////////////////////////
    bool is_end() {
	container_type& c = *this;
	return m_it == c.end();
    }

    ////////////////////////////////////////////////////////////////
    // read/unread element (needn't keep consistence)
    ////////////////////////////////////////////////////////////////
    T* read() {
	std::vector<T>& v = *this;
	if (m_it != v.end()) {
	    return &*m_it++;
	}
	return nullptr;
    }

    void unread(const T& t) {
	std::vector<T>& v = *this;
	if (m_it != v.begin()) {
	    // TODO check consistence
	    *--m_it = t;
	    return;
	} else {
	    assert(0);
	}
    }

    template<typename IT, typename ignore = typename std::enable_if<
			      std::is_same<T, typename std::iterator_traits<IT>::value_type>::value>::type>
    void unread(const IT& begin, const IT& end) {
	std::vector<T>& v = *this;
	typename std::reverse_iterator<IT> rit_begin(end);
	typename std::reverse_iterator<IT> rit_end(begin);
	std::for_each(rit_begin, rit_end, [&](const T& t) {
		this->unread(t);
	    });
    }

    ////////////////////////////////////////////////////////////////
    // read/unread iterator (keep consistence)
    ////////////////////////////////////////////////////////////////
    iterator current_iterator() {
	return m_it;
    }

    void set_iterator(const iterator& it) {
	m_it = it;
    }

    iterator read_iterator() {
	container_type& c = *this;
	if (m_it == c.end()) {
	    return c.end();
	}
	return m_it++;
    }

    void unread_iterator(iterator& it) {
	--m_it;
	assert(m_it == it);
    }

    void unread_iterator(iterator& begin, iterator& end) {
	assert(end == m_it);
	m_it = begin;
    }

    iterator vector_index();
    // void set_block_index(off_t);

    ////////////////////////////////////////////////////////////////
    // 
    ////////////////////////////////////////////////////////////////
    friend std::ostream& operator<<(std::ostream& os, self& o) {
	container_type& v = o;
	os << "[(offset:" << std::distance(v.begin(), o.m_it) << ")";
	std::copy(o.m_it, v.end(), std::ostream_iterator<char>(os));
	os << "]";
	return os;
    }

};

}}
#endif	/* __PARSER_BASIC_INDEXED_VECTOR_HPP__ */
