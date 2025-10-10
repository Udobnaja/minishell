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

static int preliminary_check(const char *path, const char *argv)
{
    if(u_file_exists(path))
    {
        err_print(ERR_EXEC, EXEC_NO_SUCH_FILE, 
            (t_err_payload){.identifier = (char *)argv});
        return X_NOTFOUND;;
    }
    if (u_file_isdir(path))
    {
        if (ft_strchr(argv, '/') == NULL)
        {
            err_print(ERR_EXEC, EXEC_NO_SUCH_FILE,
                      (t_err_payload){ .command = (char*)argv });
            return X_NOTFOUND;
        }
        err_print(ERR_EXEC, EXEC_NO_SUCH_FILE,
                  (t_err_payload){ .command = (char*)argv});
        return X_NOEXEC;
    }

    if (access(path, X_OK) == -1)
    {
        if (errno == EACCES)
        {
            err_print(ERR_EXEC, EXEC_ERR_PERMISSION,
                      (t_err_payload){ .command = (char*)argv});
                return X_NOEXEC;
        }
        else
        {
            err_print(ERR_EXEC, EXEC_ERR_GEN,
                      (t_err_payload){ .command = (char*)argv});
                return X_NOEXEC;
        }

    }
    return 0;

}

t_exec_status run_external_cmd(t_shell *sh, t_cmd cmd)
{
    char full[PATH_MAX];
    pid_t pid;
    int status;
    int temp_err_msg;
    char **envp;

    if(cmd.argv == NULL || cmd.argv[0] == NULL || cmd.argv[0][0] == '\0')
        return EXEC_OK;
    full[0] = '\0';
    if(!cmd_path(sh, cmd.argv[0], full))
    {
        err_print(ERR_EXEC, EXEC_CMD_NOT_FOUND, (t_err_payload){.command = cmd.argv[0]});
        sh->last_status = 127;
        return EXEC_CMD_NOT_FOUND;
    }
    temp_err_msg = preliminary_check (full, cmd.argv[0]);
    if(temp_err_msg != 0)
    {
        sh->last_status = temp_err_msg; // будет отдельный сервис для ласт статус
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
            exit(1); // проверить на лики
        execve(full, cmd.argv, envp);
        if(errno == ENOEXEC)
        {
            err_print(ERR_EXEC, EXEC_ERR_NOT_EXEC, (t_err_payload){0});
            return EXEC_ERR_NOT_EXEC; 
        }
        exit(1);
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