#include "executor_internal.h"

pid_t	process_pid(pid_t pid, int *st)
{
	pid_t	w_pid;

	while (1)
	{
		w_pid = waitpid(pid, st, 0);
		if (w_pid == -1 && errno == EINTR)
			continue ;
		break ;
	}
	return (w_pid);
}

int	get_last_code(int st)
{
	int	last_code;

	last_code = sh_status_from_wait(st);
	sh_report_wait_signal(st);
	return (last_code);
}

t_exec_result	wait_all(pid_t *pids, size_t n)
{
	size_t		i;
	int			last_code;
	int			st;
	pid_t		w_pid;
	const pid_t	last_pid = find_last_pid(pids, n);

	last_code = 0;
	i = 0;
	while (i < n)
	{
		if (pids[i] > 0)
		{
			st = 0;
			w_pid = process_pid(pids[i], &st);
			if (w_pid == -1)
				return (exec_external_sys_error(EXEC_ERR_GEN, "waitpid",
						errno));
			if (pids[i] == last_pid)
				last_code = get_last_code(st);
		}
		i++;
	}
	return (exec_external_result(EXEC_OK, last_code));
}

t_exec_result	execution_run_pipeline(t_pipe *p)
{
	size_t			i;
	t_exec_result	result_fork;
	t_exec_result	result_pipe;

	i = 0;
	p->prev[FD_READ] = -1;
	p->prev[FD_WRITE] = -1;
	p->next[FD_READ] = -1;
	p->next[FD_WRITE] = -1;
	while (i < p->n)
	{
		if (i + 1 < p->n)
		{
			result_pipe = exec_make_pipe(p);
			if (result_pipe.status != EXEC_OK)
				return (result_pipe);
		}
		result_fork = exec_fork_child(p, i);
		if (result_fork.status != EXEC_OK)
			return (result_fork);
		pipe_parent_rotate(p);
		i++;
	}
	pipe_close(p);
	return (exec_external_result(EXEC_OK, SH_OK));
}

t_exec_result	execute_pipeline(t_shell *sh, t_pipeline *pl)
{
	t_pipe			p;
	t_exec_result	result;
	t_exec_result	w_result;

	p.sh = sh;
	p.pl = pl;
	p.n = pl->count;
	p.pids = ft_calloc(p.n, sizeof(pid_t));
	if (p.pids == NULL)
		return (exec_external_sys_error(EXEC_ALLOC_ERR, NULL, 1));
	result = execution_run_pipeline(&p);
	w_result = wait_all(p.pids, p.n);
	free(p.pids);
	if (result.status != EXEC_OK)
		return (result);
	return (w_result);
}
