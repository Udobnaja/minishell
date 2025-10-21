#include "executor_internal.h"

static t_exec_result	exec_builtin_result(t_exec_status status)
{
	t_exec_result	result;

	result.flow = FLOW_OK;
	if (status != EXEC_OK)
		result.exit_code = SH_GENERAL_ERROR;
	else
		result.exit_code = SH_OK;
	result.status = status;
	result.errno_val = 0;
	return (result);
}

static t_exec_status	exec_smpl_builtin(t_shell *sh, t_cmd *cmd)
{
	if (cmd->builtin_kind == BUILTIN_ENV)
		return (env(sh, cmd));
	if (cmd->builtin_kind == BUILTIN_UNSET)
		return (unset(sh, cmd));
	if (cmd->builtin_kind == BUILTIN_EXPORT)
		return (export(sh, cmd));
	if (cmd->builtin_kind == BUILTIN_PWD)
		return (pwd(sh, cmd));
	if (cmd->builtin_kind == BUILTIN_ECHO)
		return (echo(cmd));
	return (cd(sh, cmd));
}

t_exec_result	execute_builtin(t_shell *sh, t_cmd *cmd)
{
	t_exec_status	status;

	if (cmd->builtin_kind == BUILTIN_EXIT)
		return (builtin_exit(sh, cmd));
	status = exec_smpl_builtin(sh, cmd);
	return (exec_builtin_result(status));
}

int	save_descriptors(int fd[3])
{
	fd[0] = dup(STDIN_FILENO);
	fd[1] = dup(STDOUT_FILENO);
	fd[2] = dup(STDERR_FILENO);
	if (fd[0] < 0 || fd[1] < 0 || fd[2] < 0)
	{
		close_fd(&fd[0]);
		close_fd(&fd[1]);
		close_fd(&fd[2]);
		return (-1);
	}
	return (0);
}
int	restore_descriptors(int fd[3])
{
	if (dup2(fd[0], STDIN_FILENO) < 0)
		return (-1);
	if (dup2(fd[1], STDOUT_FILENO) < 0)
		return (-1);
	if (dup2(fd[2], STDERR_FILENO) < 0)
		return (-1);
	return (0);
}
void	close_descriptors(int fd[3])
{
	close_fd(&fd[0]);
	close_fd(&fd[1]);
	close_fd(&fd[2]);
}

t_exec_result	exec_builtin_with_redirs(t_shell *sh, t_cmd *cmd)
{
	int				fd[3];
	t_exec_result	result;

	fd[0] = -1;
	fd[1] = -1;
	fd[2] = -1;
	if (save_descriptors(fd) < 0)
	{
		return (exec_external_error_result(EXEC_ERR_GEN, "dup", errno));
	}
	result = apply_redirections(cmd);
	if (result.status != EXEC_OK)
	{
		restore_descriptors(fd); // TODO: look
		close_descriptors(fd);
		return (result);
	}
	result = execute_builtin(sh, cmd);
	if (restore_descriptors(fd) < 0)
	{
		result = exec_external_error_result(EXEC_ERR_GEN, "dup2", errno);
	}
	close_descriptors(fd);
	return (result);
}

static void	exec_update_underscore(t_shell *sh, const t_cmd *cmd)
{
	const char		*val = NULL;
	t_env_status	status;

	if (cmd->argv && cmd->argc > 0 && cmd->argv[cmd->argc - 1])
		val = cmd->argv[cmd->argc - 1];
	if (!val)
		val = cmd->name;
	if (!val)
		val = "";
	status = env_set(sh->env_store, "_", val);
	if (status != ENV_OK)
		err_print(ERR_ENV, status, (t_err_payload){0});
}

t_exec_result	execute(t_shell *sh, t_pipeline *pipeline)
{
	t_exec_result result;

	ft_bzero(&result, sizeof result);
	if (pipeline->count > 0)
		exec_update_underscore(sh, pipeline->cmds[pipeline->count - 1]);
	if (pipeline->count > 1)
	{
		sh_setup_rl_hook(SH_JOB_NONE);
		result = execute_pipeline(sh, pipeline);
		sh_setup_rl_hook(SH_INTERACTIVE);
	}
	else if (pipeline->count == 1)
	{
		if (pipeline->cmds[0]->builtin_kind == BUILTIN_NONE)
		{
			sh_setup_rl_hook(SH_JOB_NONE);
			result = execute_external(sh, pipeline->cmds[0]);
			sh_setup_rl_hook(SH_INTERACTIVE);
		}
		else
			result = execute_builtin(sh, pipeline->cmds[0]);
	}
	return (result);
}