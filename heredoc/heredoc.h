#ifndef HEREDOC_H
# define HEREDOC_H

# include "shell.h"

typedef enum e_heredoc_status {
	HEREDOC_OK = 0,
	HEREDOC_ALLOC_ERROR,
	HEREDOC_ABORTED, // TODO: not sure that it is a error
	HEREDOC_EXCEED_LIMIT,
	HEREDOC_CREATE_FAIL,
	HEREDOC_WRITE_ERROR,
	HEREDOC_ERRNO
}	t_heredoc_status;

typedef struct s_heredoc_result
{
	t_heredoc_status	status;
	int	                errno_val;
	int                 fd;
}	t_heredoc_result;

t_heredoc_result	heredoc_write_to_tmpfile(t_shell *sh, char *eof, int has_expansion);
int					heredoc_store_add(t_heredoc_store *store, int fd);

#endif