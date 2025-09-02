#include "lexer_internal.h"

static t_lex_result lex_ok();
static t_lex_result lex_err(t_lex_status e, char invalid_char);

t_lex_result lex_tokenize(const char *str, t_token_list	*token_list)
{
	size_t			i;
	t_lex_token_result	token_result;;
	t_token_node	*token_node;

	i = 0;
	while (str[i])
	{
		while (str[i] && ft_isspace(str[i]))
			i++;
		if (str[i] == '>' || str[i] == '<' || str[i] == '|')
		{
			// TODO: create tokens with redir adn pipes + valid
			i++;
		} else {
			token_result = create_word_token(str + i);
			if (token_result.status != LEX_OK)
				 /* TODO: lex_list_destroy(token_list); */
				return (lex_err(token_result.status, token_result.payload.error.invalid_char));
			i += token_result.payload.success.consumed;			
		}
		token_node = lex_create_node(token_result.payload.success.token);
		if (!token_node)
			//     free(token_result.payload.success.token);
			//  /* TODO: lex_list_destroy(token_list); */
			return lex_err(LEX_ALLOC_ERROR, 0);
		lex_push_node(token_list, token_node);	
	}
	return (lex_ok());
}

static t_lex_result lex_ok()
{
    t_lex_result result;

	result.status = LEX_OK;
	result.invalid_char = 0;
	return (result);
}

static t_lex_result lex_err(t_lex_status e, char invalid_char)
{
    t_lex_result result;

	result.status = e;
	result.invalid_char = invalid_char;
	return (result);
}