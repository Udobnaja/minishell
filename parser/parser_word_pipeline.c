#include "parser_internal.h"

t_parser_status prs_word_to_argv(
	const t_word *word, const t_token_node	*prev,
	t_shell *sh, t_cmd *cmd
)
{
	char			*arg;
	t_parser_status	status;

	arg = NULL;
	status = prs_join_word(word, sh, &arg);
	if (status != PARSE_OK)
		return (status);
	if (prev && prs_is_simple_redirect(prev->token->type))
    {
        if (prs_arg_is_ambiguous(arg)) // TODO: wrong
        {
            free(arg);
            return (PARSE_AMBIGUOUS_REDIRECT);
        }      
    }
    else if (prev && prev->token->type == T_HEREDOC)
    {
        // TO smth else
    }
	if (!pipeline_push_cmd_argv(cmd, arg))
		return (PARSE_ALLOC_ERROR);
	return (PARSE_OK);
}