#include "lexer_internal.h"

static t_lex_token_result lex_token_ok(t_token *token, size_t consumed);
static t_lex_token_result lex_token_err(t_lex_status status, char invalid_char);
static t_lex_status	init_word_token(t_token *token, int pieces_count);

t_lex_token_result	create_word_token(const char *str)
{
	t_lex_parse_result			parse_res;
	t_lex_status				init_status;
	t_token						*token;
	const t_lex_count_result	count_res = lex_count_word_pieces(str);
	
	if (count_res.status != LEX_OK)
		return (lex_token_err(count_res.status, count_res.payload.invalid_char));
	token = calloc(1, sizeof(t_token));
	if (!token)
		return lex_token_err(LEX_ALLOC_ERROR, 0);
	init_status = init_word_token(token, count_res.payload.count);
	if (init_status != LEX_OK)
		//     free(token); 
		return lex_token_err(init_status, 0);
	parse_res = lex_parse_word(str, token);
	if (parse_res.status != LEX_OK)
		//     free(token); 
		return lex_token_err(parse_res.status, 0);
	return (lex_token_ok(token, parse_res.payload.consumed));
}

static t_lex_status	init_word_token(t_token *token, int pieces_count)
{
	ft_bzero(token, sizeof(t_token));
	token->type = T_WORD;
	token->word.count = pieces_count;
	token->word.pieces = malloc((pieces_count) * sizeof(t_piece));
	if (!token->word.pieces)
		return (LEX_ALLOC_ERROR);
	return (LEX_OK);
}

static t_lex_token_result lex_token_ok(t_token *token, size_t consumed)
{
	t_lex_token_result result;

	result.status = LEX_OK;
	result.payload.success.token = token;
	result.payload.success.consumed = consumed;
	return (result);
}

static t_lex_token_result lex_token_err(t_lex_status status, char invalid_char)
{
	t_lex_token_result result;

	result.status = status;
	result.payload.error.invalid_char = invalid_char;
	return (result);
}