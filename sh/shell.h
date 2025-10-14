#ifndef SHELL_H
# define SHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/errno.h>
# include <fcntl.h>
# include <signal.h>
# include <termios.h>

# include "env.h"
# include "libft.h"
# include "utils.h"

# define SH_TMPDIR_MAX   256
# define SH_TMPPATH_MAX  512

# ifdef O_CLOEXEC
#  define SH_O_CLOEXEC O_CLOEXEC
# else
#  define SH_O_CLOEXEC 0
# endif

extern volatile sig_atomic_t g_last_signal;

typedef struct s_heredoc_entry
{
	int	fd;
}	t_heredoc_entry;

typedef struct s_heredoc_store
{
	t_heredoc_entry *entries;
	size_t          count;
	size_t			capacity;
}	t_heredoc_store;

typedef struct s_shell
{
	t_env_store		*env_store;
	int				last_status;
	t_heredoc_store *heredoc_store;
}	t_shell;

typedef enum e_sh_status {
	SH_OK = 0,
	SH_GENERAL_ERROR = 1,
	SH_MISUSE = 2,
	SH_NOT_EXECUTABLE = 126,
	SH_NOT_FOUND = 127
}	t_sh_status;

typedef enum e_sh_job {
	SH_JOB_NONE = 0,
	SH_HEREDOC = 1,
	SH_INTERACTIVE = 2,
	SH_CHILD = 3
}	t_sh_job;

int		sh_get_tmpdir(const t_env_store *env, char *tmpdir, size_t max_name_size);
int		sh_mktmpfd(const t_env_store *env, char *path, const char *prefix);
int 	sh_status_from_signal(int sig);
int		sh_status_from_errno_exec(int err);
int		sh_status_from_wait(int wst);
int		sh_termios_apply(void);
void	sh_termios_restore(void);
void	sh_echoctl_off(struct termios *t);
void	sh_echoctl_on(struct termios *t);

void	sh_shell_signals(void);
void	sh_heredoc_signals(void);
void	sh_childprocess_signals(void);

void	sh_setup_rl_hook(t_sh_job job);

#endif
