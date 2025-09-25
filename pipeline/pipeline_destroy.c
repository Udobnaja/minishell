#include "pipeline_internal.h"

static void pipeline_free_redirects(t_redirect *redirect);
static void	pipeline_argv(char **argv);
static void	pipeline_free_cmd(t_cmd *cmd);

void	pipeline_destroy(t_pipeline *pipeline)
{
	size_t	i;

	if (!pipeline || !pipeline->cmds)
		return;
	i = 0;
	while (i < pipeline->count)
	{
		if (pipeline->cmds[i])
			pipeline_free_cmd(pipeline->cmds[i]);
		i++;
	}
	free(pipeline->cmds);
	pipeline->cmds = NULL;
	pipeline->count = 0;
}

static void pipeline_free_redirects(t_redirect *redirect)
{
	t_redirect *next;

	while (redirect)
	{
		next = redirect->next;
		if (redirect->type == REDIR_HEREDOC)
		{
            if (redirect->target.fd >= 0)
                close(redirect->target.fd);
		}
		else if ((redirect->type == REDIR_IN || redirect->type == REDIR_OUT
			|| redirect->type == REDIR_APPEND) && redirect->target.path)
			free(redirect->target.path);
		free(redirect);
		redirect = next;
	}
}

static void	pipeline_argv(char **argv)
{
	size_t	i;

	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}	
	free(argv);
}

static void	pipeline_free_cmd(t_cmd *cmd)
{
	free(cmd->name);
	if (cmd->argv)
		pipeline_argv(cmd->argv);
	if (cmd->redirect_list)
		pipeline_free_redirects(cmd->redirect_list);
	free(cmd);
}