#include "heredoc.h"

static int heredoc_end_of_key(const char c) {
    if (ft_isspace(c) || c == '\'' || c == '"' || c == '$')
        return (1);
    return !(ft_isalnum(c) || c == '_');
}

char *heredoc_create_env_key(const char *str)
{
	size_t	i;
	char	*key;

	i = 1;
	if (str[i] == '?')
		i++;
	else if (str[i] && (ft_isalpha(str[i]) || str[i] == '_'))
	{
		i++;
		while(str[i] && !heredoc_end_of_key(str[i]))
			i++;
	}	
	key = malloc(i + 1);
	if (!key)
		return NULL;
	ft_memcpy(key, str, i);
	key[i] = '\0';
	return (key);
}