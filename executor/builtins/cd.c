#include "executor_internal.h"

static t_exec_status cd_print_error_and_exit(t_exec_status status, char *cmd, int errno_val)
{
	t_err_payload payload;
	
	payload = (t_err_payload){0};
	if (cmd)
		payload.command = cmd;
	if (errno_val)
		payload.errno_val = errno_val;
	err_print(ERR_EXEC, status, payload);
	return (status);
}

static t_exec_status cd_go_path(t_shell *sh, const char *target)
{
	const char *oldpwd;
	const char *newpwd;
	
	oldpwd = u_getcwd();
	if(chdir(target) != 0)
		return (
			cd_print_error_and_exit(EXEC_ERR_GEN, "cd", errno)
		);
	if(oldpwd && env_set(sh->env_store, "OLDPWD", oldpwd) != ENV_OK)
		return (
			cd_print_error_and_exit(EXEC_ALLOC_ERR, NULL, 0)
		);
	errno = 0;
	newpwd = u_getcwd();
	if(!newpwd)
		return (
			cd_print_error_and_exit(EXEC_ERR_GEN, "cd", errno)
		);
	if(env_set(sh->env_store, "PWD", newpwd) != ENV_OK)
		return (
			cd_print_error_and_exit(EXEC_ALLOC_ERR, NULL, 0)
		);
	return EXEC_OK;
}

static t_exec_status cd_go_home(t_shell *sh)
{
	const char *home = env_get_value(sh->env_store, "HOME");
	
	if(!home)
		return (
			cd_print_error_and_exit(EXEC_ERR_CD, NULL, 0)
		);
	return cd_go_path(sh, home);
}

t_exec_status cd(t_shell *sh, t_cmd *cmd)
{
	
	if(!cmd->argv[1])
		return cd_go_home(sh);
	if (exec_is_invalid_option(cmd->argv[1]))
		return (
			cd_print_error_and_exit(EXEC_ERR_INVALID_OPTION, cmd->argv[0], 0)
		);
	if(cmd->argv[2])
		return (
			cd_print_error_and_exit(EXEC_TOO_MANY_ARGS, cmd->argv[0], 0)
		);
	return cd_go_path(sh, cmd->argv[1]);
}
