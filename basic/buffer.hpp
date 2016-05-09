#ifndef __PARSER_BASIC_BUFFER_HPP__
#define __PARSER_BASIC_BUFFER_HPP__

#include <list>
#include <fstream>
#include <iterator>
#include "parser/basic/vector_view.hpp"
#include "parser/basic/indexed_vector.hpp"
#include "parser/basic/filename.hpp"

namespace parser_tools { namespace basic {

template<typename T>
class buffer : public indexed_vector<T>,
	       public filename
{
public:
    typedef buffer<T> self;
    typedef indexed_vector<T> container_type;
    typedef typename container_type::iterator iterator;

    buffer() {}
    buffer(std::list<T>& l) {
	init(l);
    };
    template<typename ignore = typename std::enable_if<std::is_same<T, char>::value>::type>
    buffer(const std::string& string) {
	indexed_vector<T>& iv = *this;
	iv.init(const_cast<char*>(string.c_str()), string.size());
    }
    buffer(filename& filename_) {
	init(filename_);
    }
    void init(std::list<T>& l) {
	indexed_vector<T>& iv = *this;
	iv.init(l);
    }
    void init(T* ptr, size_t size) {
	indexed_vector<T>& iv = *this;
	iv.init(ptr, size);
    }
    void init(filename& filename_) {
	std::ifstream ifs(filename_.get_filename());
    
	ifs.seekg(0, std::ios::end);
	int size = ifs.tellg();
	ifs.seekg(0, std::ios::beg);
	char* buffer = new char[size];

	ifs.read(buffer, size);
	init(buffer, size);
	delete[] buffer;

	filename& this_filename = *this;
	this_filename = filename_;
    }
    ~buffer() {}

    ////////////////////////////////////////////////////////////////
    // read/unread iterator (needn't keep consistence)
    ////////////////////////////////////////////////////////////////
    // T* read() {
    // 	container_type& iv = *this;
    // 	return iv.read();
    // }
    // void unread(const T& t) {
    // 	container_type& iv = *this;
    // 	iv.unread(t);
    // }

    // template<typename IT, typename ignore = typename std::enable_if<
    // 			      std::is_same<T, typename std::iterator_traits<IT>::value_type>::value>::type>
    // void unread(const IT& begin, const IT& end) {
    // 	container_type& iv = *this;
    // 	iv.unread(begin, end);
    // }

    bool validate(const vector_view<T>&);

    ////////////////////////////////////////////////////////////////
    // 
    ////////////////////////////////////////////////////////////////
    friend std::ostream& operator<<(std::ostream& os, self& o) {
	return os << static_cast<container_type&>(o);
    }
};

}}
#endif	/* __PARSER_BASIC_BUFFER_HPP__ */
