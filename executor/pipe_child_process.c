#include "executor_internal.h"

t_exec_result	child_process(t_pipe *p, size_t i)
{
	t_exec_result	result;

	if (i > 0)
	{
		result = exec_pipe_apply_stdio(p, p->prev[FD_READ], STDIN_FILENO);
		if (result.status != EXEC_OK)
			return (result);
	}
	if (i + 1 < p->n)
	{
		result = exec_pipe_apply_stdio(p, p->next[FD_WRITE], STDOUT_FILENO);
		if (result.status != EXEC_OK)
			return (result);
	}
	pipe_close(p);
	return (exec_external_result(EXEC_OK, SH_OK));
}

void	child_setup(t_pipe *p, size_t i)
{
	t_exec_result	result;

	result = child_process(p, i);
	if (result.status != EXEC_OK)
	{
		exec_child_process_clean(p->sh, p->pids, p->pl);
		exit(result.exit_code);
	}
}

void	run_child_process(t_pipe *p, size_t i)
{
	t_cmd			*cmd;
	char			path[PATH_MAX];
	t_exec_result	result;

	sh_setup_rl_hook(SH_CHILD);
	cmd = p->pl->cmds[i];
	child_setup(p, i);
	result = apply_redirections(cmd);
	if (result.status != EXEC_OK)
	{
		result = exec_external_result(result.status, SH_GENERAL_ERROR);
		exec_child_process_clean(p->sh, p->pids, p->pl);
		exit(result.exit_code);
	}
	if (cmd_is_empty(cmd))
	{
		result = exec_external_result(EXEC_OK, SH_OK);
		exec_child_process_clean(p->sh, p->pids, p->pl);
		exit(result.exit_code);
	}
	exec_run_buildin(p->sh, cmd, p->pids, p->pl);
	path[0] = '\0';
	exec_cmd_path(p, cmd, path);
	exec_child(path, cmd, p->sh, p);
}

t_exec_result	exec_make_pipe(t_pipe *p)
{
	t_exec_result	result;

	if (pipe(p->next) == -1)
	{
		result = exec_external_sys_error(EXEC_ERR_GEN, "pipe", errno);
		pipe_close(p);
		return (result);
	}
	return (exec_external_result(EXEC_OK, SH_OK));
}

t_exec_result	exec_fork_child(t_pipe *p, size_t i)
{
	t_exec_result	result;

	p->pids[i] = fork();
	if (p->pids[i] == -1)
	{
		result = exec_external_sys_error(EXEC_ERR_GEN, "fork", errno);
		pipe_close(p);
		return (result);
	}
	if (p->pids[i] == 0)
		run_child_process(p, i);
	return (exec_external_result(EXEC_OK, SH_OK));
}
