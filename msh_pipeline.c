#include "minishell.h"

static size_t msh_pipline_count_cmds(const t_token_list *token_list)
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

t_parser_status	msh_pipline_init(t_token_list *token_list, t_pipeline *pipeline)
{
	const size_t	cmds_count = msh_pipline_count_cmds(token_list);
	size_t			i;

	i = 0;
	pipeline->cmds = ft_calloc((size_t)cmds_count, sizeof *pipeline->cmds);
	if (!pipeline->cmds)
		return (PARSE_ALLOC_ERROR);
	while (i < cmds_count)
	{
		pipeline->cmds[i] = ft_calloc(1, sizeof *pipeline->cmds[i]);
		if (!pipeline->cmds[i]) {
			while (i--)
				free(pipeline->cmds[i]);
			free(pipeline->cmds);
			pipeline->cmds = NULL;
			return (PARSE_ALLOC_ERROR);
		}
		i++;
	}		
	pipeline->count = cmds_count;
	return (PARSE_OK);
}

static t_builtin msh_pipeline_to_builtin(const char *name)
{
	if (!ft_strcmp(name, "echo"))
		return BUILTIN_ECHO;
	if (!ft_strcmp(name, "cd"))
		return BUILTIN_CD;
	if (!ft_strcmp(name, "pwd"))
		return BUILTIN_PWD;
	if (!ft_strcmp(name, "export"))
		return BUILTIN_EXPORT;
	if (!ft_strcmp(name, "unset"))
		return BUILTIN_UNSET;
	if (!ft_strcmp(name, "env"))
		return BUILTIN_ENV;
	if (!ft_strcmp(name, "exit"))
		return BUILTIN_EXIT;
	return BUILTIN_NONE;
}

t_parser_status	msh_pipline_count_expanded(const char *str, t_shell *sh, size_t *consumed, size_t *total)
{
	char	*key;
	char 	*expanded;

	key = expn_dup_env_key(str);
	if (!key)
		return (PARSE_ALLOC_ERROR);
	expanded = expn_expand(key, sh->env_store, sh->last_status);
	if (!expanded)
	{
		free(key);
		return (PARSE_ALLOC_ERROR);
	}
	*total += ft_strlen(expanded);
	*consumed += ft_strlen(key);
	free(key);
	free(expanded);
	return (PARSE_OK);
}

t_parser_status msh_pipline_count_with_expansion(const char *str, t_shell *sh, size_t *total)
{
	size_t	j;
	t_parser_status status;

	j = 0;
	while (str[j])
	{
		if (str[j] == '$')
		{
			status = msh_pipline_count_expanded(str + j, sh, &j, total);
			if (status != PARSE_OK)
				return (status);
		}
		else
		{
			j++;
			*total += 1;
		}	
	}
	return (PARSE_OK);
}

t_parser_status	msh_pipline_count_total(const t_word *word, t_shell *sh, size_t *total)
{
	size_t			i;
	t_parser_status status;

	*total = 0;
	i = 0;
	while (i < word->count)
	{
		if (word->pieces[i].quote == SGL)
			*total += ft_strlen(word->pieces[i].text);
		else
		{
			status = msh_pipline_count_with_expansion(word->pieces[i].text, sh, total);
			if (status != PARSE_OK)
				return (status);
		}
		i++;
	}
	return (PARSE_OK);
}

void msh_pipline_join_until_expansion(const char *str, size_t *consumed, char **new_word)
{
	size_t i;

	i = 0;
	while (str[i] && str[i] != '$')
		i++;
	ft_memcpy(*new_word, str, i);
	*new_word += i; 
	*consumed += i;
}

static t_parser_status msh_pipline_join_expansion(const char *str, t_shell *sh, size_t *consumed, char **new_word)
{
	char	*key;
	char	*expanded;
	size_t	len;

	key = expn_dup_env_key(str);
	if (!key)
		return (PARSE_ALLOC_ERROR);	
	expanded = expn_expand(key, sh->env_store, sh->last_status);
	if (!expanded)
	{
		free(key);
		return (PARSE_ALLOC_ERROR);
	}
	len = ft_strlen(expanded);
	ft_memcpy(*new_word, expanded, len);
	*new_word += len; 	
	*consumed += (ft_strlen(key));	
	free(key);
	free(expanded);
	return (PARSE_OK);
}

t_parser_status	msh_pipline_join_expn(const char *str, t_shell *sh, char **new_word)
{
	size_t	j;
	t_parser_status status;

	j = 0;
	while (str[j])
	{
		msh_pipline_join_until_expansion(str + j, &j, new_word);
		if (str[j] == '$')
		{
			status = msh_pipline_join_expansion(str + j, sh, &j, new_word);
			if (status != PARSE_OK)
				return (status);
		}
	}
	return (PARSE_OK);
}

t_parser_status	msh_pipline_join_word(const t_word *word, t_shell *sh, char **new_word)
{
	size_t			i;
	size_t			total;
	char			*nw_p;
	size_t			len;
	t_parser_status	status;
	
	if (msh_pipline_count_total(word, sh, &total) != PARSE_OK)
		return (PARSE_ALLOC_ERROR);
	*new_word = malloc(total + 1);
	if (!*new_word)
		return (PARSE_ALLOC_ERROR);
	nw_p = *new_word;	
	i = 0;	
	while (i < word->count)
	{
		if (word->pieces[i].quote == SGL)
		{
			len = ft_strlen(word->pieces[i].text);
			ft_memcpy(nw_p, word->pieces[i].text, len);
			nw_p += len;
		}
		else
		{
			status = msh_pipline_join_expn(word->pieces[i].text, sh, &nw_p);
			if (status != PARSE_OK) {
				free(*new_word);
				*new_word = NULL;
				return (status);
			}
		}
		i++;
	}
	*nw_p = '\0';
	return (PARSE_OK);
}

// TODO: too long
static int msh_pipeline_push_cmd_argv(t_cmd *cmd, char *arg)
{
    size_t	argc;
	char	**new_argv;
	size_t	i;

	argc = 0;
    if (cmd->argv)
	{
		while (cmd->argv[argc])
			argc++;
	}
    new_argv = ft_calloc(argc + 2, sizeof *new_argv);
    if (!new_argv)
	{
		free(arg);
		return (0);
	}
	i = 0;
	while (i < argc)
	{
		new_argv[i] = cmd->argv[i];
		i++;
	}
	new_argv[argc] = arg;
    new_argv[argc + 1] = NULL;
    free(cmd->argv);
    cmd->argv = new_argv;
    return 1;
}


t_parser_status	msh_pipline(t_token_list *token_list, t_shell *shell, t_pipeline *pipeline)
{
	size_t			i;
	t_token_node	*cur;
	t_token_node	*prev;
	char			*arg;
	t_parser_status	status;

	if (msh_pipline_init(token_list, pipeline) != PARSE_OK)
		return (PARSE_ALLOC_ERROR);
	i = 0;
	cur = token_list->head;
	prev = NULL;
	arg = NULL;
	while (cur)
	{
		if (cur->token->type == T_WORD)
		{
			status = msh_pipline_join_word(&cur->token->word, shell, &arg);
			if (status != PARSE_OK)
			{
				return (status);
			}
			(void)(prev);
			//if (prev && prev->token->type prs_is_redirect && arg && !arg[0] or split count > 1
			// ambigiouse redirect
			if (!msh_pipeline_push_cmd_argv(pipeline->cmds[i], arg))
			{
				return (PARSE_ALLOC_ERROR);
			}
		}
		else if (cur->token->type == T_PIPE)
		{
			// cmd[i] - detect builtin and name
			// i + 1
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

// TODO: clenup / destroy pipeline
