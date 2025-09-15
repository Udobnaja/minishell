#ifndef HEREDOC_H
# define HEREDOC_H

# include <readline/readline.h>
# include <readline/history.h>
# include <sys/errno.h>
# include "libft.h"
# include "shell.h"
# include "../expansion/expansion.h"

# define HEREDOC_MAX 16 // MB transfer it to sh 

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

// TODO: internal _header ?

t_heredoc_status	heredoc_write_line(int fd, const char *document, int has_expansion, t_shell *sh);
int					heredoc_write(int fd, const char *buf, size_t len);
char				*heredoc_create_env_key(const char *str);

// TODO: public API
t_heredoc_result	heredoc_write_to_tmpfile(t_shell *sh, char *eof, int has_expansion);

#endif