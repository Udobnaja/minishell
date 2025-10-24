#include "executor_internal.h"

/*
 Runs an external executable in a child process using execve
 */

void	exec_child(const char *full, t_cmd *cmd, t_shell *sh, t_pipe *p)
{
	char			**envp;
	t_exec_result	result;

	envp = env_to_envp(sh->env_store);
	if (envp == NULL)
	{
		result = exec_external_error_result(EXEC_ALLOC_ERR, NULL, 0);
		exec_child_process_clean(sh, p->pids, p->pl);
		exit(result.exit_code);
	}
	execve(full, cmd->argv, envp);
	{
		result = exec_external_error_result(EXEC_ERR_GEN, cmd->argv[0], errno);
		env_free_envp(envp);
		exec_child_process_clean(sh, p->pids, p->pl);
		exit(result.exit_code);
	}
}

void	exec_one_child(const char *full, t_cmd *cmd, t_shell *sh,
		t_pipeline *pl)
{
	char			**envp;
	t_exec_result	result;

	envp = env_to_envp(sh->env_store);
	if (envp == NULL)
	{
		result = exec_external_error_result(EXEC_ALLOC_ERR, NULL, 0);
		exec_child_process_clean(sh, NULL, pl);
		exit(result.exit_code);
	}
	execve(full, cmd->argv, envp);
	{
		result = exec_external_error_result(EXEC_ERR_GEN, cmd->argv[0], errno);
		env_free_envp(envp);
		exec_child_process_clean(sh, NULL, pl);
		exit(result.exit_code);
	}
}
/*
the function waits for the child process
to complete and receives its exit code
*/

static t_exec_result	wait_one(pid_t pid, const char *cmd)
{
	int		status;
	pid_t	w_pid;

	status = 0;
	while (1)
	{
		w_pid = waitpid(pid, &status, 0);
		if (w_pid == -1 && errno == EINTR)
			continue ;
		break ;
	}
	if (w_pid == -1)
		return (exec_external_sys_error(EXEC_ERR_GEN, (char *)cmd, errno));
	sh_report_wait_signal(status);
	return (exec_external_result(EXEC_OK, sh_status_from_wait(status)));
}

void	process_child(t_shell *sh, t_pipeline *pl)
{
	char			full[PATH_MAX];
	t_exec_result	result;
	t_cmd			*cmd;

	cmd = pl->cmds[0];
	sh_setup_rl_hook(SH_CHILD);
	result = apply_redirections(cmd);
	if (result.status != EXEC_OK)
	{
		exec_child_process_clean(sh, NULL, pl);
		exit(result.exit_code);
	}
	exec_one_child(full, cmd, sh, pl);
}

t_exec_result	execute_external(t_shell *sh, t_pipeline *pl)
{
	char			full[PATH_MAX];
	pid_t			pid;
	t_exec_result	result;
	t_cmd			*cmd;

	cmd = pl->cmds[0];
	if (cmd->argv[0][0] == '\0')
		return (exec_external_result(EXEC_OK, sh->last_status));
	full[0] = '\0';
	result = external_path(sh, cmd->argv[0], full);
	if (result.status != EXEC_OK)
		return (result);
	result = preliminary_check(full, cmd->argv[0]);
	if (result.status != EXEC_OK)
		return (result);
	pid = fork();
	if (pid < 0)
		return (exec_external_sys_error(EXEC_ERR_GEN, "fork", errno));
	if (pid == 0)
		process_child(sh, pl);
	return (wait_one(pid, cmd->argv[0]));
}
