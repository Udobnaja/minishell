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

void export_print_list(t_env_pair *pairs, size_t size)
{
    size_t i;

    exec_sort_list(pairs, size);
    i = 0;
    while(i < size)
    {
        if(!(pairs[i].key[0] == '_' &&
            (pairs[i].key[1] == '\0' || pairs[i].key[1] == '=')))
        {
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
        }
        i++;
    }
}

int exec_check_identifier(char *str)
{
    size_t i;

    if(!str || !(ft_isalpha(str[0]) || str[0] == '_'))
    {
        err_print(ERR_EXEC, EXEC_INVALID_IDENTIFIER, (t_err_payload){0});
        return EXEC_INVALID_IDENTIFIER;
    }
    i = 1;
    while(str[i] && str[i] != '=')
    {
        if(!(ft_isalnum(str[i]) || str[i] == '_'))
        {
            err_print(ERR_EXEC, EXEC_INVALID_IDENTIFIER, (t_err_payload){0});
            return EXEC_INVALID_IDENTIFIER;
        }
        i++;
    }
    return EXEC_OK;
}

int export_no_value(t_env_store *store, char *av)
{
    if(env_set(store, av, NULL) != ENV_OK)
        return EXEC_ALLOC_ERROR;
    return EXEC_OK;
}

int export_set_pairs(t_env_store *store, char *av, char *eqpos)
{
    char *key;
    char *value;

    key = ft_substr(av, 0, (size_t)(eqpos - av));
    if(!key)
        return EXEC_ALLOC_ERROR;
    value = ft_strdup(eqpos + 1);
    if(!value)
    {
        free(key);
        return EXEC_ALLOC_ERROR;
    }
    if(env_set(store, key, value) != ENV_OK)
    {
        free(key);
        free(value);
        return EXEC_ALLOC_ERROR;
    }
    free(key);
    free(value);
    return EXEC_OK;
}

int exec_apply_export(t_env_store *store, char *av)
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
    export_print_list(pairs, size);
    env_pairs_free(pairs, size);
    return EXEC_OK;

}

t_exec_status export(t_shell *sh, const t_cmd cmd)
{
    size_t i;

    if (!cmd.argv[1])
        return export_if_no_av(sh->env_store);
    if (exec_check_errors(cmd.argv[1]))
    {
        err_print(ERR_EXEC, EXEC_ERROR_INVALID_OPTION, (t_err_payload){0});
        return EXEC_ERROR_INVALID_OPTION;
    }
    i = 1;
    while(cmd.argv[i])
    {
        if(exec_apply_export(sh->env_store, cmd.argv[i]) != EXEC_OK)
            return EXEC_ALLOC_ERROR;
        i++;
    }
    return EXEC_OK;
}

