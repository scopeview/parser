#ifndef __PARSER_PARSER_PARSER_STREAM_HPP__
#define __PARSER_PARSER_PARSER_STREAM_HPP__

#include "parser/stream/abstract_token_stream_operation.hpp"
#include "parser/stream/token_stream.hpp"

namespace parser_tools { namespace parsers {

enum class parser_stream_undo_type
{
    all,
	parsed_token_stream,
};

class parser_stream : public streams::abstract_token_stream_operation_common
{
    std::shared_ptr<streams::token_stream> m_parsed_token_stream;
    streams::abstract_token_stream_operation_common* m_lexer;
    std::shared_ptr<token_list> m_token_streamer_read_token_list;

public:
    parser_stream(abstract_token_stream_operation_common&);

    std::shared_ptr<parser_stream> copy_class();


    ////////////////////////////////////////////////////////////////
    // abstract_token_stream_operation_common
    ////////////////////////////////////////////////////////////////
    virtual token peek_token() override;
    virtual token read_token() override;
    virtual void unread_token(const token&) override;
    virtual void unread_token(const token_list&) override;

    void undo(parser_stream_undo_type);
};

}}
#endif	/* __PARSER_PARSER_PARSER_STREAM_HPP__ */
