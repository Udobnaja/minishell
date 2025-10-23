#include "minishell.h"

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
