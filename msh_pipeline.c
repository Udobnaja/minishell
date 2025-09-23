#include "minishell.h"

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

t_parser_status	msh_pipeline_init(t_token_list *token_list, t_pipeline *pipeline)
{
	const size_t	cmds_count = msh_pipeline_count_cmds(token_list);
	
	if (!pipeline_init(cmds_count, pipeline))
		return (PARSE_ALLOC_ERROR);		
	return (PARSE_OK);
}


// parse part
static t_builtin msh_pipeline_to_builtin(const char *name)
{
	if (!ft_strcmp(name, "echo"))
		return (BUILTIN_ECHO);
	if (!ft_strcmp(name, "cd"))
		return (BUILTIN_CD);
	if (!ft_strcmp(name, "pwd"))
		return (BUILTIN_PWD);
	if (!ft_strcmp(name, "export"))
		return (BUILTIN_EXPORT);
	if (!ft_strcmp(name, "unset"))
		return (BUILTIN_UNSET);
	if (!ft_strcmp(name, "env"))
		return (BUILTIN_ENV);
	if (!ft_strcmp(name, "exit"))
		return (BUILTIN_EXIT);
	return (BUILTIN_NONE);
}



// write errros here
t_parser_status	msh_pipeline(t_token_list *token_list, t_shell *shell, t_pipeline *pipeline)
{
	size_t			i;
	t_token_node	*cur;
	t_token_node	*prev;
	t_parser_status	status;

	status = msh_pipeline_init(token_list, pipeline);
	if (status != PARSE_OK)
		return (status);
	i = 0;
	cur = token_list->head;
	prev = NULL;
	while (cur)
	{
		if (cur->token->type == T_WORD)
		{
			status = prs_word_to_argv(&cur->token->word, prev, shell, pipeline->cmds[i]);
			if (status != PARSE_OK)
				return (status);
		}
		else if (cur->token->type == T_PIPE)
		{
			// cmd[i] - detect builtin and name
			i++;
		} else
		{
			// setup redirects
		}
		prev = cur;
		cur = cur->next;
	}

	// TODO: name
	pipeline->cmds[i]->builtin_kind = msh_pipeline_to_builtin(pipeline->cmds[i]->argv[0]);
		
	return (PARSE_OK);
}

