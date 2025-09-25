#ifndef EXECUTOR_INTERNAL_H
# define EXECUTOR_INTERNAL_H

# include "env.h"
# include "errors.h"
# include "executor.h"
# include "libft.h"
# include "utils.h"
# include <errno.h>
# include <stdbool.h>

// builtins
bool			exec_is_invalid_option(const char *str);
t_exec_status	unset(t_shell *sh, const t_cmd cmd);
t_exec_status	env(t_shell *sh, const t_cmd cmd);
t_exec_status	export(t_shell *sh, const t_cmd cmd);
t_exec_status	pwd(t_shell *sh, t_cmd cmd);
t_exec_status	echo(const t_cmd cmd);
t_exec_status	cd(t_shell *sh, const t_cmd cmd);
t_exec_status	builtin_exit(t_shell *sh, t_cmd cmd);

#endif