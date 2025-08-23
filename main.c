#include "minishell.h"

char *get_prompt(char *default_name)
{
	(void) (default_name);
	return ("minishell$");
}

// TODO: MSH-6
void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*ptr;

	ptr = (unsigned char *)b;
	while (len--)
		*ptr++ = (unsigned char)c;
	return (b);
}
void	ft_bzero(void *s, size_t n)
{
	ft_memset(s, 0, n);
}


int main (int argc, char **argv, char **env)
{
	t_shell shell;

	(void) (argc);
	(void) (argv);

	ft_bzero(&shell, sizeof(t_shell));
	// TODO: MSH-6
	shell.env_store = env_store_create();
	if (!shell.env_store)
		// TODO: MSH-10 Errors Facade
    	return (1);
	if (env_init(shell.env_store, env) != ENV_OK)
	{
		env_destroy(&shell.env_store);
		// TODO: MSH-10 Errors Facade
		return (1);
	}
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