#ifndef SHELL_H
# define SHELL_H

# include "env.h"

typedef struct s_shell
{
	t_env_store	*env_store;
	int			last_status;
	char		*history_path;
}	t_shell;

#endif
