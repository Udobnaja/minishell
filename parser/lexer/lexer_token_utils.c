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