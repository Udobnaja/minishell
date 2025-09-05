#include "../executor.h"

static bool validate_unset(const char *str)
{
    if(!str || str[0] != '-')
        return(false);
    if(str[1] =='\0')
        return(true);
    if(str[1] != '-')
        return(true);
    if(str[1] == '-' && str[2] == '\0')
        return(false);
    return(true);
}
int env_unset(t_env_store *store, const char *name)
{
    t_env *cur;
    t_env *prev;

    if(!store || !name)
        return -1;
    prev = NULL;
    cur  = store->head;
    while(cur)
    {
        if(ft_strcmp(cur->key, name) == 0)
        {
            if(prev)
                prev->next = cur->next;
            else 
                store->head = cur->next;
            env_free_node(cur);
            if(store->size > 0)
                store->size--;
            return 0;
        }
        prev = cur;
        cur = cur->next;
    }
    return 0;
}

t_exec_status unset(t_shell *sh, const t_cmd cmd)
{
    size_t i;

    i = 1;
    if (!cmd.argv[1])
        return EXEC_OK;
    if (validate_unset(cmd.argv[1]))
    {
        err_print(ERR_EXEC, ERROR_INVALID_OPTION, (t_err_payload){0});
        return ERROR_INVALID_OPTION;
    }
    while(cmd.argv[i] != NULL)
    {
        env_unset(sh->env_store, cmd.argv[i]);
        i++;
    }
    return (EXEC_OK);
}
