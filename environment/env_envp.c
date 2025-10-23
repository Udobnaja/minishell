#include "env_internal.h"

static char		*env_str_from_node(const t_env *node);
static size_t	env_count_entries(const t_env_store *store);

char	**env_to_envp(const t_env_store *store)
{
	char	**envp;
	t_env	*cur;
	size_t	i;

	envp = ft_calloc(env_count_entries(store) + 1, sizeof(char *));
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
				return (env_free_envp(envp), NULL);
			i++;
		}
		cur = cur->next;
	}
	envp[i] = NULL;
	return (envp);
}

void	env_free_envp(char **envp)
{
	size_t	i;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

static char	*env_str_from_node(const t_env *node)
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

static size_t	env_count_entries(const t_env_store *store)
{
	size_t	i;
	t_env	*cur;

	i = 0;
	cur = store->head;
	while (cur)
	{
		if (cur->value)
			i++;
		cur = cur->next;
	}
	return (i);
}
