#include "shell_internal.h"

static t_lex_status		sh_lex(const char *str, t_token_list *token_list);
static t_parser_status	sh_pre_parse(t_token_list *token_list);
static const char		*sh_token_to_char(t_token_type type);

t_parser_status	sh_parse(const char *str)
{
	t_token_list	*token_list;
	t_parser_status	status;
	t_lex_status	tokenize_status;

	token_list = ft_calloc(1, sizeof *token_list); // TODO: возможно это унести на уровень инита
	if (!token_list)
	{
		status = PARSE_ALLOC_ERROR;
		err_print(ERR_LEXER, status, (t_err_payload){0});
		return (status);
	}
	tokenize_status = sh_lex(str, token_list);
	if (tokenize_status != LEX_OK)
	{
		lex_destroy_token_list(&token_list);
		return (PARSE_LEX_ERROR);
	}
	if (sh_pre_parse(token_list) != PARSE_OK)
	{
		lex_destroy_token_list(&token_list);
		return (PARSE_UNEXPECTED_TOKEN);
	}
	lex_destroy_token_list(&token_list);
	return (PARSE_OK);
}

static const char *sh_token_to_char(t_token_type type)
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

static t_lex_status	sh_lex(const char *str, t_token_list *token_list)
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

static const char *sh_token_error_payload(t_token_node *invalid_node, size_t token_list_size)
{
	t_token_type type;

	type = invalid_node->token->type;
	if (type == T_PIPE && (token_list_size == 1 || invalid_node->next))
		return (sh_token_to_char(type));		
	if (!invalid_node->next)
		return "newline";
	return (sh_token_to_char(invalid_node->next->token->type));
}

static t_parser_status	sh_pre_parse(t_token_list *token_list)
{
	t_err_payload		payload;
	t_pre_parse_result	result;

	payload = (t_err_payload){0};
	result = prs_pre_parse(token_list);
	if (result.status != PARSE_OK)
	{
		payload.token = sh_token_error_payload(result.invalid, token_list->size);
		err_print(ERR_PARSER, result.status, payload);
		return (result.status);
	}
	return (PARSE_OK);
}
