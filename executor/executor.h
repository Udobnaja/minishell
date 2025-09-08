#ifndef EXECUTOR_H
# define EXECUTOR_H

#include <stdbool.h>
# include "libft.h"
# include "pipeline.h"
# include "env.h"
# include "shell.h"
# include "errors.h"

typedef enum e_exec_status {
	EXEC_OK = 0,
	EXEC_TOO_MANY_ARGS,
    EXEC_ALLOC_ERROR,
	EXEC_ERROR_INVALID_OPTION,
	EXEC_INVALID_IDENTIFIER
}	t_exec_status;

//builtins
bool exec_check_errors(const char *str);
t_exec_status unset(t_shell *sh, const t_cmd cmd);
t_exec_status env(t_shell *sh, const t_cmd cmd);
t_exec_status mock_exec(t_shell *sh, t_pipeline *pipeline);
t_exec_status export(t_shell *sh, const t_cmd cmd);


#endif