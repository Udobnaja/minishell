#include "parser_internal.h"

int	prs_is_redirect(t_token_type type)
{
	return (
		type == T_HEREDOC
		|| type == T_REDIR_APP
		|| type == T_REDIR_IN
		|| type == T_REDIR_OUT);
}