/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavrukh <alavrukh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 22:51:15 by alavrukh          #+#    #+#             */
/*   Updated: 2025/10/27 22:51:19 by alavrukh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor_internal.h"

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

t_exec_result	apply_redirs_temporarily(t_cmd *cmd)
{
	int				fd[3];
	t_exec_result	result;

	fd[0] = -1;
	fd[1] = -1;
	fd[2] = -1;
	if (save_descriptors(fd) < 0)
		return (exec_external_error_result(EXEC_ERR_GEN, "dup", errno));
	result = apply_redirections(cmd);
	if (result.status != EXEC_OK)
	{
		restore_descriptors(fd);
		close_descriptors(fd);
		return (result);
	}
	if (restore_descriptors(fd) < 0)
		result = exec_external_error_result(EXEC_ERR_GEN, "dup2", errno);
	close_descriptors(fd);
	return (result);
}

static void	exec_write_exit_ifneeded(t_builtin kind)
{
	if (kind == BUILTIN_EXIT && (isatty(STDIN_FILENO)))
		write(STDERR_FILENO, "exit", 4);
}

t_exec_result	execute(t_shell *sh, t_pipeline *pipeline)
{
	t_exec_result	result;

	ft_bzero(&result, sizeof result);
	if (pipeline->count <= 0)
		return (result);
	if (pipeline->count == 1)
	{
		if (pipeline->cmds[0]->builtin_kind == BUILTIN_NONE)
		{
			sh_setup_rl_hook(SH_JOB_NONE);
			result = execute_external(sh, pipeline);
			sh_setup_rl_hook(SH_JOB_RW);
			exec_update_underscore(sh, pipeline->cmds[pipeline->count - 1]);
			return (result);
		}
		exec_update_underscore(sh, pipeline->cmds[pipeline->count - 1]);
		exec_write_exit_ifneeded(pipeline->cmds[0]->builtin_kind);
		return (exec_builtin_with_redirs(sh, pipeline->cmds[0]));
	}
	env_set(sh->env_store, "_", "");
	sh_setup_rl_hook(SH_JOB_NONE);
	result = execute_pipeline(sh, pipeline);
	sh_setup_rl_hook(SH_JOB_RW);
	return (result);
}
