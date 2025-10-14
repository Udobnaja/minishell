#include "shell.h"

int	sh_status_from_signal(int sig)
{
	return (128 + sig);
}

 int	sh_status_from_errno_exec(int err)
 {
	if (err == ENOENT)
		return (SH_NOT_FOUND);
	if (err == EACCES || err == EISDIR || err == ENOEXEC)
		return (SH_NOT_EXECUTABLE);
	return (SH_GENERAL_ERROR);
}

int	sh_status_from_wait(int wst)
{
	if (WIFEXITED(wst))
		return (WEXITSTATUS(wst));
	if (WIFSIGNALED(wst))
		return (sh_status_from_signal(WTERMSIG(wst)));
	return (SH_GENERAL_ERROR);
}