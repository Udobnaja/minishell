#include "env_internal.h"

int	env_key_is_valid(const char *key)
{
	size_t	i;

	if (ft_isalpha(key[0]) || key[0] == '_')
	{
		i = 1;
		while (key[i])
		{
			if (!(ft_isalnum(key[i]) || key[i] == '_'))
				return (0);
			i++;
		}
		return (1);
	}
	return (0);
}
