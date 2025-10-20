#include "env_internal.h"

static int	env_pair_from_node(t_env_pair *dst, const t_env *node);

// TODO: before call this checl size bc ft_calloc on 0 size can return NUll and NULL maybe an error
t_env_pair	*env_pairs_dup(const t_env_store *store)
{
	t_env_pair	*pairs;
	t_env		*cur;
	size_t		i;

	pairs = ft_calloc(store->size, sizeof(t_env_pair));
	if (!pairs)
		return (NULL);
	i = 0;
	cur = store->head;
	while (cur)
	{
		if (!env_pair_from_node(&pairs[i], cur))
		{
			env_pairs_free(pairs, i);
			return (NULL);
		}
		i++;
		cur = cur->next;
	}
	return (pairs);
}

void	env_pairs_free(t_env_pair *pairs, const size_t size)
{
	size_t	i;

	if (!pairs)
		return ;
	i = 0;
	while (i < size)
	{
		free(pairs[i].key);
		free(pairs[i].value);
		pairs[i].key = NULL;
		pairs[i].value = NULL;
		i++;
	}
	free(pairs);
}

static int	env_pair_from_node(t_env_pair *dst, const t_env *node)
{
	dst->key = ft_strdup(node->key);
	if (!dst->key)
		return (0);
	if (node->value)
	{
		dst->value = ft_strdup(node->value);
		if (!dst->value)
		{
			free(dst->key);
			dst->key = NULL;
			return (0);
		}
	}
	else
		dst->value = NULL;
	return (1);
}
