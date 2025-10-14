#include "parser_internal.h"

t_parser_status prs_word_to_argv(
	const t_word *word,
	t_shell *sh, t_cmd *cmd
)
{
	char			*arg;
	t_parser_status	status;

	arg = NULL;
	status = prs_join_word(word, sh, &arg);
	if (status != PARSE_OK)
		return (status);
	if (!pipeline_push_cmd_argv(cmd, arg))
		return (PARSE_ALLOC_ERROR);
	return (PARSE_OK);
}