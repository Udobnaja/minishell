#include "env_internal.h"

t_env_store *env_store_create(void)
{
	t_env_store *s;

	s = calloc(1, sizeof *s); // TODO: MSH-6
	return s;
}

t_env_status 	env_init(t_env_store *store, char *const envp[])
{
	t_env_status	status;

	if (!store)
		return (ENV_ALLOC_ERROR);

	if (!envp || !envp[0])
		// later
		return (ENV_OK);
	else
	{
		status = env_init_from_envp(store, envp);
		if (status != ENV_OK)
			return (status);
	}
	return (ENV_OK);
}

void	env_clear(t_env_store *store)
{
	t_env	*cur;
	t_env	*next;

	if (!store)
		return;
	cur = store->head;
	while (cur) {
		next = cur->next;
		free(cur->key);
		free(cur->value);
		free(cur);
		cur = next;
	}
	store->head = NULL;
}

void env_destroy(t_env_store **store)
{
	if (!store || !*store) return;
	env_clear(*store);
	free(*store);
	*store = NULL;
}
