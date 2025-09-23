#include "parser_internal.h"

t_parser_status prs_word_to_argv(
	const t_word *word, const t_token_node	*prev,
	t_shell *sh, t_cmd *cmd
)
{
	char			*arg;
	t_parser_status	status;

	arg = NULL;
	if (prev && prs_is_simple_redirect(prev->token->type))
	{
		status = prs_is_word_ambiguous(word, sh);
		if (status != PARSE_OK)
			return (status);     
	}
	else if (prev && prev->token->type == T_HEREDOC)
	{
		// TO smth else
		// get from storage

	}

	status = prs_join_word(word, sh, &arg);
	if (status != PARSE_OK)
		return (status);
	if (!pipeline_push_cmd_argv(cmd, arg))
		return (PARSE_ALLOC_ERROR);
	return (PARSE_OK);
}