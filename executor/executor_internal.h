#ifndef EXECUTOR_INTERNAL_H
# define EXECUTOR_INTERNAL_H

# include <errno.h>
# include <stdbool.h>

# include "env.h"
# include "errors.h"
# include "executor.h"
# include "heredoc.h"
# include "libft.h"
# include "utils.h"


# ifndef FD_READ
#  define FD_READ 0
#  define FD_WRITE 1
# endif

// builtins
bool			exec_is_invalid_option(const char *str);
t_exec_status	unset(t_shell *sh, t_cmd *cmd);
t_exec_status	env(t_shell *sh, t_cmd *cmd);
t_exec_status	export(t_shell *sh, t_cmd *cmd);
t_exec_status	pwd(t_shell *sh, t_cmd *cmd);
t_exec_status	echo(t_cmd *cmd);
t_exec_status	cd(t_shell *sh, t_cmd *cmd);
t_exec_result	builtin_exit(t_shell *sh, t_cmd *cmd);

t_exec_result	execute_external(t_shell *sh, t_cmd *cmd);
t_exec_result	execute_builtin(t_shell *sh, t_cmd *cmd);
t_exec_result	execute_pipeline(t_shell *sh, t_pipeline *pl);
t_exec_result	apply_redirections(t_cmd *cmd);
int				cmd_path(t_shell *sh, const char *name, char out[PATH_MAX]);
t_exec_result	exec_external_error_result(t_exec_status status,
					const char *cmd, int errno_val);
t_exec_result	exec_external_result(t_exec_status status, int exit_code);
t_exec_result	exec_external_sys_error(t_exec_status status, const char *cmd,
					int errno_val);
void			exec_child(const char *full, t_cmd *cmd, t_shell *sh);

void			close_fd(int *fd);

#endif