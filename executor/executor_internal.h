#ifndef EXECUTOR_INTERNAL_H
# define EXECUTOR_INTERNAL_H

# include "env.h"
# include "errors.h"
# include "executor.h"
# include "heredoc.h"
# include "libft.h"
# include "utils.h"
# include <errno.h>
# include <stdbool.h>

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
void			exec_sort_list(t_env_pair *pairs, size_t size);
void			export_print_list(t_env_pair *pairs, size_t size);
t_exec_status	exec_check_identifier(char *str);
t_exec_status	export_no_value(t_env_store *store, char *av);
t_exec_status	export_set_pairs(t_env_store *store, char *av, char *eqpos);

t_exec_result	execute_external(t_shell *sh, t_pipeline *pipeline);
t_exec_result	execute_builtin(t_shell *sh, t_cmd *cmd);
t_exec_result	execute_pipeline(t_shell *sh, t_pipeline *pl);
t_exec_result	apply_redirections(t_cmd *cmd);
int				cmd_path(t_shell *sh, const char *name, char out[PATH_MAX]);
t_exec_result	exec_external_error_result(t_exec_status status,
					const char *cmd, int errno_val);
t_exec_result	exec_external_result(t_exec_status status, int exit_code);
t_exec_result	exec_external_sys_error(t_exec_status status, const char *cmd,
					int errno_val);
void			exec_child(const char *full, t_cmd *cmd, t_shell *sh,
					t_pipe *p);
void			exec_one_child(const char *full, t_cmd *cmd, t_shell *sh,
					t_pipeline *pl);

void			close_fd(int *fd);
void			exec_child_process_clean(t_shell *sh, pid_t *pids,
					t_pipeline *pl);
void			close_pair(int p[2]);
void			pipe_parent_rotate(t_pipe *p);
void			pipe_close(t_pipe *p);
t_exec_result	exec_pipe_apply_stdio(t_pipe *p, int std_in, int std_out);
t_exec_result	child_process(t_pipe *p, size_t i);
void			child_setup(t_pipe *p, size_t i);
void			run_child_process(t_pipe *p, size_t i);
t_exec_result	exec_make_pipe(t_pipe *p);
t_exec_result	exec_fork_child(t_pipe *p, size_t i);
pid_t			find_last_pid(const pid_t *pids, size_t n);
int				cmd_is_empty(const t_cmd *cmd);
void			exec_run_buildin(t_shell *sh, t_cmd *cmd, pid_t *pids,
					t_pipeline *pl);
void			exec_cmd_path(t_pipe *p, t_cmd *cmd, char path[PATH_MAX]);
t_exec_result	dup2_end_close(int fd, int target, const char *name);

t_exec_status	redir_status_from_errno(int err);
int				save_descriptors(int fd[3]);
int				restore_descriptors(int fd[3]);
void			close_descriptors(int fd[3]);
t_exec_result	exec_builtin_with_redirs(t_shell *sh, t_cmd *cmd);

#endif