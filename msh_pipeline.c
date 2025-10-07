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
	{
		err_print(ERR_PARSER, PARSE_ALLOC_ERROR, (t_err_payload){0});
		return (PARSE_ALLOC_ERROR);
	}		
	return (PARSE_OK);
}


static t_builtin prs_cmd_to_builtin(const char *name)
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

t_parser_status prs_finish_cmd(t_cmd *cmd)
{
	char *default_name;

	if (!cmd->argv)
	{
		default_name = ft_strdup("");
		if (!default_name)
			return (PARSE_ALLOC_ERROR);
		if (!pipeline_push_cmd_argv(cmd, default_name))
		{
            free(default_name);
            return (PARSE_ALLOC_ERROR);
        }
	}
	cmd->name = ft_strdup(cmd->argv[0]);
	if (!cmd->name)
		return (PARSE_ALLOC_ERROR);
	cmd->builtin_kind = prs_cmd_to_builtin(cmd->name);	
	return (PARSE_OK);
}

t_parser_status	msh_pipeline(t_token_list *token_list, t_shell *shell, t_pipeline *pipeline)
{
	size_t			i;
	t_token_node	*cur;
	t_parser_status	status;
	size_t			cur_heredoc;

	status = msh_pipeline_init(token_list, pipeline);
	if (status != PARSE_OK)
		return (status);
	i = 0;
	cur_heredoc = 0;
	cur = token_list->head;
	while (cur)
	{
		if (cur->token->type == T_WORD)
		{
			status = prs_word_to_argv(&cur->token->word, shell, pipeline->cmds[i]);
			if (status != PARSE_OK)
			{
				err_print(ERR_PARSER, status, (t_err_payload){0});
				return (status);
			}	
		}
		else if (cur->token->type == T_PIPE)
		{
			status = prs_finish_cmd(pipeline->cmds[i]);
			if (status != PARSE_OK)
			{
				err_print(ERR_PARSER, status, (t_err_payload){0});
				return (status);
			}
			i++;
		} else
		{
			status = prs_redirect_to_pipe(cur, shell, pipeline->cmds[i], &cur_heredoc);
			if (status != PARSE_OK)
			{
				err_print(ERR_PARSER, status, (t_err_payload){0});
				return (status);
			}
			cur = cur->next->next;
			continue;
		}
		cur = cur->next;
	}	
	status = prs_finish_cmd(pipeline->cmds[i]);
	if (status != PARSE_OK)
		err_print(ERR_PARSER, status, (t_err_payload){0});

	return (status);
}

