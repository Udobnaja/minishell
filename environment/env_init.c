#include "env_internal.h"

static t_env_status 	env_add_row(t_env_store *store, char *row);

t_env_status 	env_empty_init(t_env_store *store)
{
	t_env_status	status;

	status = env_set_current_pwd(store);
	if (status != ENV_OK)
		return (status);
	status = env_set_shlvl(store, "1");
	if (status != ENV_OK)
		return (status);
	return (env_set_start_underscore(store, "minishell"));
}

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
	return (env_normalize(store));
}

static t_env_status 	env_add_row(t_env_store *store, char *row)
{
	char			*key;
	char			*value;
	t_env_status	status;

	key = NULL;
	value = NULL;
	// TODO: MSH-6
	// Include ft_trim from libft
	// " \t\r\n"
	// dont forget to free
	// dont forget to check trimmed_row[0] == '\0'
	// if so frre end status OK
	status = env_parse_row(row, &key, &value);
	if (status != ENV_OK)
		return (status);
	return (env_set_internal(store, key, value));
}