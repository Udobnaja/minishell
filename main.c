#include "minishell.h"

volatile sig_atomic_t g_last_signal = 0;

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
	sh_termios_restore();
	msh_cleanup(shell);
	exit(exit_status);
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
	if (msh_init(&shell, envp, sh_name) != 0)
		return (SH_GENERAL_ERROR); 
	char *line;
	t_msh_parse_result parse_result;
	t_exec_result exec_result;

	sh_termios_apply();
	rl_catch_signals = 0;
	sh_setup_rl_hook(SH_INTERACTIVE);
	while(1)
	{
		line = readline(msh_get_prompt(argv[0]));
		if (g_last_signal == SIGINT)
		{
			shell.last_status = sh_status_from_signal(SIGINT);
			g_last_signal = 0;
			free(line);
			continue;
		}
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
		ft_bzero(&exec_result, sizeof exec_result);
		parse_result = msh_parse(line, &shell, &pipeline);
		if (parse_result.domain == MPR_OK)
		{
			exec_result = execute(&shell, &pipeline);
			shell.last_status = exec_result.exit_code;
			heredoc_store_clear(shell.heredoc_store);
			if (exec_result.flow == FLOW_EXIT)
			{
				free(line);
				pipeline_destroy(&pipeline);
				msh_clean_and_exit(&shell, shell.last_status);
			}
		}
		else 
			shell.last_status = msh_parse_result_to_exit_status(parse_result);
		pipeline_destroy(&pipeline);
		free(line);
	}
	msh_clean_and_exit(&shell, shell.last_status);
}
