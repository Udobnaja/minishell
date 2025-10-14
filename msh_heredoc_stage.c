#include "minishell_internal.h"

static t_heredoc_status	msh_preparse_heredoc(t_token_list *token_list, t_shell *shell);

int	msh_stage_heredoc(t_shell *shell, t_token_list *token_list, t_msh_parse_result	*status)
{
	const t_heredoc_status	heredoc_status = msh_preparse_heredoc(token_list, shell);
	if (heredoc_status != HEREDOC_OK)
	{
		status->domain = MPR_HEREDOC;
		status->code.heredoc = heredoc_status;
		return (0);
	}
	return (1);
}

static t_heredoc_status	msh_preparse_heredoc(t_token_list *token_list, t_shell *shell)
{
	t_heredoc_status	status;

	status = msh_pre_heredocs(token_list);
	if (status != HEREDOC_OK)
		return (status);
	status = msh_prepare_heredocs(token_list, shell);
	if (status != HEREDOC_OK)
	{
		heredoc_store_clear(shell->heredoc_store);
		return (status);
	}
	return (status);
}