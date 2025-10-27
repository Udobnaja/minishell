#include "minishell_internal.h"

void	msh_run_noninteractive(t_shell *sh)
{
	char	*line;

	while (1)
	{
		line = readline(NULL);
		if (!line)
			break ;
		if (msh_signal_check(line, sh))
			continue ;
		if (*line == '\0')
		{
			free(line);
			continue ;
		}
		if (msh_has_only_spaces(line))
		{
			free(line);
			continue ;
		}
		msh_run_pipeline(line, sh, 0);
	}
	msh_clean_and_exit(sh, sh->last_status, 0);
}
