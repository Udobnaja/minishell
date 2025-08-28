#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>

# include "libft/libft.h"
# include "errors/errors.h"
# include "environment/env.h"
# include "parser/parser.h"

# define SHELL_NAME "minishell"

typedef struct s_shell
{
	t_env_store	*env_store;
	int			last_status;
	char		*history_path;
}	t_shell;

#endif
