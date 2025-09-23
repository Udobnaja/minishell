#include "parser_internal.h"

int	prs_is_redirect(t_token_type type)
{
	return (
		type == T_HEREDOC
		|| type == T_REDIR_APP
		|| type == T_REDIR_IN
		|| type == T_REDIR_OUT);
}

int prs_arg_is_ambiguous(const char *arg)
{
	size_t i;

	if (!arg || !*arg)
		return (1);
	i = 0;
	while (arg[i])
	{
		if (ft_isspace(arg[i]))
			return (1);
		i++;
	}
	return (0);
}