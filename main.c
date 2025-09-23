#include "minishell.h"

const char *get_prompt(char *default_name)
{
	(void) (default_name);
	return ("minishell $ ");
}

int main(int argc, char **argv, char **envp)
{
	t_shell shell;
	const char *sh_name;
	t_pipeline pipeline;

	if (argc > 0 && argv && argv[0] && argv[0][0] != '\0')
		sh_name = argv[0];
	else
		sh_name = SHELL_NAME;
	ft_bzero(&shell, sizeof(t_shell));
	if (msh_init(&shell, envp, sh_name))
		return (1);
	
	char *line;

	while(1)
	{
		line = readline(get_prompt(argv[0]));
		if (!line)
			break;
		if (*line)
		{
			add_history(line);
			ft_bzero(&pipeline, sizeof pipeline); 
			if (msh_parse(line, &shell, &pipeline) == PARSE_OK)
				mock_exec(&shell, &pipeline);
			pipeline_destroy(&pipeline);
		}		
		free(line);
		line = NULL;
	}
	heredoc_store_destroy(&shell.heredoc_store);
	env_destroy(&shell.env_store);
	return (0);
}
