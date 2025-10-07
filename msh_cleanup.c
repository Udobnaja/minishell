#include "minishell.h"

static void	msh_history_clear(void);

void	msh_cleanup(t_shell *shell)
{
	if (!shell)
		return;
	msh_history_clear();
	if (shell->heredoc_store)
		heredoc_store_destroy(&shell->heredoc_store);
	if (shell->env_store)
		env_destroy(&shell->env_store);
}
#ifdef __APPLE__
static void	msh_history_clear(void)
{
   (void)0; 
}
#else
static void	msh_history_clear(void)
{
    rl_clear_history(); 
}
#endif