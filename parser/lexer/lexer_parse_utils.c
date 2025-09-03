#include "lexer_internal.h"

t_lex_parse_result	lex_parse_ok(int consumed)
{
	t_lex_parse_result	result;

	result.status = LEX_OK;
	result.payload.consumed = consumed;
	return (result);
}

t_lex_parse_result	lex_parse_err(t_lex_status e, char invalid_char)
{
	t_lex_parse_result	result;

	result.status = e;
	result.payload.invalid_char = invalid_char;
	return (result);
}
