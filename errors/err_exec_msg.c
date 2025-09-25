#include "errors_internal.h"


const char *err_exec_msg(int code) 
{
	static const char *map[] = {
    	[EXEC_TOO_MANY_ARGS] = "%s: too many arguments",
		[EXEC_ALLOC_ERR] = "Cannot allocate memory",
		[EXEC_ERR_INVALID_OPTION] = "%s: options are not supported",
		[EXEC_INVALID_IDENTIFIER] = "%s: `%s': not a valid identifier",
		[EXEC_ERR_GEN] = "%s: %s",
		[EXEC_ERR_CD] = "cd: HOME is not set",
		[EXEC_ERR_NUMERIC_ARG] = "%s: numeric argument required"
	};
	const int size = sizeof(map) / sizeof(*map);

	if (code < 0 || code >= size)
        return NULL;
	return map[code];
}