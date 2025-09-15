#include "errors_internal.h"

const char *err_lexer_msg(int code)
{
	static const char *map[] = {
    	[LEX_UNMATCHED_QUOTES] = "syntax error unexpected EOF while looking for matching `%s'",
		[LEX_ALLOC_ERROR] = "Cannot allocate memory",
		[LEX_INVALID_SEQ] = "syntax error near unexpected token `%s'",
		[LEX_INTERNAL_ERROR] = "internal error"
	};
	const int size = sizeof(map) / sizeof(*map);

	if (code < 0 || code >= size)
        return NULL;
	return map[code];
}

const char *err_parser_msg(int code)
{
	static const char *map[] = {
    	[PARSE_UNEXPECTED_TOKEN] = "syntax error near unexpected token `%s'"
	};
	const int size = sizeof(map) / sizeof(*map);

	if (code < 0 || code >= size)
        return NULL;
	return map[code];
}
