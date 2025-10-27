#include "executor_internal.h"

/*
The function checks whether the file at the specified path can be run
path - full path to the file ("/bin/cat")
argv - command name to print in the error message
 */

t_exec_result	preliminary_check(const char *path, char *argv)
{
	if (!u_file_exists(path))
		return (exec_external_error_result(EXEC_NO_SUCH_FILE, argv, 0));
	if (u_file_isdir(path))
	{
		return (exec_external_error_result(EXEC_IS_DIRECTORY, argv, 0));
	}
	errno = 0;
	if (access(path, X_OK) == -1)
	{
		if (errno)
			return (exec_external_error_result(EXEC_ERR_GEN, argv, errno));
		else
			return (exec_external_error_result(EXEC_ERR_EXECUTION, NULL, 0));
	}
	return (exec_external_result(EXEC_OK, 0));
}

int	exec_is_dot_or_dotdot(const char *s)
{
	return (s && (ft_strcmp(s, ".") == 0 || ft_strcmp(s, "..") == 0));
}

int	exec_check_in_curr_dir(const char *name, char out[PATH_MAX],
		const char *path)
{
	struct stat	st;

	if (check_candidate(".", 1, name, out))
		return (1);
	if (path == NULL)
	{
		if (stat(out, &st) == 0)
			return (1);
	}
	errno = ENOENT;
	return (0);
}
