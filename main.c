#include "minishell.h"

volatile sig_atomic_t	g_last_signal = 0;

const char	*msh_get_prompt(const char *name)
{
	static char		buf[PROMPT_MAX];
	const size_t	max_name = PROMPT_MAX - 4;
	const char		*slash = ft_strrchr(name, '/');
	const char		*base = name;
	size_t			n;

	if (slash != NULL)
		base = slash + 1;
	n = ft_strlen(base);
	if (n > max_name)
		n = max_name;
	ft_memcpy(buf, base, n);
	ft_memcpy(buf + n, " $ ", 4);
	return (buf);
}

int	msh_has_only_spaces(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isspace(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static void	msh_clean_and_exit(t_shell *shell, int exit_status,
		int is_interactive)
{
	sh_termios_restore();
	msh_cleanup(shell);
	exit(exit_status);
}

char	*msh_get_name(int argc, char **argv)
{
	if (argc > 0 && argv && argv[0] && argv[0][0] != '\0')
		return (argv[0]);
	else
		return (SHELL_NAME);
}

void	msh_run_pipeline(char *line, t_shell *sh)
{
	t_pipeline			pipeline;
	t_msh_parse_result	parse_result;
	t_exec_result		exec_result;

	ft_bzero(&pipeline, sizeof pipeline);
	ft_bzero(&exec_result, sizeof exec_result);
	parse_result = msh_parse(line, sh, &pipeline);
	if (parse_result.domain == MPR_OK)
	{
		exec_result = execute(sh, &pipeline);
		sh->last_status = exec_result.exit_code;
		heredoc_store_clear(sh->heredoc_store);
		if (exec_result.flow == FLOW_EXIT)
		{
			free(line);
			pipeline_destroy(&pipeline);
			msh_clean_and_exit(sh, sh->last_status, 1);
		}
	}
	else
		sh->last_status = msh_parse_result_to_exit_status(parse_result);
	pipeline_destroy(&pipeline);
	free(line);
}

int	msh_interactive_signal_check(char *line, t_shell *sh)
{
	if (g_last_signal == SIGINT)
	{
		sh->last_status = sh_status_from_signal(SIGINT);
		g_last_signal = 0;
		free(line);
		return (1);
	}
	return (0);
}

int msh_interactive_space_check(char *line)
{
	if (msh_has_only_spaces(line))
	{
		free(line);
		return (1);
	}
	return (0);
}

void	msh_run_interactive(const char *sh_name, t_shell *sh)
{
	char	*line;

	while (1)
	{
		line = readline(msh_get_prompt(sh_name));
		if (msh_interactive_signal_check(line, sh))
			continue ;
		if (!line)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break ;
		}
		if (*line == '\0')
		{
			free(line);
			continue ;
		}
		add_history(line);
		if (msh_interactive_space_check(line))
			continue ;
		msh_run_pipeline(line, sh);
	}
	msh_clean_and_exit(sh, sh->last_status, 1);
}

void	msh_run_noninteractive(t_shell *sh)
{
	char	*line;

	rl_outstream = stdin; // TEST ONLY ??
	while (1)
	{
		line = get_next_line(fileno(stdin)); // readline();
		if (!line)
			break ;
		if (*line == '\0')
		{
			free(line);
			break ;
		}
		if (msh_has_only_spaces(line))
		{
			free(line);
			continue ;
		}
		msh_run_pipeline(line, sh);
	}
	msh_clean_and_exit(sh, sh->last_status, 0);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell		shell;
	const char	*sh_name = msh_get_name(argc, argv);

	ft_bzero(&shell, sizeof(t_shell));
	if (msh_init(&shell, envp, sh_name) != 0)
		return (SH_GENERAL_ERROR);
	rl_catch_signals = 0;
	sh_setup_rl_hook(SH_JOB_RW);
	if (isatty(STDIN_FILENO))
		msh_run_interactive(sh_name, &shell);
	else
		msh_run_noninteractive(&shell);
}
