#include "lexer_internal.h"

static int stop_space_or_quote(int c);
static int stop_until_sgl(int c);
static int stop_until_dbl(int c);

int (*lex_stop_resolver(t_quote quote))(int)
{
	if (quote == SGL)
		return stop_until_sgl;
	if (quote == DBL)
		return stop_until_dbl;
	return stop_space_or_quote;
}

static int stop_space_or_quote(int c)
{
    if (c == '\'' || c == '"')
        return (1);
    return (ft_isspace(c));
}

static int stop_until_sgl(int c) 
{
	return (c == '\'');
}

static int stop_until_dbl(int c)
{
	return (c == '"');
}