#include "env_internal.h"

t_env_status	env_parse_row(const char *row, char **key, char **value)
{
	char	*substr;

	substr = ft_strchr(row, '=');
	if (!substr)
	{
		*key = ft_strdup(row);
		if (!*key)
			return (ENV_ALLOC_ERROR);
		*value = NULL;
	}
	else
	{
		*key = ft_substr(row, 0, substr - row);
		if (!*key)
			return (ENV_ALLOC_ERROR);
		*value = ft_strdup(substr + 1);
		if (!*value)
		{
			free(*key);
			return (ENV_ALLOC_ERROR);
		}
	}
	return (ENV_OK);
}
