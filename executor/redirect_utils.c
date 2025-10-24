#include "executor_internal.h"

t_exec_result	dup2_end_close(int fd, int target, const char *name)
{
	int				error;
	t_exec_status	status;

	if (fd == target)
		return (exec_external_result(EXEC_OK, SH_OK));
	if (dup2(fd, target) < 0)
	{
		error = errno;
		close(fd);
		status = redir_status_from_errno(error);
		return (exec_external_sys_error(status, name, error));
	}
	close(fd);
	return (exec_external_result(EXEC_OK, SH_OK));
}
