#ifndef __PARSER_FILTER_FILTER_COMPOSER_ANY_HPP__
#define __PARSER_FILTER_FILTER_COMPOSER_ANY_HPP__

#include <list>
#include "parser/filter/filter.hpp"

namespace parser_tools { namespace filters {

class any_common : public abstract_filter_operation,
		   public std::list<filter> {
    bool m_is_match_empty;

public:
    typedef filter::result_type result_type;
    typedef std::list<filter> filter_list;
    any_common(const filter_list&, bool is_match_empty);

    virtual result_type filterate(streams::char_stream&) override;
};

class any : public any_common {
public:
    any(const std::list<filter>&);
};

class any_or_ellipsis : public any_common {
public:
    any_or_ellipsis(const std::list<filter>&);
};

}}
#endif	/* __PARSER_FILTER_FILTER_COMPOSER_ANY_HPP__ */
