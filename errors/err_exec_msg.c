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
		[EXEC_NOT_BUILTIN] = "",
		[EXEC_ERR_WAIT] = "waitpid failed",
		[EXEC_ERR_DUP] = "dup2: Bad file descriptor",
		[EXEC_ERR_PIPE] = "fork: Resource temporarily unavailable", // if pipe return -1
		[EXEC_ERR_FORK] = "fork: Resource temporarily unavailable", // if fork return -1
		[EXEC_NO_SUCH_FILE] = "%s: No such file or directory", 		// exit code 127
		[EXEC_CMD_NOT_FOUND] = "%s: command not found", 			// 127
		[EXEC_PERMISSION_DENIED] = "%s: Permission denied",			// 126
		[EXEC_IS_DIRECTORY] = "%s: Is a directory",					// 126
		[EXEC_ERR_EXECUTION] = "%s: execution error"				// 126
	};
	const int size = sizeof(map) / sizeof(*map);

	if (code < 0 || code >= size)
        return NULL;
	return map[code];
}