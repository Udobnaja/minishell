#include "executor_internal.h"

static int check_candidate(const char *dir, size_t len, const char *name, char out[PATH_MAX])
{
	size_t name_len;
	size_t need;

	name_len = ft_strlen(name);
	need = 0;
	if (len > 0)
		need = 1;
	need += name_len + len + 1;
	if(need > PATH_MAX)
		return 0;
	if(len > 0)
	{
		ft_memcpy(out, dir, len);
		out[len] = '/';
		out[len + 1] = '\0';
		ft_strlcat(out, name, PATH_MAX);
	} else
		ft_strlcpy(out, name, PATH_MAX);
	if(access(out, X_OK) == 0)
		return 1;
	return 0;
}
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
	path_env = env_get_value(sh->env_store, "PATH");
	if(path_env == NULL || path_env[0] == '\0' )
	{
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

void exec_child(const char *full, t_cmd *cmd, t_shell *sh)
{
	char			**envp;
	t_exec_result	result;
    t_exec_status   status;
	
	envp = env_to_envp(sh->env_store);
	if (envp == NULL)
	{
		result = exec_external_error_result(EXEC_ALLOC_ERR, NULL, 0);
		exit(result.exit_code);
	}
	execve(full, cmd->argv, envp);
	{		
		if (errno) 
		{
			result = exec_external_error_result(EXEC_ERR_GEN, cmd->argv[0], errno);
			env_free_envp(envp); 
			exit(result.exit_code);
		}
		result = exec_external_error_result(EXEC_ERR_EXECUTION, NULL, 0);
		env_free_envp(envp); 
		exit(result.exit_code);
	}
}

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
    return exec_external_result(EXEC_OK, sh_status_from_wait(status));
}

t_exec_result execute_external(t_shell *sh, t_cmd *cmd)
{
	char			full[PATH_MAX];
	pid_t			pid;
	t_exec_result	result;
    t_exec_status   status;
	
	if(cmd->argv[0][0] == '\0')
		return  exec_external_result(EXEC_OK, sh->last_status);
	full[0] = '\0';
	if(!cmd_path(sh, cmd->argv[0], full))
	{
		if (ft_strchr(cmd->argv[0], '/') == NULL)
			return (exec_external_error_result(
					EXEC_CMD_NOT_FOUND, cmd->argv[0], 0));
		else
			return (exec_external_error_result(
					EXEC_NO_SUCH_FILE, cmd->argv[0], 0));
	}
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
        status = apply_redirections(cmd);
        if(status != EXEC_OK)
        {
            result = exec_external_result(status, SH_GENERAL_ERROR);
            exit(result.exit_code);
        }
		exec_child(full, cmd, sh);
	}
    return wait_one(pid, cmd->argv[0]);
}
