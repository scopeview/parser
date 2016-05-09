#ifndef __PARSER_BASIC_VECTOR_VIEW_HPP__
#define __PARSER_BASIC_VECTOR_VIEW_HPP__

#include <algorithm>
#include <list>
#include <vector>
#include <string>
#include <utility>

namespace parser_tools { namespace basic {

template<typename T>
class vector_view : public std::pair<typename std::vector<T>::iterator,
				     typename std::vector<T>::iterator>
{
public:
    typedef vector_view<T> self;
    typedef std::pair<typename std::vector<T>::iterator,
		      typename std::vector<T>::iterator> pair_type;
    typedef typename std::vector<T>::iterator iterator;

    vector_view() {};
    vector_view(std::pair<iterator, iterator>& pair) : pair_type(pair) {};
    vector_view(const iterator& begin, const iterator& end) : pair_type(begin, end) {};

    ////////////////////////////////////////////////////////////////
    // 
    ////////////////////////////////////////////////////////////////
    bool merge();


    ////////////////////////////////////////////////////////////////
    // 
    ////////////////////////////////////////////////////////////////
    std::list<T> to_list() {
	pair_type& pair = *this;
	return std::list<T>(pair.first, pair.second);
    }
    template<typename ignore = typename std::enable_if<std::is_same<T, char>::value>::type>
    std::string to_string() {
	pair_type& pair = *this;
	return std::string(pair.first, pair.second);
    }


    ////////////////////////////////////////////////////////////////
    // 
    ////////////////////////////////////////////////////////////////
    friend bool operator==(const self& l, const self& r) {
	return (pair_type(l)) == (pair_type(r));
    }

    friend self operator+(const self& l, const self& r) {
	assert(l.second == r.first);
	return self(l.first, r.second);
    }

    friend std::ostream& operator<<(std::ostream& os, const self& o) {
	std::for_each(o.first, o.second, [&](const T& t) {
		os << t;
	    });
	return os;
    }
};

}}
#endif	/* __PARSER_BASIC_VECTOR_VIEW_HPP__ */
