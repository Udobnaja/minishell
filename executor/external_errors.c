#include "executor_internal.h"

t_exec_result	exec_external_sys_error(t_exec_status status, const char *cmd,
		int errno_val)
{
	t_exec_result	result;
	t_err_payload	payload;

	result.flow = FLOW_OK;
	if (status == EXEC_CMD_NOT_FOUND || status == EXEC_NO_SUCH_FILE)
		result.exit_code = SH_NOT_FOUND;
	else if (status == EXEC_IS_DIRECTORY)
		result.exit_code = SH_NOT_EXECUTABLE;
	else
		result.exit_code = SH_GENERAL_ERROR;
	result.status = status;
	result.errno_val = errno_val;
	payload = (t_err_payload){0};
	if (cmd)
		payload.command = cmd;
	if (errno_val)
		payload.errno_val = errno_val;
	err_print(ERR_EXEC, status, payload);
	return (result);
}

t_exec_result	exec_external_error_result(t_exec_status status,
		const char *cmd, int errno_val)
{
	t_exec_result	result;
	t_err_payload	payload;

	result.flow = FLOW_OK;
	if (status == EXEC_CMD_NOT_FOUND || status == EXEC_NO_SUCH_FILE)
		result.exit_code = SH_NOT_FOUND;
	else if (status == EXEC_IS_DIRECTORY)
		result.exit_code = SH_NOT_EXECUTABLE;
	else if (errno_val)
		result.exit_code = sh_status_from_errno_exec(errno_val);
	else
		result.exit_code = SH_GENERAL_ERROR;
	result.status = status;
	result.errno_val = errno_val;
	payload = (t_err_payload){0};
	if (cmd)
		payload.command = cmd;
	if (errno_val)
		payload.errno_val = errno_val;
	err_print(ERR_EXEC, status, payload);
	return (result);
}

t_exec_result	exec_external_result(t_exec_status status, int exit_code)
{
	t_exec_result	result;

	result.flow = FLOW_OK;
	result.exit_code = (int)(unsigned char)exit_code;
	result.status = status;
	result.errno_val = 0;
	return (result);
}
