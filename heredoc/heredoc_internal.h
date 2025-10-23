#ifndef HEREDOC_INTERNAL_H
# define HEREDOC_INTERNAL_H

# include <stdint.h>
# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <sys/errno.h>
# include "libft.h"
# include "heredoc.h"
# include "../expansion/expansion.h"

# define HEREDOC_STORE_CAPACITY 16

t_heredoc_status	heredoc_write_line(int fd, const char *document,
						int has_expansion, t_shell *sh);
int					heredoc_write(int fd, const char *buf, size_t len);

#endif