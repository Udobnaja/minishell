#include "parser_internal.h"

int	prs_count_words(char const *s, const char *set)
{
	size_t	i;
	int		count;

	count = 0;
	i = 0;
	while (s[i])
	{
		if (!ft_strchr(set, (unsigned char)s[i]))
		{
			while (s[i] && !ft_strchr(set, (unsigned char)s[i]))
				i++;
			count++;
		}
		else
			i++;
	}
	return (count);
}

int	prs_has_only_spaces(char const *s, const char *set)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		if (!ft_strchr(set, (unsigned char)s[i]))
			return (0);
		i++;
	}
	return (1);
}