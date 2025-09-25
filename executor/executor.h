#ifndef EXECUTOR_H
# define EXECUTOR_H

#include <sys/wait.h>

# include "minishell.h"
# include "pipeline.h"
# include "shell.h"

#define X_NOTFOUND 127
#define X_NOEXEC 126

typedef enum e_exec_status
{
	EXEC_OK = 0,
	EXEC_TOO_MANY_ARGS,
	EXEC_ALLOC_ERR,
	EXEC_ERR_INVALID_OPTION,
	EXEC_INVALID_IDENTIFIER,
	EXEC_ERR_GEN,
	EXEC_ERR_CD,
	EXEC_ERR_NUMERIC_ARG,
	EXEC_NOT_BUILTIN,
	EXEC_NO_SUCH_FILE,
	EXEC_CMD_NOT_FOUND,
	EXEC_ERR_PERMISSION,
	EXEC_ERR_FORK,
	EXEC_ERR_WAIT
}				t_exec_status;

t_exec_status execute(t_shell *sh, t_pipeline *pipeline);

#endif