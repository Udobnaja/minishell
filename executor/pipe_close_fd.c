#include "executor_internal.h"

void	close_fd(int *fd)
{
	if (*fd != -1)
	{
		close(*fd);
		*fd = -1;
	}
}

void	close_pair(int p[2])
{
	close_fd(&p[FD_READ]);
	close_fd(&p[FD_WRITE]);
}

void	pipe_parent_rotate(t_pipe *p)
{
	close_pair(p->prev);
	p->prev[FD_READ] = p->next[FD_READ];
	p->prev[FD_WRITE] = p->next[FD_WRITE];
	p->next[FD_READ] = -1;
	p->next[FD_WRITE] = -1;
	close_fd(&p->prev[FD_WRITE]);
}

void	pipe_close(t_pipe *p)
{
	close_pair(p->next);
	close_pair(p->prev);
}

t_exec_result	exec_pipe_apply_stdio(t_pipe *p, int std_in, int std_out)
{
	t_exec_result	result;
	const char		*cmd = "dup2";

	if (std_in == -1)
	{
		pipe_close(p);
		return (exec_external_sys_error(EXEC_ERR_GEN, cmd, EBADF));
	}
	if (dup2(std_in, std_out) == -1)
	{
		result = exec_external_sys_error(EXEC_ERR_GEN, cmd, errno);
		pipe_close(p);
		return (result);
	}
	return (exec_external_result(EXEC_OK, SH_OK));
}
