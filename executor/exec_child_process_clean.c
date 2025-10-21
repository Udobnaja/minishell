#include "executor_internal.h"

void	exec_child_process_clean(t_shell *sh, pid_t *pids,  t_pipeline *pl)
{
	if (sh->env_store)
		env_destroy(&sh->env_store);
	if (sh->heredoc_store)
		heredoc_store_destroy(&sh->heredoc_store);
	pipeline_destroy(pl);
	free(pids);
}
void	exec_child_clean(t_shell *sh, t_pipe *p)
{
	if (p)
		exec_child_process_clean(sh, p->pids, p->pl);
	else
		exec_child_process_clean(sh, NULL, NULL);
}