#include "lexer_internal.h"

static t_lex_parse_result	lex_parse_piece(const char *str, t_piece *piece, t_quote quote, int (*stop)(int c));
static t_quote				lex_quote_type(int c);

t_lex_parse_result lex_parse_word(const char *str, t_token *token)
{
	size_t			i;
	size_t			piece_i;
	t_quote 		quote;
	t_lex_parse_result	result;
	int				(*stop)(int);
	
	i = 0;
	piece_i = 0;
	while (str[i] && !ft_isspace(str[i]))
	{
		quote = lex_quote_type(str[i]);
		stop = lex_stop_resolver(quote);
		if (quote != NONE)
			i++;
		result = lex_parse_piece(str + i, &token->word.pieces[piece_i], quote, stop);
		if (result.status != LEX_OK)
			return (result);
		i += result.payload.consumed;	
		if (quote != NONE)
			i++;
		piece_i++;
	}	
	return (lex_parse_ok(i));
}

static t_lex_parse_result lex_parse_piece(const char *str, t_piece *piece, t_quote quote, int (*stop)(int c))
{
	size_t i;

	i = 0;
	while (str[i] && (!stop(str[i])))
		i++;
	piece->text = malloc(i + 1);
	if (!piece->text)
		return (lex_parse_err(LEX_ALLOC_ERROR, 0));
	ft_memcpy(piece->text, str, i);
	piece->text[i] = '\0';
	piece->quote = quote;
	return (lex_parse_ok(i));
}

static t_quote lex_quote_type(int c)
{
	if (c == '"')
		return (DBL);
	if (c == '\'')
		return (SGL);
	return (NONE);	
}
