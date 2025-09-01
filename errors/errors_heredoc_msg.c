#include "errors_internal.h"

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