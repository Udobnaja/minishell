#include "env_internal.h"

static char *env_str_from_node(t_env *node);

char	**env_to_envp(t_env_store *store)
{
	char 	**envp;
	t_env 	*cur;
	size_t	i;

	envp = ft_calloc(store->size + 1, sizeof(char *));
	if (!envp)
		return (NULL);
	cur = store->head;
	i = 0;
	while (cur)
	{
		if (cur->value)
		{
			envp[i] = env_str_from_node(cur);
			if (!envp[i])
				return (env_envp_free(envp), NULL);
			i++;
		}
		cur = cur->next;
	}
	envp[i] = NULL;
	return (envp);
}

void env_envp_free(char **envp)
{
	size_t	i;

	if (!envp)
		return;
	i = 0;
	while(envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

static char *env_str_from_node(t_env *node)
{
	const size_t	key_len = ft_strlen(node->key);
	const size_t	value_len = ft_strlen(node->value);
	char			*str;

	str = malloc(key_len + value_len + 1 + 1);
	if (!str)
		return (NULL);
	ft_memcpy(str, node->key, key_len);
	str[key_len] = '=';
	ft_memcpy(str + key_len + 1, node->value, value_len);
	str[key_len + 1 + value_len] = '\0';
	return (str);
}
