#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "libft.h"
# include "pipeline.h"
# include "env.h"
# include "minishell.h"

typedef enum e_exec_status {
	EXEC_OK = 0,
	EXEC_TOO_MANY_ARGS,
    EXEC_ALLOC_ERROR
}	t_exec_status;

#endif