#include "minishell_internal.h"

static size_t			msh_pipeline_count_cmds(const t_token_list *token_list);

int	msh_stage_pipeline(t_shell *shell, t_pipeline *pipeline, t_token_list *token_list, t_msh_parse_result	*status)
{
	const t_parser_status	parser_status = msh_pipeline(token_list, shell, pipeline);
	if (parser_status != PARSE_OK)
	{
		status->domain = MPR_PARSE;
		status->code.parse = parser_status;
		return (0);
	}
	return (1);
}

t_parser_status	msh_pipeline_init(t_token_list *token_list, t_pipeline *pipeline)
{
	const size_t	cmds_count = msh_pipeline_count_cmds(token_list);
	
	if (!pipeline_init(cmds_count, pipeline))
	{
		err_print(ERR_PARSER, PARSE_ALLOC_ERROR, (t_err_payload){0});
		return (PARSE_ALLOC_ERROR);
	}		
	return (PARSE_OK);
}

static size_t msh_pipeline_count_cmds(const t_token_list *token_list)
{
	size_t			count;
	t_token_node	*cur;

	count = 0;
	cur = token_list->head;
	while (cur)
	{
		if (cur->token->type == T_PIPE)
			count++;
		cur = cur->next;
	}
	return (count + 1);
}