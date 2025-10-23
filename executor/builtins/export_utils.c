#include "executor_internal.h"

void	exec_sort_list(t_env_pair *pairs, size_t size)
{
	size_t		i;
	size_t		j;
	t_env_pair	tmp;

	i = 0;
	while (i < size - 1)
	{
		j = i + 1;
		while (j < size)
		{
			if (ft_strcmp(pairs[i].key, pairs[j].key) > 0)
			{
				tmp = pairs[i];
				pairs[i] = pairs[j];
				pairs[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

void	export_print_list(t_env_pair *pairs, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
	{
		if (pairs[i].key[0] == '_' && (pairs[i].key[1] == '\0'))
		{
			i++;
			continue ;
		}
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(pairs[i].key, STDOUT_FILENO);
		if (pairs[i].value)
		{
			ft_putstr_fd("=\"", STDOUT_FILENO);
			ft_putstr_fd(pairs[i].value, STDOUT_FILENO);
			ft_putendl_fd("\"", STDOUT_FILENO);
		}
		else
			ft_putendl_fd("", STDOUT_FILENO);
		i++;
	}
}

t_exec_status	exec_check_identifier(char *str)
{
	size_t	i;
	char	*key;
	int		is_key_valid;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	key = ft_substr(str, 0, (size_t)(i));
	if (!key)
		return (EXEC_ALLOC_ERR);
	is_key_valid = env_key_is_valid(key);
	free(key);
	if (is_key_valid)
		return (EXEC_OK);
	return (EXEC_INVALID_IDENTIFIER);
}

t_exec_status	export_no_value(t_env_store *store, char *av)
{
	if (env_set(store, av, NULL) != ENV_OK)
		return (EXEC_ALLOC_ERR);
	return (EXEC_OK);
}

t_exec_status	export_set_pairs(t_env_store *store, char *av, char *eqpos)
{
	char			*key;
	char			*value;
	t_exec_status	result;

	key = ft_substr(av, 0, (size_t)(eqpos - av));
	if (!key)
		return (EXEC_ALLOC_ERR);
	value = eqpos + 1;
	if (env_set(store, key, value) != ENV_OK)
		result = EXEC_ALLOC_ERR;
	else
		result = EXEC_OK;
	free(key);
	return (result);
}
