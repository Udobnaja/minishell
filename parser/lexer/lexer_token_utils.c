#include "lexer_internal.h"

void	lex_free_token(t_token	*token)
{
	size_t	i;

	if (token->type == T_WORD)
	{
		i = 0;
		while (i < token->word.count)
		{
			free(token->word.pieces[i].text);
			token->word.pieces[i].text = NULL;
			i++;
		}
		free(token->word.pieces);
		token->word.pieces = NULL;
		token->word.count = 0;
	}
	free(token);
}

int	lex_is_operator(unsigned char c)
{
    return c == '>' || c == '<' || c == '|';
}

t_lex_token_result lex_token_ok(t_token *token, size_t consumed)
{
	t_lex_token_result result;

	result.status = LEX_OK;
	result.payload.success.token = token;
	result.payload.success.consumed = consumed;
	return (result);
}

t_lex_token_result lex_token_err(t_lex_status status, char invalid_char)
{
	t_lex_token_result result;

	result.status = status;
	result.payload.error.invalid_char = invalid_char;
	return (result);
}