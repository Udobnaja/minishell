#include "env_internal.h"

static t_env_status	env_increment_shlvl(t_env_store *store);

t_env_status 	env_normalize(t_env_store *store)
{
	t_env_status	status;

	if (!env_lookup(store, "PWD"))
	{
		status = env_set_current_pwd(store);
		if (status != ENV_OK)
			return (status);
	}
	return (env_increment_shlvl(store));
}

t_env_status 	env_set_current_pwd(t_env_store *store)
{
	t_env_status	status;
	const char		*pwd = u_getcwd();

	if (pwd)
		status = env_set(store, "PWD", pwd);	
	else
		status = ENV_WARN_NO_CWD;
	return (status);
}

t_env_status 	env_set_shlvl(t_env_store *store, const char *lvl)
{
	return (env_set(store, "SHLVL", lvl));
}

t_env_status 	env_set_start_underscore(t_env_store *store, const char *name)
{
	return (env_set(store, "_", name));
}

static t_env_status	env_increment_shlvl(t_env_store *store)
{
	t_env			*node;
	int				error;
	int				shlvl;
	char			*str_shlvl;
	t_env_status	status;

	error = 0;
	node = env_lookup(store, "SHLVL");
	if (!node || !node->value)
		return (env_set_shlvl(store, "1"));
	shlvl = ft_satoi(node->value, &error);
	if (error || shlvl < 0 || shlvl >= SH_SHLVL_MAX)
		return (env_set_shlvl(store, "1"));
	shlvl += 1;
	str_shlvl = ft_itoa(shlvl);
	if (!str_shlvl)
		return (ENV_ALLOC_ERROR);
	status = env_set(store, "SHLVL", str_shlvl);
	free(str_shlvl);
	return (status);
}
