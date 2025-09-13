#include "executor_internal.h"

t_exec_status mock_exec(t_shell *sh, t_pipeline *pipeline)
{
    t_cmd cmd = *pipeline->cmds[0];

    if (cmd.builtin_kind == BUILTIN_ENV)
    {
       return env(sh, cmd);
    } else if (cmd.builtin_kind == BUILTIN_UNSET)
    {
        return unset(sh, cmd);
    } 
    else if (cmd.builtin_kind == BUILTIN_EXPORT)
    {
        return export(sh, cmd);
    } 
    else if (cmd.builtin_kind == BUILTIN_PWD)
    {
        return pwd(sh, cmd);
    } 
    return (EXEC_OK);
}
bool exec_is_invalid_option(const char *str)
{
    if(!str)
        return(false);
    if(str[0] != '-')
        return(true);
    return(false);
}

t_exec_status unset(t_shell *sh, const t_cmd cmd)
{
    size_t i;

    i = 1;
    if (!cmd.argv[1])
        return EXEC_OK;
    if (exec_is_invalid_option(cmd.argv[1]))
    {
        err_print(ERR_EXEC, EXEC_ERR_INVALID_OPTION, (t_err_payload){0});
        return EXEC_ERR_INVALID_OPTION;
    }
    while(cmd.argv[i] != NULL)
    {
        env_unset(sh->env_store, cmd.argv[i]);
        i++;
    }
    return (EXEC_OK);
}
