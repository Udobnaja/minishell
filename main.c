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

static void msh_clean_and_exit(t_shell *shell,int exit_status)
{
	msh_cleanup(shell);
	exit(exit_status);
}

int main(int argc, char **argv, char **envp)
{
	t_shell shell;
	const char *sh_name;
	t_pipeline pipeline;

	sh_termios_apply();

	if (argc > 0 && argv && argv[0] && argv[0][0] != '\0')
		sh_name = argv[0];
	else
		sh_name = SHELL_NAME;
	ft_bzero(&shell, sizeof(t_shell));
	if (msh_init(&shell, envp, sh_name) != 0)
		return (SH_GENERAL_ERROR); 
	char *line;
	t_msh_parse_result parse_result;
	while(1)
	{
		sh_shell_signals();
		line = readline(msh_get_prompt(argv[0]));
		if (!line)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break;
		}
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
	sh_termios_restore();
	msh_clean_and_exit(&shell, shell.last_status);
}
