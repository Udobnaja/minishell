#include "env_internal.h"

// TODO: MSH-6
char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	substr_len;
	size_t	s_len;
	char	*alloc_memory;

	if (!s)
		return (NULL);
	s_len = strlen(s);
	if (start >= s_len)
		return (strdup(""));
	substr_len = strlen(s + start);
	if (substr_len > len)
		substr_len = len;
	alloc_memory = malloc((substr_len + 1) * sizeof(char));
	if (!alloc_memory)
		return (NULL);
	strlcpy(alloc_memory, s + start, substr_len + 1);
	return (alloc_memory);
}

t_env_status	env_parse_row(const char *row, char **key, char **value)
{
	char	*substr;

	substr = strchr(row, '=');
	if (!substr)
	{
		*key = strdup(row);
		if (!*key)
			return (ENV_ALLOC_ERROR);
		*value = NULL;
	}
	else
	{
		*key = ft_substr(row, 0, substr - row);
		if (!*key)
			return (ENV_ALLOC_ERROR);
		*value = strdup(substr + 1);
		if (!*value) {
			free(*key);
			return (ENV_ALLOC_ERROR);
		}
	}
	return (ENV_OK);
}