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
	EXEC_ERR_WAIT,
	EXEC_ERR_NOT_EXEC,
	EXEC_ERR_DUP,
	EXEC_PERMISSION_DENIED,
	EXEC_ERR_PIPE,
	EXEC_IS_DIRECTORY,
	EXEC_EXECUTION_ERROR
}				t_exec_status;


typedef struct s_pipe
{
	t_shell *sh;
	t_pipeline *pl;
	pid_t *pids;
	size_t n;
	int prev[2];
	int next[2];
}t_pipe;

t_exec_status execute(t_shell *sh, t_pipeline *pipeline);

#endif