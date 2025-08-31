#include "errors_internal.h"

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