#include "lexer_internal.h"

static t_lex_status			init_word_token(t_token *token, size_t pieces_count);
static t_lex_token_result	lex_create_word_token(const char *str);
static t_lex_token_result	lex_create_operator_token(const char *str);

t_lex_token_result	lex_create_token(const char *str)
{
	if (lex_is_operator(str[0]))
		return lex_create_operator_token(str);
	else
		return lex_create_word_token(str);
}

static t_lex_token_result	lex_create_word_token(const char *str)
{
	t_lex_parse_result			parse_res;
	t_lex_status				init_status;
	t_token						*token;
	const t_lex_count_result	count_res = lex_count_word_pieces(str);
	
	if (count_res.status != LEX_OK)
		return (lex_token_err(count_res.status, count_res.payload.invalid_char));
	token = ft_calloc(1, sizeof(t_token));
	if (!token)
		return lex_token_err(LEX_ALLOC_ERROR, 0);
	init_status = init_word_token(token, count_res.payload.count);
	if (init_status != LEX_OK)
	{
		lex_free_token(token);
		return lex_token_err(init_status, 0);
	}
	if (count_res.payload.count == 0)
		return (lex_token_ok(token, 0));
	parse_res = lex_parse_word(str, token);
	if (parse_res.status != LEX_OK)
	{
		lex_free_token(token);
		return lex_token_err(parse_res.status, 0);
	}		
	return (lex_token_ok(token, parse_res.payload.consumed));
}

static t_lex_token_result	lex_create_operator_token(const char *str)
{
	t_token				*token;
	t_lex_parse_result	parse_res;

	token = ft_calloc(1, sizeof(t_token));
	if (!token)
		return lex_token_err(LEX_ALLOC_ERROR, 0);
	parse_res = lex_parse_operator(str, token);
	if (parse_res.status != LEX_OK)
	{
		lex_free_token(token);
		return lex_token_err(parse_res.status, parse_res.payload.invalid_char);
	}		
	return (lex_token_ok(token, parse_res.payload.consumed));
}

static t_lex_status	init_word_token(t_token *token, size_t pieces_count)
{
	token->type = T_WORD;
	if (pieces_count == 0)
        return LEX_OK;
	token->word.pieces = ft_calloc(pieces_count, sizeof *token->word.pieces);
	if (!token->word.pieces)
		return (LEX_ALLOC_ERROR);
	token->word.count = pieces_count;
	return (LEX_OK);
}
