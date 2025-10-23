#include "errors_internal.h"

const char	*err_env_msg(int code)
{
	static const char	*map[] = {[ENV_ALLOC_ERROR] = "Cannot allocate memory"};
	const int			size = sizeof(map) / sizeof(*map);

	if (code < 0 || code >= size)
		return (NULL);
	return (map[code]);
}
