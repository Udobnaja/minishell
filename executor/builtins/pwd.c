#include "executor_internal.h"

t_exec_status pwd(t_shell *sh, t_cmd cmd)
{
	const char *cwd;
	const char *pwd;
	t_err_payload payload;

	payload = (t_err_payload){0};
	if(exec_is_invalid_option(cmd.argv[1]))
	{
		payload.command = "pwd";
		err_print(ERR_EXEC, EXEC_ERR_INVALID_OPTION, payload);
		return EXEC_ERR_INVALID_OPTION;
	}
	errno = 0;
	cwd = u_getcwd();
	if(cwd)
	{
		ft_putendl_fd(cwd, STDOUT_FILENO);
		return EXEC_OK;
	}
	pwd = env_get_value(sh->env_store, "PWD");
	if (pwd)
	{
		ft_putendl_fd(pwd, STDOUT_FILENO);
		return EXEC_OK;
		
	}
	payload.command = "pwd";
	payload.errno_val = errno;
	err_print(ERR_EXEC, EXEC_ERR_GEN, payload);
	return EXEC_ERR_GEN;
}
