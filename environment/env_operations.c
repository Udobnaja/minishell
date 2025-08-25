#include "env_internal.h"

t_env_status	env_set(t_env_store *store, const char *key, const char *value)
{
    char *d_key;
    char *d_value;

    d_key = ft_strdup(key);
    if (!d_key)
        return (ENV_ALLOC_ERROR);
    if (value)
    {
        d_value = ft_strdup(value);
        if (!d_value)
        {
            free(d_key);
            return (ENV_ALLOC_ERROR);
        }
    }
    else
        d_value = NULL;
         
    return (env_set_internal(store, d_key, d_value));
}

t_env_status	env_set_internal(t_env_store *store, char *key, char *value)
{
    t_env	        *node;

    node = env_lookup(store, key);
    if (node)
    {
        free(key);
        free(node->value);
        node->value = value;
        return (ENV_OK);
    }
    node = env_create_node(key, value);
	if (!node)
	{
		free(key);
		free(value);
		return (ENV_ALLOC_ERROR);
	}
	env_push_node(store, node);
    return (ENV_OK);
}