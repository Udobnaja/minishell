#include "minishell.h"

static t_env_status	sh_env_init(t_shell *shell, char **envp, const char *sh_name);

const char *get_prompt(char *default_name)
{
	(void) (default_name);
	return ("minishell $ ");
}

int main (int argc, char **argv, char **envp)
{
	t_shell shell;
	const char *sh_name;

	if (argc > 0 && argv && argv[0] && argv[0][0] != '\0')
		sh_name = argv[0];
	else
		sh_name = SHELL_NAME;
	ft_bzero(&shell, sizeof(t_shell));
	if (sh_env_init(&shell, envp, sh_name) != ENV_OK)
		return (1);
	
	char *line;

	while(1)
	{
		line = readline(get_prompt(argv[0]));
		if (!line)
			break;
		if (*line)
			add_history(line);
		free(line);
		line = NULL;
	}
	env_destroy(&shell.env_store);
	return (0);
}

static t_env_status	sh_env_init(t_shell *shell, char **envp, const char *sh_name)
{
	t_error			error;
	t_env_status	status;

	shell->env_store = env_store_create();
	if (!shell->env_store)
	{
		status = ENV_ALLOC_ERROR;
		error = err_create(ERR_ENV, status);
		err_print(&error);
    	return (status);
	}
	status = env_init(shell->env_store, envp, sh_name);
	if (status != ENV_OK)
	{
		env_destroy(&shell->env_store);
		error = err_create(ERR_ENV, status);
		err_print(&error);
		return (status);
	}
	return (status);
}