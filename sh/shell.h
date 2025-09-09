#ifndef SHELL_H
# define SHELL_H

# include "env.h"
# include "libft.h"
# include "errors.h"
# include "parser.h"

typedef struct s_shell
{
	t_env_store	*env_store;
	int			last_status;
	char		*history_path;
}	t_shell;

t_env_status	sh_env_init(t_shell *shell, char **envp, const char *sh_name);
t_parser_status	sh_parse(const char *str);

#endif
