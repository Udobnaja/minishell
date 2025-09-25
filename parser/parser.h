#ifndef PARSER_H
# define PARSER_H

# include "./lexer/lexer.h"

typedef struct s_shell t_shell;
typedef struct s_cmd t_cmd;

typedef enum e_parser_status {
	PARSE_OK = 0,
	PARSE_UNEXPECTED_TOKEN,
	PARSE_ALLOC_ERROR,
	PARSE_LEX_ERROR, // TODO: Upper level
	PARSE_HEREDOC_ERROR, // TODO: Upper level
	PARSE_AMBIGUOUS_REDIRECT
}	t_parser_status;

typedef struct s_pre_parse_result
{
	t_parser_status	status;
	t_token_node	*invalid;
}	t_pre_parse_result;

t_pre_parse_result	prs_pre_parse(t_token_list	*token_list);

t_parser_status		prs_word_to_argv(
	const t_word *word,
	t_shell *sh, t_cmd *cmd
);

t_parser_status prs_redirect_to_pipe(
	t_token_node *token_node,
	t_shell *sh, t_cmd *cmd, size_t *cur_heredoc
);

#endif
