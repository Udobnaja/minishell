#include "errors_internal.h"


const char *err_exec_msg(int code) 
{
	static const char *map[] = {
    	[EXEC_TOO_MANY_ARGS] = "too many arguments",
		[EXEC_ALLOC_ERROR] = "allocation error",
		[ERROR_INVALID_OPTION] = "options are not supported"
	};
	const int size = sizeof(map) / sizeof(*map);

	if (code < 0 || code >= size)
        return NULL;
	return map[code];
}