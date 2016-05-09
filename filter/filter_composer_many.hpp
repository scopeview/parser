#ifndef __PARSER_FILTER_FILTER_COMPOSER_MANY_HPP__
#define __PARSER_FILTER_FILTER_COMPOSER_MANY_HPP__

#include "parser/filter/filter.hpp"

namespace parser_tools { namespace filters {

class many_common : public abstract_filter_operation {
    bool m_is_match_empty;
    filter m_filter;

public:
    many_common(const filter&, bool is_match_empty);

    virtual result_type filterate(streams::char_stream&) override;
    // virtual result_type filterate_furthest(streams::char_stream&) override;
};

class many : public many_common {
public:
    many(const filter&);
};

class many_or_ellipsis : public many_common {
public:
    many_or_ellipsis(const filter&);
};

}}
#endif	/* __PARSER_FILTER_FILTER_COMPOSER_MANY_HPP__ */
