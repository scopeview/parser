#include "parser/token/token_type.hpp"

namespace parser_tools {

token_type token_type::nil = token_type(-1);

std::vector<std::string>* token_type::s_token_type_map = nullptr;

void token_type::register_token_type_map(std::vector<std::string>& map)
{
    s_token_type_map = &map;
}

////////////////////////////////////////////////////////////////
token_type::token_type() : m_type(-1)
{}

token_type::token_type(int type) : m_type(type)
{}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
bool token_type::is_type(const token_type& type)
{
    return m_type == type;
}


}
