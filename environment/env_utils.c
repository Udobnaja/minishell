#include "env_internal.h"

t_env	*env_create_node(char *key, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = key;
	node->value = value;
	node->next = NULL;
	return (node);
}

void	env_push_node(t_env_store *store, t_env	*node)
{
	t_env *cur;

	if (!store->head)
		store->head = node;
	else {
		cur = store->head;
		while (cur->next)
			cur = cur->next;
		cur->next = node;
	}
}

t_env	*env_lookup(t_env_store *store, const char *key)
{
	t_env *cur;

	cur = store->head;
	while (cur)
	{
		if (ft_strcmp(cur->key, key) == 0)
			return (cur);
		cur = cur->next;
	}
    return (NULL);
}

void	env_free_node(t_env	*node)
{
	free(node->key);
	free(node->value);
	free(node);
}