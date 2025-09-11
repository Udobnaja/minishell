#ifndef HEREDOC_H
# define HEREDOC_H

# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"
# include "shell.h"
# include "../expansion/expansion.h"

#define HEREDOC_MAX 16 // MB transfer it to sh 

typedef enum e_heredoc_status {
	HEREDOC_OK = 0,
    HEREDOC_ALLOC_ERROR,
	HEREDOC_ABORTED, // TODO: not sure that it is a error
    HEREDOC_EXCEED_LIMIT,
    HEREDOC_CREATE_FAIL,
    HEREDOC_WRITE_ERROR
}	t_heredoc_status;

#endif