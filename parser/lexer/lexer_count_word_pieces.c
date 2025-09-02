#include "lexer_internal.h"

static t_lex_count_result lex_count_error(t_lex_status e, char invalid_char);
static t_lex_count_result lex_count_ok(int count);

t_lex_count_result lex_count_word_pieces(const char *str)
{
	int		count;
	size_t	i;
	char	q;

	count = 0;
	i = 0;
	while (str[i] && !ft_isspace(str[i]))
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			q = str[i];
			i++;
			while (str[i] && str[i] != q)
				i++;
			if (str[i] == '\0')
				return (lex_count_error(LEX_UNMATCHED_QUOTES, q));
			i++;
		} else
			while (str[i] && (!ft_isspace(str[i]) && str[i] != '\'' && str[i] != '"'))
				i++;
		count++;
	}		
	return (lex_count_ok(count));	
}

static t_lex_count_result lex_count_error(t_lex_status e, char invalid_char)
{
	t_lex_count_result result;
	
	result.status = e;
	result.payload.invalid_char = invalid_char;
	return (result);
}

static t_lex_count_result lex_count_ok(int count)
{
	t_lex_count_result result;
	
	result.status = LEX_OK;
	result.payload.count = count;
	return (result);
}