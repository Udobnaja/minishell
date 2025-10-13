#include "executor_internal.h"

t_exec_status execute_builtin(t_shell *sh, t_cmd *cmd)
{

    if (cmd->builtin_kind == BUILTIN_ENV)
       return env(sh, *cmd);
    if (cmd->builtin_kind == BUILTIN_UNSET)
        return unset(sh, *cmd);
    if (cmd->builtin_kind == BUILTIN_EXPORT)
        return export(sh, *cmd); 
    if (cmd->builtin_kind == BUILTIN_PWD)
        return pwd(sh, *cmd);
    if (cmd->builtin_kind == BUILTIN_ECHO)
        return echo(*cmd);
    if (cmd->builtin_kind == BUILTIN_CD)
        return cd(sh, *cmd);
    if (cmd->builtin_kind == BUILTIN_EXIT)
        return builtin_exit(sh, *cmd);
    return (EXEC_NOT_BUILTIN);
}

t_exec_status execute_external(t_shell *sh, t_cmd *cmd)
{
    return (
        run_external_cmd(sh, *cmd)
    );

}


t_exec_status execute(t_shell *sh, t_pipeline *pipeline)
{
    t_exec_status   status;

    if(pipeline->count > 1)
        execute_pipeline(sh, pipeline);
    else if (pipeline->count == 1)
    {
        if (pipeline->cmds[0]->builtin_kind == BUILTIN_NONE)
        {
            status = execute_external(sh, pipeline->cmds[0]);
            if (status != EXEC_OK)
                return (status);
        } else {
            status = execute_builtin(sh, pipeline->cmds[0]);
            if (status != EXEC_OK)
                return (status);
        }
    }
    return (EXEC_OK);
}