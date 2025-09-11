#include "parser.h"

static int prs_is_redirect(t_token_type type)
{
	return (
		type == T_HEREDOC
		|| type == T_REDIR_APP
		|| type == T_REDIR_IN
		|| type == T_REDIR_OUT);
}

static t_token_node *prs_validate_pipe(t_token_node *pipe)
{
	t_token_node *cur;

	cur = pipe->next;
	if (!cur)
		return (pipe);
	if (cur->token->type == T_WORD)
		return (NULL);
	if (cur->token->type == T_PIPE)
		return (pipe);
	if (prs_is_redirect(cur->token->type))
	{
		if (!cur->next || cur->next->token->type != T_WORD)
			return (cur);
		return (NULL);	
	}
	return (cur);
}

static t_token_node *prs_validate_tokens_order(t_token_list	*token_list)
{
	t_token_node *cur;
	t_token_node *invalid;

	cur = token_list->head;
	if (cur->token->type == T_PIPE)
		return (cur);
	while (cur)
	{
		if (cur->token->type != T_WORD && !cur->next)
			return (cur);
		else if (cur->token->type == T_PIPE)
		{
			invalid = prs_validate_pipe(cur);	
			if (invalid)
				return (invalid);
		}		
		else if (prs_is_redirect(cur->token->type) && cur->next->token->type != T_WORD)
			return (cur);
		cur = cur->next;
	}
	return (NULL);
}

static t_pre_parse_result	prs_ok(void)
{
	t_pre_parse_result	result;

	result.status = PARSE_OK;
	result.invalid = NULL;
	return (result);
}

static t_pre_parse_result	prs_err(t_parser_status e, t_token_node *node)
{
	t_pre_parse_result	result;

	result.status = e;
	result.invalid = node;
	return (result);
}


t_pre_parse_result prs_pre_parse(t_token_list	*token_list)
{
	t_token_node *invalid_node; 

	invalid_node = prs_validate_tokens_order(token_list);
	if (invalid_node)
		return (prs_err(PARSE_UNEXPECTED_TOKEN, invalid_node));
	return (prs_ok());
}
