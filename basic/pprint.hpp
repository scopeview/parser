#ifndef __PPRINT_HPP__
#define __PPRINT_HPP__

#include <iostream>
#include <iomanip>

namespace pprint {

class ostream {
    int m_prefix;
    int m_step_width;
    bool m_is_print_prefix;
    std::ostream* m_std_ostream;

public:
    typedef ostream self;

    ostream(std::ostream& stream) : m_is_print_prefix(false), m_prefix(0), m_step_width(2), m_std_ostream(&stream) {};
    ostream(std::ostream& stream, int prefix) : m_is_print_prefix(false), m_prefix(prefix),
						      m_step_width(2), m_std_ostream(&stream) {};

    int getw() {return m_prefix;}
    void setw(int n) {m_prefix = n; m_is_print_prefix = true;}
    void incw(int n) {m_prefix+=n; m_is_print_prefix = true;}
    void decw(int n) {
	m_prefix-=n;
	if (m_prefix < 0)
	    m_prefix = 0;
	m_is_print_prefix = true;
    }
    void enablew() {m_is_print_prefix = true;}
    void disablew() {m_is_print_prefix = false;}
    std::ostream& get_ostream() {return *m_std_ostream;}

    template<typename T>
    self& operator<<(const T& t) {
	if (m_is_print_prefix) {
	    (*m_std_ostream) << std::setw(m_prefix);
	    m_is_print_prefix = false;
	}
	(*m_std_ostream) << t;
	return *this;
    }
};

extern ostream cout;

}
#endif	/* __PPRINT_HPP__ */
