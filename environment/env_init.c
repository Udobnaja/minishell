#include "env_internal.h"

static t_env_status 	env_add_row(t_env_store *store, char *row);

t_env_status 	env_init_from_envp(t_env_store *store, char *const envp[])
{
	int				i;
	t_env_status	status;

	i = 0;
	while (envp[i])
	{
		status = env_add_row(store, envp[i]);
		if (status != ENV_OK)
			return (status);
		i++;
	}
	return (ENV_OK);
}

static t_env_status 	env_add_row(t_env_store *store, char *row)
{
	char			*key;
	char			*value;
	t_env			*node;
	t_env_status	status;

	key = NULL;
	value = NULL;
	status = env_parse_row(row, &key, &value);
	if (status != ENV_OK)
		return (status);	
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