#ifndef UTILS_H
# define UTILS_H

# include <stddef.h> 
# include <unistd.h>
# include <limits.h>

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

#endif