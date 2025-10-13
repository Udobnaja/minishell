#include "errors_internal.h"


const char *err_exec_msg(int code) 
{
	static const char *map[] = {
    	[EXEC_TOO_MANY_ARGS] = "%s: too many arguments",
		[EXEC_ALLOC_ERR] = "cannot allocate memory",
		[EXEC_ERR_INVALID_OPTION] = "%s: options are not supported",
		[EXEC_INVALID_IDENTIFIER] = "%s: `%s': not a valid identifier",
		[EXEC_ERR_GEN] = "%s: %s",
		[EXEC_ERR_CD] = "cd: HOME is not set",
		[EXEC_ERR_NUMERIC_ARG] = "%s: numeric argument required",
		[EXEC_ERR_WAIT] = "waitpid failed",
		[EXEC_ERR_DUP] = "dup2: Bad file descriptor",
		[EXEC_ERR_PIPE] = "pipe: Resource temporarily unavailable", 
		[EXEC_ERR_FORK] = "fork: Resource temporarily unavailable",
		[EXEC_NO_SUCH_FILE] = "%s: No such file or directory",
		[EXEC_CMD_NOT_FOUND] = "%s: command not found",
		[EXEC_PERMISSION_DENIED] = "%s: Permission denied",		
		[EXEC_IS_DIRECTORY] = "%s: Is a directory",	
		[EXEC_ERR_EXECUTION] = "Internal execution error"
	};
	const int size = sizeof(map) / sizeof(*map);

	if (code < 0 || code >= size)
        return NULL;
	return map[code];
}