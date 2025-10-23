#include "pipeline_internal.h"

static int	pipeline_init_capacity(t_cmd *cmd);
static int	pipeline_increase_capacity(t_cmd *cmd);

int	pipeline_push_cmd_argv(t_cmd *cmd, char *arg)
{
	if (cmd->capacity == 0)
	{
		if (!pipeline_init_capacity(cmd))
		{
			free(arg);
			return (0);
		}
	}
	else if (cmd->argc >= cmd->capacity)
	{
		if (!pipeline_increase_capacity(cmd))
		{
			free(arg);
			return (0);
		}
	}
	cmd->argv[cmd->argc++] = arg;
	cmd->argv[cmd->argc] = NULL;
	return (1);
}

void	pipeline_push_redirect(t_cmd *cmd, t_redirect *node)
{
	t_redirect	*cur;

	node->next = NULL;
	if (!cmd->redirect_list)
	{
		cmd->redirect_list = node;
		return ;
	}
	cur = cmd->redirect_list;
	while (cur->next)
		cur = cur->next;
	cur->next = node;
}

static int	pipeline_init_capacity(t_cmd *cmd)
{
	cmd->capacity = PIPELINE_ARGV_CAPACITY;
	cmd->argv = ft_calloc(cmd->capacity + 1, sizeof * cmd->argv);
	if (!cmd->argv)
	{
		cmd->capacity = 0;
		return (0);
	}
	return (1);
}

static int	pipeline_increase_capacity(t_cmd *cmd)
{
	char	**new_argv;
	size_t	new_capacity;

	if (cmd->capacity > SIZE_MAX / 2)
		return (0);
	new_capacity = cmd->capacity * 2;
	new_argv = ft_calloc(new_capacity + 1, sizeof * new_argv);
	if (!new_argv)
		return (0);
	ft_memcpy(new_argv, cmd->argv, cmd->argc * (sizeof(char *)));
	free(cmd->argv);
	cmd->argv = new_argv;
	cmd->capacity = new_capacity;
	return (1);
}
