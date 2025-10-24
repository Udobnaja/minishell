#include "executor_internal.h"
/* 
Generates a full path to the executable file, checks its
existence and availability for execution
dir - path to the directory from the PATH variable
name - the name of the command
len - the length of the current segment from the PATH variable
out - the buffer into which the full path is written
*/

static int check_candidate(const char *dir, size_t len, const char *name, char out[PATH_MAX])
{
	size_t name_len;
	size_t need;

	name_len = ft_strlen(name);
	if(len == 0)
	{
		need = 2 + name_len + 1;
		if(need > PATH_MAX)
			return 0;
		ft_strlcpy(out, "./", 3);
		ft_strlcat(out, name, PATH_MAX);
	}
	else
	{
		need = len + 1 + name_len + 1;
		if(need > PATH_MAX)
			return 0;
		ft_memcpy(out, dir, len);
		out[len] = '/';
		out[len + 1] = '\0';
		ft_strlcat(out, name, PATH_MAX);
	}
	if (u_file_isdir(out))
		return (0); 
	if(access(out, X_OK) == 0)
		return 1;
	return 0;
}
/* 
Loop through all directories listed in PATH,
and use check_candidate() to check whether an executable 
file named name exists in any of them.
param i is the current index of the character in the string path
param start the beginning of the current directory

 */
static int search_in_path(const char *path, const char *name, char out[PATH_MAX])
{
	size_t i;
	size_t start;
	size_t len;

	i = 0;
	start = 0;
	while(1)
	{
		if(path[i] == ':' || path[i] == '\0')
		{
			len = i - start;
			if(check_candidate(path+start, len, name, out) != 0)
				return 1;
			if(path[i] == '\0')
				break;
			start = i + 1;
		}
		i++;
	}
	errno = ENOENT;
	return 0;
}

static int exec_is_dot_or_dotdot(const char *s) {
    return (s && (ft_strcmp(s, ".") == 0 || ft_strcmp(s, "..") == 0));
}
/* 
The function searches for the name command and writes 
the full path to out. Decides where to search (in PATH or in a given path)
If the file is found, it returns 1.
 */

int cmd_path(t_shell *sh, const char *name, char out[PATH_MAX])
{
	const char *path_env;
	if (name == NULL || out == NULL)
		return 0;
	if(ft_strchr(name, '/') != NULL)
	{
		ft_strlcpy(out, name, PATH_MAX);
		return 1;
	}
	 if (exec_is_dot_or_dotdot(name)) {
        errno = ENOENT;
        return (0);
    }
	path_env = env_get_value(sh->env_store, "PATH");
	if(path_env == NULL || path_env[0] == '\0' )
	{
		if(check_candidate(".", 1, name, out))
			return 1;
		errno = ENOENT;
		return 0;
	}
	if(search_in_path(path_env, name, out) != 0)
		return 1;
	return 0;
}

t_exec_result    exec_external_sys_error(t_exec_status status, const char *cmd, int errno_val)
{
	t_exec_result	result;
	t_err_payload	payload;

	result.flow = FLOW_OK;
	if (status == EXEC_CMD_NOT_FOUND || status == EXEC_NO_SUCH_FILE)
		result.exit_code = SH_NOT_FOUND;
	else if (status == EXEC_IS_DIRECTORY)
		result.exit_code = SH_NOT_EXECUTABLE;
	else
		result.exit_code = SH_GENERAL_ERROR;	
	result.status = status;
	result.errno_val = errno_val;

	payload = (t_err_payload){0};
	if (cmd)
		payload.command = cmd;
	if (errno_val)
		payload.errno_val = errno_val;
	err_print(ERR_EXEC, status, payload);
	return (result);
}

t_exec_result    exec_external_error_result(t_exec_status status, const char *cmd, int errno_val)
{
	t_exec_result	result;
	t_err_payload	payload;

	result.flow = FLOW_OK;
	if (status == EXEC_CMD_NOT_FOUND || status == EXEC_NO_SUCH_FILE)
		result.exit_code = SH_NOT_FOUND;
	else if (status == EXEC_IS_DIRECTORY)
		result.exit_code = SH_NOT_EXECUTABLE;
	else if (errno_val)
		result.exit_code = sh_status_from_errno_exec(errno_val);
	else
		result.exit_code = SH_GENERAL_ERROR;	
	result.status = status;
	result.errno_val = errno_val;

	payload = (t_err_payload){0};
	if (cmd)
		payload.command = cmd;
	if (errno_val)
		payload.errno_val = errno_val;
	err_print(ERR_EXEC, status, payload);
	return (result);
}

t_exec_result    exec_external_result(t_exec_status status, int exit_code)
{
	t_exec_result  result;

	result.flow = FLOW_OK;
	result.exit_code = (int)(unsigned char)exit_code;
	result.status = status;
	result.errno_val = 0;
	return (result);
}
/* 
The function checks whether the file at the specified path can be run
path - full path to the file ("/bin/cat")
argv - command name to print in the error message
 */

static t_exec_result preliminary_check(const char *path, char *argv)
{
	if(!u_file_exists(path))
		return (exec_external_error_result(
			EXEC_NO_SUCH_FILE, argv, 0));
	if (u_file_isdir(path))
	{
		return (exec_external_error_result(
			EXEC_IS_DIRECTORY, argv, 0));
	}
	errno = 0;
	if (access(path, X_OK) == -1)
	{
		if (errno)
			return (exec_external_error_result(
				EXEC_ERR_GEN, argv, errno));
		else
			return (exec_external_error_result(
					EXEC_ERR_EXECUTION, NULL, 0));
	}
	return exec_external_result(EXEC_OK, 0);
}

/*
 Runs an external executable in a child process using execve
 */

void exec_child(const char *full, t_cmd *cmd, t_shell *sh, t_pipe *p)
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

void exec_one_child(const char *full, t_cmd *cmd, t_shell *sh, t_pipeline *pl)
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

static t_exec_result wait_one(pid_t pid, const char *cmd)
{
    int   status;
    pid_t w_pid;

    status = 0;
    while (1)
    {
        w_pid = waitpid(pid, &status, 0);
        if (w_pid == -1 && errno == EINTR)
            continue;
        break;
    }
	if (w_pid == -1)
		return exec_external_sys_error(EXEC_ERR_GEN, (char *)cmd, errno);
	sh_report_wait_signal(status);
    return exec_external_result(EXEC_OK, sh_status_from_wait(status));
}
t_exec_result external_path(t_shell *sh, const char *name, char full[PATH_MAX])
{
	if(!cmd_path(sh, name, full))
	{
		if (ft_strchr(name, '/') == NULL)
			return (exec_external_error_result(
					EXEC_CMD_NOT_FOUND, name, 0));
		else
			return (exec_external_error_result(
					EXEC_NO_SUCH_FILE, name, 0));
	}
	return exec_external_result(EXEC_OK, sh->last_status);
}

t_exec_result execute_external(t_shell *sh, t_pipeline *pl)
{
	char			full[PATH_MAX];
	pid_t			pid;
	t_exec_result	result;
	t_cmd *cmd = pl->cmds[0];
	
	if(cmd->argv[0][0] == '\0')
		return  exec_external_result(EXEC_OK, sh->last_status);
	full[0] = '\0';
	result = external_path(sh, cmd->argv[0], full);
	if(result.status != EXEC_OK)
		return (result);
	result = preliminary_check(full, cmd->argv[0]);
	if(result.status != EXEC_OK)
		return (result);
	pid = fork();
	if(pid < 0)
		return (exec_external_sys_error(
					EXEC_ERR_GEN, "fork", errno));
	if(pid == 0)
	{
		sh_setup_rl_hook(SH_CHILD);
        result = apply_redirections(cmd);
        if(result.status != EXEC_OK)
		{
			exec_child_process_clean(sh, NULL, pl);
            exit(result.exit_code);
		}
		exec_one_child(full, cmd, sh, pl);
	}
    return wait_one(pid, cmd->argv[0]);
}
