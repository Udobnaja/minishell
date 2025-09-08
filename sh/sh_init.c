#include "shell.h"

t_env_status	sh_env_init(t_shell *shell, char **envp, const char *sh_name)
{
	t_env_status	status;

	shell->env_store = env_store_create();
	if (!shell->env_store)
	{
		status = ENV_ALLOC_ERROR;
		err_print(ERR_ENV, status, (t_err_payload){0});
    	return (status);
	}
	status = env_init(shell->env_store, envp, sh_name);
	if (status != ENV_OK)
	{
		env_destroy(&shell->env_store);
		err_print(ERR_ENV, status, (t_err_payload){0});
		return (status);
	}
	return (status);
}

