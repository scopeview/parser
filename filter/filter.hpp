#ifndef __PARSER_FILTER_FILTER_HPP__
#define __PARSER_FILTER_FILTER_HPP__

#include <memory>
#include "parser/filter/filter_status.hpp"
#include "parser/filter/abstract_filter_operation.hpp"
#include "parser/stream/char_stream.hpp"
#include "parser/matcher/abstract_matcher_operation.hpp"

using namespace parser_tools::matchers;

namespace parser_tools { namespace filters {

class filter : public abstract_filter_operation {
    std::shared_ptr<matchers::abstract_matcher_operation> m_matcher_op;

public:
    typedef abstract_filter_operation::result_type result_type;

    filter();

    template<typename T, typename ignore = 
	     typename std::enable_if<std::is_base_of<matchers::abstract_matcher_operation, T>::value, T>::type>
    filter(const T& t) {
	std::shared_ptr<T> t_ptr(new T(t));
	m_matcher_op = std::static_pointer_cast<matchers::abstract_matcher_operation, T>(t_ptr);
    }

    result_type do_filterate(streams::char_stream&);

    virtual result_type filterate(streams::char_stream&) override;
    // virtual result_type filterate_furthest(streams::char_stream&) override;
};

}}
#endif	/* __PARSER_FILTER_FILTER_HPP__ */
