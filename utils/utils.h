#ifndef UTILS_H
# define UTILS_H

# include <limits.h>
# include <stddef.h>
# include <sys/stat.h>
# include <unistd.h>

# ifndef U_PATH_MAX
#  ifndef PATH_MAX
#   include <sys/param.h>
#   ifdef MAXPATHLEN
#    define U_PATH_MAX MAXPATHLEN
#   else
#    define U_PATH_MAX 4096
#   endif
#  else
#   define U_PATH_MAX PATH_MAX
#  endif
# endif

const char	*u_getcwd(void);
int			u_file_exists(const char *path);
int			u_file_isdir(const char *path);
int			u_file_iswdir(const char *path);
int			u_file_iswxdir(const char *path);

#endif