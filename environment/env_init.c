#include "env_internal.h"

static t_env_status	env_add_row(t_env_store *store, const char *row);

t_env_status	env_empty_init(t_env_store *store)
{
	t_env_status	status;

	status = env_set_current_pwd(store);
	if (status != ENV_OK)
		return (status);
	return (env_set_shlvl(store, "1"));
}

t_env_status	env_init_from_envp(t_env_store *store, char *const envp[])
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
	return (env_normalize(store));
}

static t_env_status	env_add_row(t_env_store *store, const char *row)
{
	char			*key;
	char			*value;
	t_env_status	status;
	char			*trimmed_row;

	key = NULL;
	value = NULL;
	trimmed_row = ft_strtrim(row, "\t\r\n");
	if (!trimmed_row)
		return (ENV_ALLOC_ERROR);
	if (trimmed_row[0] == '\0')
	{
		free(trimmed_row);
		return (ENV_OK);
	}
	status = env_parse_row(trimmed_row, &key, &value);
	free(trimmed_row);
	if (status != ENV_OK)
		return (status);
	return (env_set_internal(store, key, value));
}
