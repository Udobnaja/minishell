#include "env_internal.h"

t_env_store *env_store_create(void)
{
	t_env_store *s;

	s = ft_calloc(1, sizeof *s);
	return s;
}

t_env_status 	env_init(t_env_store *store, char *const envp[], const char *sh_name)
{
	t_env_status	status;

	if (!envp || !envp[0])
		status = env_empty_init(store);
	else
		status = env_init_from_envp(store, envp);
	if (status != ENV_OK)
		return (status);
	if (!env_lookup(store, "OLDPWD"))
	{
		status = env_set(store, "OLDPWD", NULL);
		if (status != ENV_OK)
			return (status);
	}
	return (env_set_start_underscore(store, sh_name));
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
		env_free_node(cur);
		cur = next;
	}
	store->head = NULL;
	store->size = 0;
}

void env_destroy(t_env_store **store)
{
	if (!store || !*store) return;
	env_clear(*store);
	free(*store);
	*store = NULL;
}
