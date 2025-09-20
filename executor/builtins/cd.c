#include "executor_internal.h"

t_exec_status cd_go_path(t_shell *sh, const char *target)
{
    const char *oldpwd;
    const char *newpwd;
    t_err_payload payload;
    
    payload = (t_err_payload){0};
    oldpwd = u_getcwd();
    if(chdir(target) != 0)
    {
        payload.errno_val = errno;
        err_print(ERR_EXEC, EXEC_ERR_GEN, payload);
        return EXEC_ERR_GEN;
    }
    newpwd = u_getcwd();
    if(!newpwd)
    {
        payload.errno_val = errno;
        err_print(ERR_EXEC, EXEC_ERR_GEN, payload);
        return EXEC_ERR_GEN;
    }
    if(oldpwd && env_set(sh->env_store, "OLDPWD", oldpwd) != ENV_OK)
    {
        err_print(ERR_EXEC, EXEC_ERR_GEN, (t_err_payload){0});
        return EXEC_ERR_GEN;
    }
    if(env_set(sh->env_store, "PWD", newpwd) != ENV_OK)
    {
        err_print(ERR_EXEC, EXEC_ERR_GEN, (t_err_payload){0});
        return EXEC_ERR_GEN;
    }
    return EXEC_OK;
}

t_exec_status cd_go_home(t_shell *sh)
{
    const char *home;

    home = env_get_value(sh->env_store, "HOME");
    if(!home)
    {
        err_print(ERR_EXEC, EXEC_ERR_CD, (t_err_payload){0});
        return EXEC_ERR_CD;
    }
    return cd_go_path(sh, home);
}

t_exec_status cd(t_shell *sh, const t_cmd cmd)
{
    t_exec_status status;

    if(!cmd.argv[1])
        return cd_go_home(sh);
    if (exec_is_invalid_option(cmd.argv[1]))
    {
        status = EXEC_ERR_INVALID_OPTION;
        err_print(ERR_EXEC, status, (t_err_payload){0});
        return status;
    }
    if(cmd.argv[2])
    {
        err_print(ERR_EXEC, EXEC_TOO_MANY_ARGS, (t_err_payload){0});
        return EXEC_TOO_MANY_ARGS;
    }
    return cd_go_path(sh, cmd.argv[1]);
}
