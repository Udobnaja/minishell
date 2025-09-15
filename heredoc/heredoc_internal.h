#ifndef HEREDOC_INTERNAL_H
# define HEREDOC_INTERNAL_H

# include <readline/readline.h>
# include <readline/history.h>
# include <sys/errno.h>
# include "libft.h"
# include "../expansion/expansion.h"

# include "heredoc.h"

t_heredoc_status	heredoc_write_line(int fd, const char *document, int has_expansion, t_shell *sh);
int					heredoc_write(int fd, const char *buf, size_t len);
char				*heredoc_create_env_key(const char *str);

#endif