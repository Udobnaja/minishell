#include "executor_internal.h"

t_exec_status env(t_shell *sh, const t_cmd cmd)
{
    char **envp;
    size_t i;
    if(cmd.argv[1] != NULL) 
    {
        err_print(ERR_EXEC, EXEC_TOO_MANY_ARGS, (t_err_payload){0}); // TODO: move lvl up
        return EXEC_TOO_MANY_ARGS;
    }
    envp = env_to_envp(sh->env_store);
    if(!envp)
    {
        err_print(ERR_EXEC, EXEC_ALLOC_ERR, (t_err_payload){0}); // TODO: move lvl up
        return EXEC_ALLOC_ERR;
    }
    i = 0;
    while(envp[i])
    {
        ft_putendl_fd(envp[i], STDOUT_FILENO);
        i++;
    }
    env_free_envp(envp);
    return EXEC_OK;
}