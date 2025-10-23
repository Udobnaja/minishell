#include "minishell_internal.h"

t_msh_parse_result	msh_parse(const char *str, t_shell *shell,
		t_pipeline *pipeline)
{
	t_token_list		*token_list;
	t_msh_parse_result	status;

	ft_bzero(&status, sizeof(status));
	if (!msh_stage_init_tokens(&token_list, &status))
		return (status);
	if (!msh_stage_tokenize(str, token_list, &status))
	{
		lex_destroy_token_list(&token_list);
		return (status);
	}
	if (!msh_stage_preparse(token_list, &status))
	{
		lex_destroy_token_list(&token_list);
		return (status);
	}
	if (!msh_stage_heredoc(shell, token_list, &status))
	{
		lex_destroy_token_list(&token_list);
		return (status);
	}
	if (!msh_stage_pipeline(shell, pipeline, token_list, &status))
		heredoc_store_clear(shell->heredoc_store);
	lex_destroy_token_list(&token_list);
	return (status);
}
