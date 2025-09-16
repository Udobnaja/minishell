#ifndef PARSER_H
# define PARSER_H

# include "pipeline.h"
# include "./lexer/lexer.h"

typedef enum e_parser_status {
	PARSE_OK = 0,
	PARSE_UNEXPECTED_TOKEN,
	PARSE_ALLOC_ERROR,
	PARSE_LEX_ERROR, // TODO: Upper level
	PARSE_HEREDOC_ERROR // TODO: Upper level
}	t_parser_status;

typedef struct s_pre_parse_result
{
	t_parser_status	status;
	t_token_node	*invalid;
}	t_pre_parse_result;

t_pre_parse_result	prs_pre_parse(t_token_list	*token_list);

#endif
