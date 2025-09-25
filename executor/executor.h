#ifndef EXECUTOR_H
# define EXECUTOR_H

# include <string.h>
# include <stdio.h>
# include "pipeline.h"
# include "shell.h"

typedef enum e_exec_status
{
	EXEC_OK = 0,
	EXEC_TOO_MANY_ARGS,
	EXEC_ALLOC_ERR,
	EXEC_ERR_INVALID_OPTION,
	EXEC_INVALID_IDENTIFIER,
	EXEC_ERR_GEN,
	EXEC_ERR_CD,
	EXEC_ERR_NUMERIC_ARG
}	t_exec_status;

t_exec_status	mock_exec(t_shell *sh, t_pipeline *pipeline);

#endif