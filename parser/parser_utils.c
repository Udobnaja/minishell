#include "parser_internal.h"

int	prs_is_simple_redirect(t_token_type type)
{
	return (
		type == T_REDIR_APP
		|| type == T_REDIR_IN
		|| type == T_REDIR_OUT);
}

int	prs_is_redirect(t_token_type type)
{
	return (
		type == T_HEREDOC
		|| prs_is_simple_redirect(type));
}

static int prs_has_ambiguous_separator(const char *s)
{
	size_t i = 0;

	if (!s) return 0;
	while (s[i])
	{
		if (ft_strchr(" \t\n", (unsigned char)s[i]))
			return 1;
		i++;
	}
	return 0;
}

int prs_arg_is_ambiguous(const char *arg)
{
	if (!arg || !*arg)
		return (1);
	return (prs_has_ambiguous_separator(arg));
}