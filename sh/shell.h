#ifndef SHELL_H
# define SHELL_H

# include "env.h"
# include "libft.h"
# include "parser.h"
# include "utils.h"
# include <sys/errno.h>

typedef struct s_shell
{
	t_env_store	*env_store;
	int			last_status;
	char		*history_path;
}	t_shell;

int				sh_get_tmpdir(const t_env_store *env, char *tmpdir, size_t max_name_size);

// TODO: probably not sh layer — miinishel layer
t_env_status	sh_env_init(t_shell *shell, char **envp, const char *sh_name);
t_parser_status	sh_parse(const char *str);
int				sh_env_status_is_fatal(t_env_status status);

#endif
