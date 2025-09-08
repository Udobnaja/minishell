#include "utils.h"

const char	*u_getcwd(void)
{
	static char buf[PATH_MAX];

	if (!getcwd(buf, sizeof buf))
		return (NULL);
	return (buf);
}