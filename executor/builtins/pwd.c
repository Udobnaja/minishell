#include "executor_internal.h"

t_exec_status pwd(t_shell *sh, t_cmd cmd)
{
	const char *cwd;
	const char *pwd;

	if(exec_is_invalid_option(cmd.argv[1]))
	{
		err_print(ERR_EXEC, EXEC_ERROR_INVALID_OPTION, (t_err_payload){0});
		return EXEC_ERROR_INVALID_OPTION;
	}
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
	err_print(ERR_EXEC, EXEC_ERR_PWD, (t_err_payload){0});  //should I put "errno" in payload? 
	return EXEC_ERR_PWD;
}
