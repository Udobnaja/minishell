#include "executor_internal.h"

void exit_destroy_minishell(t_shell *sh, int status)
{
    if(sh)
    {
        if(sh->env_store)
            env_destroy(&sh->env_store);
        if(sh->heredoc_store)
            heredoc_store_destroy(&sh->heredoc_store);
        if(sh->history_path)
        {
            free(sh->history_path);
            sh->history_path = NULL;
        } 
        rl_clear_history();      
    }
    exit(status);
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

t_exec_status builtin_exit(t_shell *sh, t_cmd cmd)
{
    long long code;
    int				error;
    
    error = 0;
    ft_putendl_fd("exit", STDERR_FILENO);
    if(!cmd.argv[1])
        exit_destroy_minishell(sh, (int)(unsigned char)sh->last_status);
    if(!exec_is_number(cmd.argv[1]))
    {
        err_print(ERR_EXEC, EXEC_ERR_NUMERIC_ARG, (t_err_payload){0});
        exit_destroy_minishell(sh, 2);
    }
    if(cmd.argv[2])
    {
        err_print(ERR_EXEC, EXEC_TOO_MANY_ARGS, (t_err_payload){0});
        return EXEC_TOO_MANY_ARGS;
    }
    code = ft_satoi(cmd.argv[1], &error); //TODO добавить обработку оверфлоу для лонг лонг мах, далее код выхода должен быть 2
    exit_destroy_minishell(sh, (int)(unsigned char)code);
    return EXEC_OK;
}
