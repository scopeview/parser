#ifndef __PARSER_TOKEN_TOKEN_HPP__
#define __PARSER_TOKEN_TOKEN_HPP__

#include <list>
#include <iomanip> 
#include "parser/token/token_type.hpp"
#include "parser/token/token_buf.hpp"
// #include "parser/token/token_list.hpp"
#include "parser/basic/pprint.hpp"

namespace parser_tools {

class token;
typedef std::list<token> token_list;

// class token_list;
class token : public token_type,
	      public token_buf,
	      public token_list
{

public:
    typedef token self;

    static token nil;

    token();
    token(const token_type&);
    token(const token_type&, const token_buf&);
    token(const token_type&, const token_list&);

    bool is_terminal() const;
    bool is_nil() const;
    std::string to_string();
    std::string to_pprint_string();

    token_list to_orignal_token_list() const;

    ////////////////////////////////////////////////////////////////
    friend bool operator==(const token& l, const token& r) {
	return static_cast<const token_type&>(l) == static_cast<const token_type&>(r)
	    && static_cast<const token_buf&>(l) == static_cast<const token_buf&>(r)
	    && static_cast<const token_list&>(l) == static_cast<const token_list&>(r);
    }
    
    // friend std::ostream& operator<<(std::ostream& os, const self& o) {
    friend pprint::ostream& operator<<(pprint::ostream& os, const self& o) {
	int prefix = os.getw();
	os.disablew();
	// os.enablew();
	os << "#<token ";
	// os.incw(2);
	// os.get_ostream() << std::setw(os.getw()) << " " << (const token_type&)o;
	os.get_ostream() << (const token_type&)o << " ";
	// os.incw(std::string("#<token ").size());
	// os.enablew();
	if (o.is_terminal()) {
	    // os.get_ostream() << std::setw(os.getw()) << " " << (const token_buf&)o;
	    os.get_ostream() << (const token_buf&)o;
	} else {
	    os.get_ostream() << "\n";
	    os.enablew();
	    os.incw(2);
	    os.enablew();
	    os << "(";
	    const token_list& list = o;
	    std::for_each(list.begin(), list.end(), [&](const token& t) {
		    os << t;
		    os.get_ostream() << " ";
		});
	    os.disablew();
	    os << ")";
	    // os << (const token_list&)o;
	}
	os.disablew();
	os << ">";
	os.setw(prefix);
	return os;
    }

};

}
#endif	/* __PARSER_TOKEN_TOKEN_HPP__ */
