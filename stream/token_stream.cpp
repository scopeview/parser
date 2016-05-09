#include <algorithm>
#include <utility>
#include "parser/stream/token_stream.hpp"

namespace parser_tools { namespace streams {

token_stream::token_stream()
{}

token_stream::token_stream(const std::list<token>& list)
    : container_type(list)
{}

////////////////////////////////////////////////////////////////
// abstract_token_stream_operation_common
////////////////////////////////////////////////////////////////
token token_stream::peek_token()
{
    token t;
    container_type& c = *this;
    if (!c.empty()) {
	t = *c.begin();
    }
    return t;
}

token token_stream::read_token()
{
    token t;
    container_type& c = *this;
    if (!c.empty()) {
	t = *c.begin();
	c.pop_front();
    }
    return t;
}

void token_stream::unread_token(const token& t)
{
    using namespace std::rel_ops;
    container_type& c = *this;
    if (t != token::nil)
	c.push_front(t);
}

void token_stream::unread_token(const token_list& l)
{
    std::for_each(l.rbegin(), l.rend(), [&](const token& t) {
	    this->unread_token(t);
	});
}

////////////////////////////////////////////////////////////////
// abstract_token_stream_operation_length
////////////////////////////////////////////////////////////////
int token_stream::size()
{
    container_type& c = *this;
    return c.size();
}

////////////////////////////////////////////////////////////////
// abstract_token_stream_operation_tail
////////////////////////////////////////////////////////////////
int token_stream::add_tail_token(const token& t)
{
    container_type& c = *this;
    c.push_back(t);
}

token token_stream::read_tail_token()
{
    container_type& c = *this;
    token t;
    if (c.size()) {
	t = c.back();
	c.pop_back();
    }
    return t;
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////
token_list token_stream::read_token_all()
{
    container_type& this_token_list = *this;
    token_list list(this_token_list);
    this_token_list.clear();
    
    return list;
}

token_list token_stream::peek_token_all()
{
    return static_cast<container_type&>(*this);
}

}}
