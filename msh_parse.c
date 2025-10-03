#include "minishell.h"

static t_lex_status		msh_lex(const char *str, t_token_list *token_list);
static t_parser_status	msh_pre_parse(t_token_list *token_list);
static const char		*msh_token_to_char(t_token_type type);
static t_heredoc_status	msh_pre_parse_heredoc(t_token_list *token_list, t_shell *shell);

t_msh_parse_result	msh_parse(const char *str, t_shell *shell, t_pipeline *pipeline)
{
	t_token_list		*token_list;
	t_msh_parse_result	status;
	t_lex_status		tokenize_status;
	t_parser_status		parser_status;
	t_heredoc_status	heredoc_status;

	token_list = ft_calloc(1, sizeof *token_list);
	ft_bzero(&status, sizeof(status));
	if (!token_list)
	{
		status.domain = MPR_LEX;
		status.code.lex = LEX_ALLOC_ERROR;
		err_print(ERR_LEXER, status.code.lex, (t_err_payload){0});
		return (status);
	}
	tokenize_status = msh_lex(str, token_list);
	if (tokenize_status != LEX_OK)
	{
		status.domain = MPR_LEX;
		status.code.lex = tokenize_status;
		lex_destroy_token_list(&token_list);
		return (status);
	}
	parser_status = msh_pre_parse(token_list);
	if (parser_status != PARSE_OK)
	{
		status.domain = MPR_PARSE;
		status.code.parse = parser_status;
		lex_destroy_token_list(&token_list);
		return (status);
	}
	heredoc_status = msh_pre_parse_heredoc(token_list, shell);
	if (heredoc_status != HEREDOC_OK)
	{
		status.domain = MPR_HEREDOC;
		status.code.heredoc = heredoc_status;
		lex_destroy_token_list(&token_list);
		return (status);
	}
	parser_status = msh_pipeline(token_list, shell, pipeline);
	if (parser_status != PARSE_OK)
	{
		status.domain = MPR_PARSE;
		status.code.parse = parser_status;
		lex_destroy_token_list(&token_list);
		heredoc_store_clear(shell->heredoc_store);
		return (status);
	}
	lex_destroy_token_list(&token_list);
	return (status);
}

static const char *msh_token_to_char(t_token_type type)
{
	if (type == T_PIPE)
		return ("|");
	if (type == T_REDIR_IN)
		return ("<");
	if (type == T_REDIR_OUT)
		return (">");
	if (type == T_REDIR_APP)
		return (">>");
	if (type == T_HEREDOC)
		return ("<<");
	return ("?");
}

static t_lex_status	msh_lex(const char *str, t_token_list *token_list)
{
	t_lex_result	result;
	t_err_payload	payload;
	char			token[2];

	payload = (t_err_payload){0};
	result = lex_tokenize(str, token_list);
	if (result.status != LEX_OK)
	{
		if (result.invalid_char)
		{
			token[0] = result.invalid_char;
			token[1] = '\0';
			payload.token = token;	
		}	
		err_print(ERR_LEXER, result.status, payload);
		return (result.status);
	}
	return (result.status);
}

static const char *msh_err_token_label(t_token_node *invalid_node, size_t token_list_size)
{
	t_token_type type;

	type = invalid_node->token->type;
	if (type == T_PIPE && (token_list_size == 1 || invalid_node->next))
		return (msh_token_to_char(type));		
	if (!invalid_node->next)
		return "newline";
	return (msh_token_to_char(invalid_node->next->token->type));
}

static t_parser_status	msh_pre_parse(t_token_list *token_list)
{
	t_err_payload		payload;
	t_pre_parse_result	result;

	payload = (t_err_payload){0};
	result = prs_pre_parse(token_list);
	if (result.status != PARSE_OK)
	{
		payload.token = msh_err_token_label(result.invalid, token_list->size);
		err_print(ERR_PARSER, result.status, payload);
		return (result.status);
	}
	return (PARSE_OK);
}

static t_heredoc_status	msh_pre_parse_heredoc(t_token_list *token_list, t_shell *shell)
{
	t_heredoc_status	status;

	status = msh_pre_heredocs(token_list);
	if (status != HEREDOC_OK)
		return (status);
	status = msh_prepare_heredocs(token_list, shell);
	if (status != HEREDOC_OK)
	{
		heredoc_store_clear(shell->heredoc_store);
		return (status);
	}
	return (status);
}
