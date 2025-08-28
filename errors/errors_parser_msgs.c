#include "errors_internal.h"

const char *err_lexer_msg(int code)
{
	static const char *map[] = {
    	[LEX_UNMATCHED_QUOTES] = "syntax error unexpected EOF while looking for matching `%s'"
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

const char *err_expansion_msg(int code)
{
	static const char *map[] = {
    	[EXP_AMBIGUOUS_REDIRECT] = "ambiguous redirect"
	};
	const int size = sizeof(map) / sizeof(*map);

	if (code < 0 || code >= size)
        return NULL;
	return map[code];
}

const char *err_heredoc_msg(int code)
{
	static const char *map[] = {
    	[HEREDOC_ABORTED] = "", // TODO: Not sure
		[HEREDOC_CREATE_FAIL] = "cannot create temp file for here-document: Too many open files",
		[HEREDOC_EXCEED_LIMIT] = "maximum here-document count exceeded",
	};
	const int size = sizeof(map) / sizeof(*map);

	if (code < 0 || code >= size)
        return NULL;
	return map[code];
}