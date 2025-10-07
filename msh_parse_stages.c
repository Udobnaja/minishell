#include "minishell_internal.h"

int	msh_stage_pipeline(t_shell *shell, t_pipeline *pipeline, t_token_list *token_list, t_msh_parse_result	*status)
{
	const t_parser_status	parser_status = msh_pipeline(token_list, shell, pipeline);
	if (parser_status != PARSE_OK)
	{
		status->domain = MPR_PARSE;
		status->code.parse = parser_status;
		lex_destroy_token_list(&token_list);
		heredoc_store_clear(shell->heredoc_store);
		return (0);
	}
	return (1);
}