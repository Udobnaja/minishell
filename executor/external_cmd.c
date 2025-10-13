#include "executor_internal.h"

static int check_candidate(const char *dir, size_t len, const char *name, char out[PATH_MAX])
{
    if(len + 1 + ft_strlen(name) + 1 >= PATH_MAX)
        return 0;
    if(len > 0)
        ft_memcpy(out, dir, len);
    out[len] = '/';
    out[len + 1] = '\0';
    ft_strlcat(out, name, PATH_MAX);
    if(access(out, X_OK) == 0)
        return 1;
    return 0;
}
static int search_in_path(const char *path, const char *name, char out[PATH_MAX])
{
    size_t i;
    size_t start;
    size_t len;

    i = 0;
    start = 0;
    while(1)
    {
        if(path[i] == ':' || path[i] == '\0')
        {
            len = i - start;
            if(check_candidate(path+start, len, name, out) != 0)
                return 1;
            if(path[i] == '\0')
                break;
            start = i + 1;
        }
        i++;
    }
    errno = ENOENT;
    return 0;
}

int cmd_path(t_shell *sh, const char *name, char out[PATH_MAX])
{
    const char *path_env;
    if (name == NULL || out == NULL)
        return 0;
    if(ft_strchr(name, '/') != NULL)
    {
        ft_strlcpy(out, name, PATH_MAX);
        return 1;
    }
    path_env = env_get_value(sh->env_store, "PATH");
    if(path_env == NULL || path_env[0] == '\0' )
    {
        errno = ENOENT;
        return 0;
    }
    if(search_in_path(path_env, name, out) != 0)
        return 1;
    return 0;
}

static int preliminary_check(const char *path, char *argv)
{
    if(!u_file_exists(path))
    {
        err_print(ERR_EXEC, EXEC_NO_SUCH_FILE, 
            (t_err_payload){.command = argv});
        return X_NOTFOUND;
    }
    if (u_file_isdir(path))
    {
        err_print(ERR_EXEC, EXEC_IS_DIRECTORY,
                  (t_err_payload){ .command = argv});
        return X_NOEXEC;
    }
    if (access(path, X_OK) == -1)
    {
        t_err_payload payload;
        payload.errno_val = errno;
        if (errno)
            err_print(ERR_EXEC, EXEC_ERR_GEN, payload);
        else
            err_print(ERR_EXEC, EXEC_ERR_EXECUTION, payload);
        return X_NOEXEC;
    }
    return 0;
}

t_exec_status run_external_cmd(t_shell *sh, t_cmd cmd)
{
    char full[PATH_MAX];
    pid_t pid;
    int status;
    int check_status;
    char **envp;
    
    if(cmd.argv == NULL || cmd.argv[0] == NULL || cmd.argv[0][0] == '\0')
        return EXEC_OK;
    full[0] = '\0';
    if(!cmd_path(sh, cmd.argv[0], full))
    {
        if (ft_strchr(cmd.argv[0], '/') == NULL)
            err_print(ERR_EXEC, EXEC_CMD_NOT_FOUND, 
                        (t_err_payload){ .command = cmd.argv[0] });
        else
            err_print(ERR_EXEC, EXEC_NO_SUCH_FILE, 
                        (t_err_payload){ .command = cmd.argv[0] });
        sh->last_status = 127;
        return EXEC_OK;
    }
    check_status = preliminary_check (full, cmd.argv[0]);
    if(check_status != 0)
    {
        sh->last_status = check_status;
        return EXEC_OK;
    }
    pid = fork();
    if(pid < 0)
    {
        err_print(ERR_EXEC, EXEC_ERR_FORK, (t_err_payload){0});
        return EXEC_ERR_FORK;
    }
    if(pid == 0)
    {
        envp = env_to_envp(sh->env_store);
        if(envp == NULL)
        {
            t_err_payload payload = {0};
            payload.command = cmd.argv[0];
            err_print(ERR_EXEC, EXEC_ERR_EXECUTION, payload);
            exit(1);
        }    
        execve(full, cmd.argv, envp);
        {
            t_err_payload payload = {0};
            payload.command   = cmd.argv[0];
            payload.errno_val = errno;
            if (errno == ENOEXEC) 
            {
                err_print(ERR_EXEC, EXEC_ERR_GEN, payload);
                exit(126);
            }
            err_print(ERR_EXEC, EXEC_ERR_EXECUTION, payload);
            exit(1);
        }
    }
    if(waitpid(pid, &status, 0) == -1)
    {
        sh->last_status = 1;
        return EXEC_ERR_WAIT;
    }
    if(WIFEXITED(status))
        sh->last_status = WEXITSTATUS(status);
    // else if(WIFSIGNALED(status))
    //     sh->last_status = 128 + WTERMSIG(status); //для сигналов
    return EXEC_OK;
}
