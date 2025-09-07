#include "lexer_internal.h"

static int	stop_until_sgl(int c);
static int	stop_until_dbl(int c);

int	(*lex_stop_resolver(t_quote quote))(int c)
{
	if (quote == SGL)
		return (stop_until_sgl);
	if (quote == DBL)
		return (stop_until_dbl);
	return (lex_stop_until_new_piece);
}

int	lex_stop_until_new_piece(int c)
{
	if (c == '\'' || c == '"')
		return (1);
	if (lex_is_operator(c))
		return (1);
	return (ft_isspace(c));
}

static int	stop_until_sgl(int c)
{
	return (c == '\'');
}

static int	stop_until_dbl(int c)
{
	return (c == '"');
}
