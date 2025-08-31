#include "../executor.h"

t_exec_status env(t_shell *sh, const t_cmd cmd)
{
    char **envp;
    size_t i;
    if(cmd.argv[1] != NULL) // проверить ли ещё !cmd.argv ?
    {
        ft_putstr_fd("minishell:env:too many arguments\n", STDERR_FILENO);
        return EXEC_TOO_MANY_ARGS;
    }
    envp = env_to_envp(sh->env_store);
    if(!envp)
    {
        ft_putstr_fd("minishell:env:allocation error\n", STDERR_FILENO);
        return EXEC_ALLOC_ERROR;
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