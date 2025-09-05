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
	ERROR_INVALID_OPTION
}	t_exec_status;

#endif