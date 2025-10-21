#include "executor_internal.h"

static int				exec_is_number(const char *str);
static t_exec_result	exec_exit_result(t_exec_flow flow, t_exec_status status, int exit_code);
static t_exec_result	exec_quit(t_exec_status status, int exit_code);
static t_exec_result	exec_continue(t_exec_status status, int exit_code);

t_exec_result builtin_exit(t_shell *sh, t_cmd *cmd)
{
	long long code;
	int				error;
	
	error = 0;
	if(!cmd->argv[1])
		return (exec_quit(EXEC_OK, sh->last_status));
	if(!exec_is_number(cmd->argv[1]))
		return (exec_quit(EXEC_ERR_NUMERIC_ARG, SH_MISUSE));
	if(cmd->argv[2])
		return exec_continue(EXEC_TOO_MANY_ARGS, SH_GENERAL_ERROR);
	code = ft_satoll(cmd->argv[1], &error);
	if (error)
		return exec_quit(EXEC_ERR_NUMERIC_ARG, SH_MISUSE);
	return (exec_quit(EXEC_OK, code));
}

static int exec_is_number(const char *str)
{
	int i;

	if(!str || !*str)
		return 0;
	i = 0;
	if(str[i] == '+' || str[i] == '-')
		i++;
	if(!str[i])
		return 0;
	while(str[i])
	{
		if(!ft_isdigit((unsigned char)str[i]))
			return 0;
		i++;
	}
	return 1;
}

static t_exec_result	exec_exit_result(t_exec_flow flow, t_exec_status status, int exit_code)
{
	t_exec_result  result;

	result.flow = flow;
	result.exit_code = (int)(unsigned char)exit_code;
	result.status = status;
	result.errno_val = 0;
	return (result);
}

static t_exec_result	exec_quit(t_exec_status status, int exit_code)
{
	t_exec_result	result;
	t_err_payload	payload;

	ft_bzero(&payload, sizeof payload);
	result = exec_exit_result(FLOW_EXIT, status, exit_code);
	if (status != EXEC_OK)
	{
		payload.command = "exit";
		err_print(ERR_EXEC, status, payload);
	}
	return result;
}

static t_exec_result	exec_continue(t_exec_status status, int exit_code)
{
	t_exec_result	result;
	t_err_payload	payload;

	ft_bzero(&payload, sizeof payload);
	result = exec_exit_result(FLOW_OK, status, exit_code);
	if (status != EXEC_OK)
	{
		payload.command = "exit";
		err_print(ERR_EXEC, status, payload);
	}
	return result;
}
