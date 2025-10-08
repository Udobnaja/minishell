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

static int	msh_rl_event_hook(void)
{
	if (g_last_signal == SIGINT && sh_job(SH_JOB_GET) == SH_INTERACTIVE)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_last_signal = 0;
	} else if (g_last_signal == SIGINT && sh_job(SH_JOB_GET) == SH_HEREDOC)
	{
		sh_job(SH_HEREDOC_ABORTED);
		rl_done = 1;
		g_last_signal = 0;
	}

	return 0;
}

void	msh_rl_install_event_hook(void)
{
	rl_event_hook = msh_rl_event_hook;
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

	rl_catch_signals = 0;
	msh_rl_install_event_hook();
	sh_shell_signals();
	sh_job(SH_INTERACTIVE);
	while(1)
	{
		line = readline(msh_get_prompt(argv[0]));
		if (!line)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break;
		}
		// if (g_last_signal == SIGINT)
		// {
		// 	rl_done = 1;
		// 	g_last_signal = 0;
		// }
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
