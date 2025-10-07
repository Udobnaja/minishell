#include "minishell_internal.h"

static t_lex_status	msh_lex(const char *str, t_token_list *token_list);

int	msh_stage_init_tokens(t_token_list **token_list, t_msh_parse_result	*status)
{
    *token_list = ft_calloc(1, sizeof **token_list);
	if (!*token_list)
	{
		status->domain = MPR_LEX;
		status->code.lex = LEX_ALLOC_ERROR;
		err_print(ERR_LEXER, status->code.lex, (t_err_payload){0});
		return (0);
	}
    return (1);
}

int	msh_stage_tokenize(const char *str, t_token_list *token_list, t_msh_parse_result	*status)
{
	const t_lex_status	tokenize_status = msh_lex(str, token_list);
	if (tokenize_status != LEX_OK)
	{
		status->domain = MPR_LEX;
		status->code.lex = tokenize_status;
		return (0);
	}
	return (1);
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