#include "executor_internal.h"

bool exec_is_invalid_option(const char *str)
{
    if(!str)
        return(false);
    if(str[0] == '-')
        return(true);
    return(false);
}
t_exec_status unset(t_shell *sh, t_cmd *cmd)
{
    size_t i;
    t_err_payload payload;

	payload = (t_err_payload){0};
    i = 1;
    if (!cmd->argv[1])
        return EXEC_OK;
    if (exec_is_invalid_option(cmd->argv[1]))
    {
        payload.command = cmd->argv[0];
        err_print(ERR_EXEC, EXEC_ERR_INVALID_OPTION, payload);
        return EXEC_ERR_INVALID_OPTION;
    }
    while(cmd->argv[i] != NULL)
    {
        env_unset(sh->env_store, cmd->argv[i]);
        i++;
    }
    return (EXEC_OK);
}
