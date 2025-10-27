#include "minishell_internal.h"

void	msh_cleanup(t_shell *shell)
{
	if (!shell)
		return ;
	rl_clear_history();
	if (shell->heredoc_store)
		heredoc_store_destroy(&shell->heredoc_store);
	if (shell->env_store)
		env_destroy(&shell->env_store);
}

void	msh_clean_and_exit(t_shell *shell, int exit_status, int is_interactive)
{
	if (is_interactive)
		sh_termios_restore();
	msh_cleanup(shell);
	exit(exit_status);
}
