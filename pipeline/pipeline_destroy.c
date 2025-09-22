#include "pipeline_internal.h"

static void pipline_free_redirects(t_redirect *redirect);
static void	pipline_argv(char **argv);
static void	pipline_free_cmd(t_cmd *cmd);

void	pipline_destroy(t_pipeline *pipeline)
{
	size_t	i;

	if (!pipeline || !pipeline->cmds)
		return;
	i = 0;
	while (i < pipeline->count)
	{
		if (pipeline->cmds[i])
			pipline_free_cmd(pipeline->cmds[i]);
		i++;
	}
	free(pipeline->cmds);
	pipeline->cmds = NULL;
	pipeline->count = 0;
}

static void pipline_free_redirects(t_redirect *redirect)
{
	t_redirect *next;

	while (redirect)
	{
		next = redirect->next;
		// TODO: Think about FD!
		if ((redirect->type == REDIR_IN || redirect->type == REDIR_OUT || redirect->type == REDIR_APPEND || redirect->type == REDIR_HEREDOC)
			&& redirect->target.path)
			free(redirect->target.path);
		free(redirect);
		redirect = next;
	}
}

static void	pipline_argv(char **argv)
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

static void	pipline_free_cmd(t_cmd *cmd)
{
	free(cmd->name);
	if (cmd->argv)
		pipline_argv(cmd->argv);
	if (cmd->redirect_list)
		pipline_free_redirects(cmd->redirect_list);
	free(cmd);
}