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