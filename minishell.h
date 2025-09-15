#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>

# include "shell.h"
# include "pipeline.h"
# include "executor.h"
# include "errors.h"

# define SHELL_NAME "minishell"

t_env_status	msh_env_init(t_shell *shell, char **envp, const char *sh_name);
t_parser_status	msh_parse(const char *str);
int				msh_env_status_is_fatal(t_env_status status);

#endif
