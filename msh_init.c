#include "minishell.h"

static int msh_env_status_is_fatal(t_env_status status);
static t_env_status	msh_env_init(t_shell *shell, char **envp, const char *sh_name);

int msh_init(t_shell *shell, char **envp, const char *sh_name)
{
	if (msh_env_status_is_fatal(msh_env_init(shell, envp, sh_name)))
		return (1);

	shell->heredoc_store = heredoc_store_create();
	if (!shell->heredoc_store)
	{
		env_destroy(&shell->env_store);
		err_print(ERR_HEREDOC, HEREDOC_ALLOC_ERROR, (t_err_payload){0});
    	return (1);
	}
	return (0);
}

static t_env_status	msh_env_init(t_shell *shell, char **envp, const char *sh_name)
{
	t_env_status	status;

	shell->env_store = env_store_create();
	if (!shell->env_store)
	{
		err_print(ERR_ENV, ENV_ALLOC_ERROR, (t_err_payload){0});
    	return (ENV_ALLOC_ERROR);
	}
	status = env_init(shell->env_store, envp, sh_name);
	if (msh_env_status_is_fatal(status))
	{
		err_print(ERR_ENV, status, (t_err_payload){0});
		env_destroy(&shell->env_store);
		return (status);
	}
	return (status);
}

static int msh_env_status_is_fatal(t_env_status status)
{
	return (status == ENV_ALLOC_ERROR);
}
