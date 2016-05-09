#ifndef __PARSER_PARSER_PARSER_POLICY_HPP__
#define __PARSER_PARSER_PARSER_POLICY_HPP__

namespace parser_tools { namespace parsers {

enum class self_parsed_policy {
    allow_self_parsed,
	not_allow_self_parsed
};

enum class final_token_checker_policy {
    parse_until_eof,
	return_immediately,
};

}}
#endif	/* __PARSER_PARSER_PARSER_POLICY_HPP__ */
