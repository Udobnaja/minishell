#ifndef SHELL_H
# define SHELL_H

# include "env.h"
# include "libft.h"
# include "parser.h"
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

typedef struct s_shell
{
	t_env_store	*env_store;
	int			last_status;
	char		*history_path;
}	t_shell;

int				sh_get_tmpdir(const t_env_store *env, char *tmpdir, size_t max_name_size);
int				sh_mktmpfd(const t_env_store *env, char *path, const char *prefix);

#endif
