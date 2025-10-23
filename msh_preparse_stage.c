#include "minishell_internal.h"

static const char		*msh_token_to_char(t_token_type type);
static const char		*msh_err_token_label(t_token_node *invalid_node,
							size_t token_list_size);
static t_parser_status	msh_preparse(t_token_list *token_list);

int	msh_stage_preparse(t_token_list *token_list, t_msh_parse_result *status)
{
	const t_parser_status	parser_status = msh_preparse(token_list);

	if (parser_status != PARSE_OK)
	{
		status->domain = MPR_PARSE;
		status->code.parse = parser_status;
		return (0);
	}
	return (1);
}

static t_parser_status	msh_preparse(t_token_list *token_list)
{
	t_err_payload		payload;
	t_pre_parse_result	result;

	payload = (t_err_payload){0};
	result = prs_preparse(token_list);
	if (result.status != PARSE_OK)
	{
		payload.token = msh_err_token_label(result.invalid, token_list->size);
		err_print(ERR_PARSER, result.status, payload);
		return (result.status);
	}
	return (PARSE_OK);
}

static const char	*msh_err_token_label(t_token_node *invalid_node,
		size_t token_list_size)
{
	t_token_type	type;

	type = invalid_node->token->type;
	if (type == T_PIPE && (token_list_size == 1 || invalid_node->next))
		return (msh_token_to_char(type));
	if (!invalid_node->next)
		return ("newline");
	return (msh_token_to_char(invalid_node->next->token->type));
}

static const char	*msh_token_to_char(t_token_type type)
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
