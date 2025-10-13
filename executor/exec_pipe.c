#include "executor_internal.h"

void close_fd(int *fd)
{
    if(*fd != -1)
    {
        close(*fd);
        *fd = -1;
    }
}
void close_pair(int p[2])
{
    close_fd(&p[FD_READ]);
    close_fd(&p[FD_WRITE]);
}
void pipe_child_close(t_pipe *p)
{
    close_pair(p->prev);
    close_pair(p->next);
}

void pipe_parent_rotate(t_pipe *p)
{
    close_pair(p->prev);
    p->prev[FD_READ] = p->next[FD_READ];
    p->prev[FD_WRITE] = p->next[FD_WRITE];
    p->next[FD_READ] = -1;
    p->next[FD_WRITE] = -1;
    close_fd(&p->prev[FD_WRITE]);
}

void pipe_close(t_pipe *p)
{
    close_pair(p->next);
    close_pair(p->prev);
}

t_exec_status child_process(t_pipe *p, size_t i)
{
    if (i > 0)
    {
        if(p->prev[FD_READ] == -1)
            return EXEC_ERR_DUP;
        if(dup2(p->prev[FD_READ], STDIN_FILENO) == -1)
            return EXEC_ERR_DUP;
    }
    if (i + 1 < p->n)
    {
        if(p->next[FD_WRITE] == -1)
            return EXEC_ERR_DUP;
        if(dup2(p->next[FD_WRITE], STDOUT_FILENO) == -1)
            return EXEC_ERR_DUP;
    }
    pipe_child_close(p);
    return EXEC_OK;
}

t_exec_status wait_all(pid_t *pids, size_t n, int *last_status_out)
{
    size_t i;
    int last;
    int st;

    if(last_status_out == NULL)
        return EXEC_ALLOC_ERR;
    last = 0;
    i = 0;
    while(i < n)
    {
        if(pids[i] != 0)
        {
            st = 0;
            if(waitpid(pids[i], &st, 0) > 0)
            {
                if (WIFEXITED(st) != 0)
                last = WEXITSTATUS(st);
            // else if (WIFSIGNALED(st) != 0)
            //     last = 128 + WTERMSIG(st); 
            }
        }
        i++;
    }
    *last_status_out = last;
    return EXEC_OK;
}

void exec_handle_error1(const char *path)
{
    t_err_payload pay;
    int code;

    pay.identifier = (char *)path;
    if (errno == ENOENT)
    {
        err_print(ERR_EXEC, EXEC_NO_SUCH_FILE, pay);
        code = 127;
    }
    else if (errno == EACCES)
    {
        err_print(ERR_EXEC, EXEC_PERMISSION_DENIED, pay);
        code = 126;
    }
    else if (errno == EISDIR)
    {
        err_print(ERR_EXEC, EXEC_IS_DIRECTORY, pay);
        code = 126;
    }
    exit(code);
}

void exec_handle_error2(const char *path)
{
    t_err_payload pay;
    int code;

    pay.identifier = (char *)path;
    if (errno == ENOEXEC)
    {
        err_print(ERR_EXEC, EXEC_ERR_NOT_EXEC, pay);
        code = 126;
    }
    else
    {
        err_print(ERR_EXEC, EXEC_ERR_EXECUTION, pay);
        code = 126;
    }
    exit(code);
}
static void child_setup(t_pipe *p, size_t i)
{
    if (child_process(p, i) != EXEC_OK)
    {
        err_print(ERR_EXEC, EXEC_ERR_DUP, (t_err_payload){0});
        exit(1);
    }
}

int cmd_is_empty(const t_cmd *cmd)
{
    if (cmd == NULL) 
        return 1;
    if (cmd->argv == NULL) 
        return 1;
    if (cmd->argv[0] == NULL) 
        return 1;
    if (cmd->argv[0][0] == '\0') 
        return 1;
    return 0;
}

void exec_run_buildin(t_shell *sh, t_cmd *cmd)
{
    int st;
    if (cmd->builtin_kind != BUILTIN_NONE)
    {
        st = execute_builtin(sh, cmd);
        exit(st);
    }
}

void run_child_process(t_pipe *p, size_t i)
{
    t_cmd  *cmd;
    char    path[PATH_MAX];
    char  **envp;
    t_exec_status st;

    cmd = p->pl->cmds[i];
    child_setup(p, i);
    st = apply_redirections(cmd);
    if(st != EXEC_OK)
        exit (1);
    if (cmd_is_empty(cmd))
        exit(0);
    exec_run_buildin(p->sh, cmd);
    path[0] = '\0';
    if (cmd_path(p->sh, cmd->argv[0], path) == 0)
    {
        t_err_payload pay;
        pay.identifier = cmd->argv[0];
        err_print(ERR_EXEC, EXEC_CMD_NOT_FOUND, pay);
        exit(127);
    }
    envp = env_to_envp(p->sh->env_store);
    if (envp == NULL)
        exit(1);
    execve(path, cmd->argv, envp);
    if(errno == ENOEXEC)
        exec_handle_error2(path);
    else 
        exec_handle_error1(path);
}

t_exec_status exec_make_pipe(t_pipe *p)
{
    if(pipe(p->next) == -1)
    {
        err_print(ERR_EXEC, EXEC_ERR_PIPE, (t_err_payload){0});
        pipe_close(p);
        return EXEC_ERR_PIPE;
    }
    return EXEC_OK;
}
t_exec_status exec_fork_child(t_pipe *p, size_t i)
{
    p->pids[i] = fork();
    if(p->pids[i] == -1)
    {
        err_print(ERR_EXEC, EXEC_ERR_FORK, (t_err_payload){0});
        pipe_close(p);
        return EXEC_ERR_FORK;
    }
    if(p->pids[i] == 0)
        run_child_process(p, i);
    return EXEC_OK;
}

t_exec_status execution_run_pipeline(t_pipe *p)
{
    size_t i;

    i = 0;
    p->prev[FD_READ] = -1;
    p->prev[FD_WRITE] = -1;
    while(i < p->n)
    {
        p->next[FD_READ]  = -1;
        p->next[FD_WRITE] = -1;

        if (i + 1 < p->n)
        {
            if (exec_make_pipe(p) != EXEC_OK)
                return EXEC_ERR_PIPE;
        }
        if (exec_fork_child(p, i) != EXEC_OK)
            return EXEC_ERR_FORK;
        pipe_parent_rotate(p);
        i++;
    }
    pipe_close(p);
    return EXEC_OK;
}
t_exec_status execute_pipeline(t_shell *sh, t_pipeline *pl)
{
    t_pipe p;
    int last = 0;
    t_exec_status st;

    p.sh = sh;
    p.pl = pl;
    p.n = pl->count;
    p.pids = (pid_t *)malloc(sizeof(pid_t)* p.n);
    if(p.pids == NULL)
    {
        err_print(ERR_EXEC, EXEC_ALLOC_ERR, (t_err_payload){0});
        sh->last_status = 1;
        return EXEC_ALLOC_ERR;
    }
    st = execution_run_pipeline(&p);
    if(st == EXEC_OK)
    {
        if(wait_all(p.pids, p.n, &last) != EXEC_OK)
            last = 1;
    }
    else 
        last = 1;
    free(p.pids);
    sh->last_status = last;
    return st;
}
