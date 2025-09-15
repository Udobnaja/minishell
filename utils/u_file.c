#include "utils.h"

int	u_file_exists(const char *path)
{
	struct stat	st;

	return (path && stat(path, &st) == 0);
}

int	u_file_isdir(const char *path)
{
	struct stat	st;

	return (path && stat(path, &st) == 0 && S_ISDIR(st.st_mode));
}

int	u_file_iswdir(const char *path)
{
    return (u_file_isdir(path) && access(path, W_OK) == 0);
}

int	u_file_iswxdir(const char *path)
{
    return (u_file_isdir(path) && access(path, W_OK | X_OK) == 0);
}
