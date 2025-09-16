#include "heredoc_internal.h"

int heredoc_write(int fd, const char *buf, size_t len)
{
	ssize_t	nw;
	int e;
	errno = 0;

	nw = write(fd, buf, len);
	e = errno;
	if (nw != (ssize_t)len)
	{
		if (e == 0)
			e = ENOSPC;
		return (e);
	}
	return (0);
}