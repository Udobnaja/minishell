#ifndef SHELL_H
# define SHELL_H

# include "env.h"
# include "libft.h"
# include "utils.h"
# include <sys/errno.h>
# include <fcntl.h>

# define SH_TMPDIR_MAX   256
# define SH_TMPPATH_MAX  512

# ifdef O_CLOEXEC
#  define SH_O_CLOEXEC O_CLOEXEC
# else
#  define SH_O_CLOEXEC 0
# endif

static volatile sig_atomic_t g_last_signal;

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

int	sh_get_tmpdir(const t_env_store *env, char *tmpdir, size_t max_name_size);
int	sh_mktmpfd(const t_env_store *env, char *path, const char *prefix);
int sh_status_from_signal(int sig);
int	sh_status_from_errno_exec(int err);
int	sh_status_from_wait(int wst);

#endif
