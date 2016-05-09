#include "parser/basic/filename.hpp"

namespace parser_tools { namespace basic {

filename::filename()
{}

filename::filename(const std::string& filename) : m_filename(filename)
{}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
std::string filename::get_filename() const
{
    return m_filename;
}

}}
