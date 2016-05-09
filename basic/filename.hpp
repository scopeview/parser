#ifndef __PARSER_BASIC_FILENAME_HPP__
#define __PARSER_BASIC_FILENAME_HPP__

#include <string>

namespace parser_tools { namespace basic {

class filename {
    std::string m_filename;

public:
    filename();
    filename(const std::string&);
    std::string get_filename() const;
};

}}
#endif	/* __PARSER_BASIC_FILENAME_HPP__ */
