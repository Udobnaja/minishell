#include "../executor.h"

void exec_sort_list(t_env_pair *pairs ,size_t size)
{
    size_t i;
    size_t j;
    t_env_pair tmp;

    i = 0;
    while(i < size - 1)
    {
        j = i + 1;
        while(j < size)
        {
            if(ft_strcmp(pairs[i].key, pairs[j].key) > 0)
            {
                tmp = pairs[i];
                pairs[i] = pairs[j];
                pairs[j] = tmp;
            }
            j++;
        }
        i++;
    }

}

void exec_export_print_list(t_env_pair *pairs, size_t size)
{
    size_t i;

    i = 0;
    while(i < size)
    {
        if(pairs[i].key[0] == '_' && (pairs[i].key[1] == '\0'))
        {
            i++;
            continue;
        }
        ft_putstr_fd("declare -x ", STDOUT_FILENO);
        ft_putstr_fd(pairs[i].key, STDOUT_FILENO);
        if(pairs[i].value)
        {
            ft_putstr_fd("=\"", STDOUT_FILENO);
            ft_putstr_fd(pairs[i].value, STDOUT_FILENO);
            ft_putendl_fd("\"", STDOUT_FILENO);
        }
        else 
            ft_putendl_fd("", STDOUT_FILENO);
        i++;
    }
}

t_exec_status exec_check_identifier(char *str)
{
    size_t i;

    if(!str || !(ft_isalpha(str[0]) || str[0] == '_'))
    {
        ft_putstr_fd("minishell: export: `", STDERR_FILENO);
        ft_putstr_fd(str, STDERR_FILENO);
        ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
        return EXEC_INVALID_IDENTIFIER;
    }
    i = 1;
    while(str[i] && str[i] != '=')
    {
        if(!(ft_isalnum(str[i]) || str[i] == '_'))
        {
            ft_putstr_fd("minishell: export: `", STDERR_FILENO);
            ft_putstr_fd(str, STDERR_FILENO);
            ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
            return EXEC_INVALID_IDENTIFIER;
        }
        i++;
    }
    return EXEC_OK;
}

t_exec_status export_no_value(t_env_store *store, char *av)
{
    if(env_set(store, av, NULL) != ENV_OK)
        return EXEC_ALLOC_ERROR;
    return EXEC_OK;
}

t_exec_status export_set_pairs(t_env_store *store, char *av, char *eqpos)
{
    char *key;
    char *value;
    t_exec_status result;

    key = ft_substr(av, 0, (size_t)(eqpos - av));
    if(!key)
        return EXEC_ALLOC_ERROR;
    value = eqpos + 1;
    if(env_set(store, key, value) != ENV_OK)
        result = EXEC_ALLOC_ERROR;
    else    
        result = EXEC_OK;  
    free(key);
    return result;
}

t_exec_status exec_apply_export(t_env_store *store, char *av)
{
    char *equal; 

    if(exec_check_identifier(av) != EXEC_OK)
        return EXEC_INVALID_IDENTIFIER;
    equal = ft_strchr(av, '=');
    if(equal == NULL)
        return export_no_value(store, av);
    return export_set_pairs(store, av, equal);
    
}

t_exec_status export_if_no_av(t_env_store *store)
{
    size_t size;
    t_env_pair *pairs;

    size = env_size(store);
    if (size == 0)
        return EXEC_OK;
    pairs = env_pairs_dup(store);
    if (!pairs)
        return EXEC_ALLOC_ERROR;
    exec_sort_list(pairs, size);
    exec_export_print_list(pairs, size);
    env_pairs_free(pairs, size);
    return EXEC_OK;
}

t_exec_status export(t_shell *sh, const t_cmd cmd)
{
    size_t i;
    t_exec_status status;
    t_exec_status overall;

    if (!cmd.argv[1])
    return export_if_no_av(sh->env_store);
    i = 1;
    overall = EXEC_OK;
    while(cmd.argv[i])
    {
        if (exec_is_invalid_option(cmd.argv[i]))
        {
            err_print(ERR_EXEC, EXEC_ERROR_INVALID_OPTION, (t_err_payload){0});
            return EXEC_ERROR_INVALID_OPTION;
        }
        status = exec_apply_export(sh->env_store, cmd.argv[i]);
        if(status == EXEC_ALLOC_ERROR)
            return EXEC_ALLOC_ERROR;
        if(status == EXEC_INVALID_IDENTIFIER)
             overall = EXEC_INVALID_IDENTIFIER;
        i++;
    }
    return overall;
}

