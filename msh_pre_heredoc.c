#include "minishell.h"

t_parser_status msh_pre_heredocs(t_token_list *token_list)
{
	t_token_node	*cur;
	size_t			h_count;
	t_err_payload	payload;

	cur = token_list->head;
	payload = (t_err_payload){0};
	h_count = 0;
	while (cur)
	{
		if (cur->token->type == T_PIPE)
			h_count = 0;
		if (cur->token->type == T_HEREDOC)
			h_count++;
		if (h_count > HEREDOC_MAX)
		{
			err_print(ERR_HEREDOC, HEREDOC_EXCEED_LIMIT, payload);
			return (PARSE_HEREDOC_ERROR);
		}
		cur = cur->next;
	}
	return (PARSE_OK);
}

