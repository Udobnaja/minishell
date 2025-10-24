#include "executor_internal.h"

/*
Generates a full path to the executable file, checks its
existence and availability for execution
dir - path to the directory from the PATH variable
name - the name of the command
len - the length of the current segment from the PATH variable
out - the buffer into which the full path is written
*/

int	check_candidate(const char *dir, size_t len, const char *name,
		char out[PATH_MAX])
{
	size_t	name_len;
	size_t	need;

	name_len = ft_strlen(name);
	if (len == 0)
	{
		need = 2 + name_len + 1;
		if (need > PATH_MAX)
			return (0);
		ft_strlcpy(out, "./", 3);
		ft_strlcat(out, name, PATH_MAX);
	}
	else
	{
		need = len + 1 + name_len + 1;
		if (need > PATH_MAX)
			return (0);
		ft_memcpy(out, dir, len);
		out[len] = '/';
		out[len + 1] = '\0';
		ft_strlcat(out, name, PATH_MAX);
	}
	if (access(out, X_OK) == 0)
		return (1);
	return (0);
}

/*
Loop through all directories listed in PATH,
and use	check_candidate(void) to check whether an executable
file named name exists in any of them.
param i is the current index of the character in the string path
param start the beginning of the current directory

 */
int	search_in_path(const char *path, const char *name, char out[PATH_MAX])
{
	size_t	i;
	size_t	start;
	size_t	len;

	i = 0;
	start = 0;
	while (1)
	{
		if (path[i] == ':' || path[i] == '\0')
		{
			len = i - start;
			if (check_candidate(path + start, len, name, out) != 0)
				return (1);
			if (path[i] == '\0')
				break ;
			start = i + 1;
		}
		i++;
	}
	errno = ENOENT;
	return (0);
}
/*
The function searches for the name command and writes
the full path to out. Decides where to search (in PATH or in a given path)
If the file is found, it returns 1.
 */

int	cmd_path(t_shell *sh, const char *name, char out[PATH_MAX])
{
	const char	*path_env;

	if (name == NULL || out == NULL)
		return (0);
	if (ft_strchr(name, '/') != NULL)
	{
		ft_strlcpy(out, name, PATH_MAX);
		return (1);
	}
	path_env = env_get_value(sh->env_store, "PATH");
	if (path_env == NULL || path_env[0] == '\0')
	{
		if (check_candidate(".", 1, name, out))
			return (1);
		errno = ENOENT;
		return (0);
	}
	if (search_in_path(path_env, name, out) != 0)
		return (1);
	return (0);
}

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

t_exec_result	external_path(t_shell *sh, const char *name,
		char full[PATH_MAX])
{
	if (!cmd_path(sh, name, full))
	{
		if (ft_strchr(name, '/') == NULL)
			return (exec_external_error_result(EXEC_CMD_NOT_FOUND, name, 0));
		else
			return (exec_external_error_result(EXEC_NO_SUCH_FILE, name, 0));
	}
	return (exec_external_result(EXEC_OK, sh->last_status));
}
