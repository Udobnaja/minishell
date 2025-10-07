#include "minishell.h"

const char *msh_get_prompt(char *default_name)
{
	(void) (default_name);
	return ("minishell $ ");
}

int msh_has_only_spaces(char *str)
{
	size_t i;

	i = 0;
	while(str[i])
	{
		if (!ft_isspace(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int main(int argc, char **argv, char **envp)
{
	t_shell shell;
	const char *sh_name;
	t_pipeline pipeline;
	g_last_signal = 0;

	if (argc > 0 && argv && argv[0] && argv[0][0] != '\0')
		sh_name = argv[0];
	else
		sh_name = SHELL_NAME;
	ft_bzero(&shell, sizeof(t_shell));
	if (msh_init(&shell, envp, sh_name) != 0)
		return (SH_GENERAL_ERROR);
	
	char *line;
	t_msh_parse_result parse_result;
	int exit_status;

	while(1)
	{
		line = readline(msh_get_prompt(argv[0]));
		if (!line)    //TODO: EOF / Ctrl-D
			break;
		if (*line == '\0')
		{
			free(line);
			continue;
		}
		add_history(line);
		if (msh_has_only_spaces(line))
		{
			free(line);
			continue;
		}
		ft_bzero(&pipeline, sizeof pipeline);
		parse_result = msh_parse(line, &shell, &pipeline);
		if (parse_result.domain == MPR_OK)
			execute(&shell, &pipeline);
		else 
			shell.last_status = msh_parse_result_to_exit_status(parse_result);
		pipeline_destroy(&pipeline);
		free(line);
	}
	exit_status = shell.last_status;
	heredoc_store_destroy(&shell.heredoc_store);
	env_destroy(&shell.env_store);

	return (exit_status);
}
