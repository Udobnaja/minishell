#include "executor_internal.h"

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
		return (exec_external_error_result(EXEC_ERR_GEN, "dup", errno));
	result = apply_redirections(cmd);
	if (result.status != EXEC_OK)
	{
		restore_descriptors(fd);
		close_descriptors(fd);
		return (result);
	}
	result = execute_builtin(sh, cmd);
	if (restore_descriptors(fd) < 0)
		result = exec_external_error_result(EXEC_ERR_GEN, "dup2", errno);
	close_descriptors(fd);
	return (result);
}
