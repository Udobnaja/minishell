#include "minishell_internal.h"

static int	msh_interactive_space_check(char *line);

void	msh_run_interactive(const char *sh_name, t_shell *sh)
{
	char	*line;

	while (1)
	{
		line = readline(msh_get_prompt(sh_name));
		if (!line)
		{
			write(STDERR_FILENO, "exit\n", 5);
			break ;
		}
		if (msh_signal_check(line, sh))
			continue ;
		if (*line == '\0')
		{
			free(line);
			continue ;
		}
		add_history(line);
		if (msh_interactive_space_check(line))
			continue ;
		msh_run_pipeline(line, sh, 1);
	}
	msh_clean_and_exit(sh, sh->last_status, 1);
}

static int	msh_interactive_space_check(char *line)
{
	if (msh_has_only_spaces(line))
	{
		free(line);
		return (1);
	}
	return (0);
}
