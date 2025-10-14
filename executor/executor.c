#include "executor_internal.h"

static t_exec_result	exec_builtin_result(t_exec_status status)
{
	t_exec_result  result;

	result.flow = FLOW_OK;
    if (status != EXEC_OK)
        result.exit_code = SH_GENERAL_ERROR;
    else
        result.exit_code = SH_OK;
	result.status = status;
	result.errno_val = 0;
	return (result);
}

static t_exec_status exec_smpl_builtin(t_shell *sh, t_cmd *cmd)
{
    if (cmd->builtin_kind == BUILTIN_ENV)
       return env(sh, cmd);
    if (cmd->builtin_kind == BUILTIN_UNSET)
        return unset(sh, cmd);
    if (cmd->builtin_kind == BUILTIN_EXPORT)
        return export(sh, cmd); 
    if (cmd->builtin_kind == BUILTIN_PWD)
        return pwd(sh, cmd);
    if (cmd->builtin_kind == BUILTIN_ECHO)
        return echo(cmd);
    return cd(sh, cmd);
}

t_exec_result execute_builtin(t_shell *sh, t_cmd *cmd)
{
    t_exec_status status;

    if (cmd->builtin_kind == BUILTIN_EXIT)
        return builtin_exit(sh, cmd);
    status = exec_smpl_builtin(sh, cmd);
    return (
        exec_builtin_result(status)
    );
}

t_exec_result execute(t_shell *sh, t_pipeline *pipeline)
{
    t_exec_result   result;

    ft_bzero(&result, sizeof result);
    if(pipeline->count > 1)
        execute_pipeline(sh, pipeline);
    else if (pipeline->count == 1)
    {
        if (pipeline->cmds[0]->builtin_kind == BUILTIN_NONE)
            result = execute_external(sh, pipeline->cmds[0]);
        else
            result = execute_builtin(sh, pipeline->cmds[0]);
    }
    return (result);
}