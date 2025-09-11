#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "pipeline.h"
# include "shell.h"

typedef enum e_exec_status {
	EXEC_OK = 0,
	EXEC_TOO_MANY_ARGS,
    EXEC_ALLOC_ERR,
	EXEC_ERR_INVALID_OPTION,
	EXEC_INVALID_IDENTIFIER,
	EXEC_ERR_GEN
}	t_exec_status;

t_exec_status mock_exec(t_shell *sh, t_pipeline *pipeline);


#endif