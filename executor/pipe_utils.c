#include "executor_internal.h"

pid_t	find_last_pid(const pid_t *pids, size_t n)
{
	size_t	i;

	i = n;
	while (i > 0)
	{
		i--;
		if (pids[i] > 0)
			return (pids[i]);
	}
	return (0);
}

int	cmd_is_empty(const t_cmd *cmd)
{
	if (cmd == NULL)
		return (1);
	if (cmd->argv == NULL)
		return (1);
	if (cmd->argv[0] == NULL)
		return (1);
	if (cmd->argv[0][0] == '\0')
		return (1);
	return (0);
}

void	exec_run_buildin(t_shell *sh, t_cmd *cmd, pid_t *pids, t_pipeline *pl)
{
	int	exit_code;

	if (cmd->builtin_kind != BUILTIN_NONE)
	{
		exit_code = execute_builtin(sh, cmd).exit_code;
		exec_child_process_clean(sh, pids, pl);
		exit(exit_code);
	}
}

void	exec_cmd_path(t_pipe *p, t_cmd *cmd, char path[PATH_MAX])
{
	t_exec_result	result;

	if (cmd_path(p->sh, cmd->argv[0], path) == 0)
	{
		if (ft_strchr(cmd->argv[0], '/') == NULL)
			result = exec_external_error_result(EXEC_CMD_NOT_FOUND,
					cmd->argv[0], 0);
		else
			result = exec_external_error_result(EXEC_NO_SUCH_FILE, cmd->argv[0],
					0);
		exec_child_process_clean(p->sh, p->pids, p->pl);
		exit(result.exit_code);
	}
}
