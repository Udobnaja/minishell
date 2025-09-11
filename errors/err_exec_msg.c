#include "errors_internal.h"


const char *err_exec_msg(int code) 
{
	static const char *map[] = {
    	[EXEC_TOO_MANY_ARGS] = "export: too many arguments",
		[EXEC_ALLOC_ERR] = "export: allocation error",
		[EXEC_ERR_INVALID_OPTION] = "export: options are not supported",
		[EXEC_INVALID_IDENTIFIER] = "export: `%s': not a valid identifier",
		[EXEC_ERR_GEN] = "pwd: options are not supported"
	};
	const int size = sizeof(map) / sizeof(*map);

	if (code < 0 || code >= size)
        return NULL;
	return map[code];
}