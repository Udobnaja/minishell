#include "minishell.h"

int msh_parse_result_to_exit_status(t_msh_parse_result result)
{
	if (result.domain == MPR_OK)
		return (SH_OK);
	if (result.domain == MPR_LEX)
	{
		if (result.code.lex == LEX_INVALID_SEQ
			|| result.code.lex == LEX_UNMATCHED_QUOTES)
			return (SH_MISUSE);
		return (SH_GENERAL_ERROR);
	}
	if (result.domain == MPR_PARSE)
	{
		if (result.code.parse == PARSE_UNEXPECTED_TOKEN)
			return (SH_MISUSE);
		 return (SH_GENERAL_ERROR); 
	}
	if (result.domain == MPR_HEREDOC)
	{
		// CTRL+D - warning 0
		if (result.code.heredoc == HEREDOC_ABORTED)
			return (130);  // TODO: 128 + signal N
		if (result.code.heredoc == HEREDOC_EXCEED_LIMIT)
			return (SH_MISUSE);
		return (SH_GENERAL_ERROR);
	}
	return (SH_GENERAL_ERROR);
}
