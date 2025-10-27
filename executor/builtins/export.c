#include "executor_internal.h"

t_exec_status	export_apply(t_env_store *store, char *av)
{
	char				*equal;
	const t_exec_status	key_status = exec_check_identifier(av);

	if (key_status != EXEC_OK)
		return (key_status);
	equal = ft_strchr(av, '=');
	if (equal == NULL)
		return (export_no_value(store, av));
	return (export_set_pairs(store, av, equal));
}

t_exec_status	export_if_no_argv(t_env_store *store)
{
	size_t		size;
	t_env_pair	*pairs;

	size = env_size(store);
	if (size == 0)
		return (EXEC_OK);
	pairs = env_pairs_dup(store);
	if (!pairs)
		return (EXEC_ALLOC_ERR);
	exec_sort_list(pairs, size);
	export_print_list(pairs, size);
	env_pairs_free(pairs, size);
	return (EXEC_OK);
}

bool	exec_is_status_fatal(t_exec_status status)
{
	return (status == EXEC_ALLOC_ERR);
}

t_exec_status	export_process_argv(t_env_store *store, char **av)
{
	size_t			i;
	t_exec_status	tmp_status;
	t_exec_status	status;
	t_err_payload	payload;

	payload = (t_err_payload){0};
	status = EXEC_OK;
	i = 0;
	while (av[i])
	{
		tmp_status = export_apply(store, av[i]);
		if (exec_is_status_fatal(tmp_status))
			return (tmp_status);
		else if (tmp_status != EXEC_OK)
		{
			status = tmp_status;
			payload.command = "export";
			payload.identifier = av[i];
			err_print(ERR_EXEC, status, payload);
		}
		i++;
	}
	return (status);
}

t_exec_status	export(t_shell *sh, t_cmd *cmd)
{
	t_err_payload	payload;
	t_exec_status	status;

	payload = (t_err_payload){0};
	if (!cmd->argv[1])
		return (export_if_no_argv(sh->env_store));
	if (exec_is_invalid_option(cmd->argv[1]))
	{
		status = EXEC_ERR_INVALID_OPTION;
		payload.command = "export";
		err_print(ERR_EXEC, status, payload);
		return (status);
	}
	return (export_process_argv(sh->env_store, &cmd->argv[1]));
}
