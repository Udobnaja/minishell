#include "executor_internal.h"

void close_pair(int p[2])
{
    if(p[FD_READ] != -1)
    {
        close(p[FD_READ]);
        p[FD_READ] = -1;
    }
    if(p[FD_WRITE] != -1)
    {
        close(p[FD_WRITE]);
        p[FD_WRITE] = -1;
    }
}

void close_fd(int *fd)
{
    if(*fd != -1)
    {
        close(*fd);
        *fd = -1;
    }
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

void run_child_process(t_pipe *p, size_t i)
{
    int st;
    char *path;
    char **envp;
    int code;

    if(child_process(p, i) != EXEC_OK)
    {
        err_print(ERR_EXEC, EXEC_ERR_DUP, (t_err_payload){0});
        exit(1);
    }
    if (is_builtin(p->pl->cmds[i]))
    {
        st = run_builtin(p->sh, p->pl->cmds[i]);                
        exit(st);
    }
    path = NULL;
    if(p->pl->cmds[i]->argv && p->pl->cmds[i]->argv[0])
    path = cmd_path(p->pl->cmds[i]->argv[0], p->sh->env_store, NULL);
    if (path == NULL)
    {
        t_err_payload pay;
        pay.identifier = p->pl->cmds[i]->argv[0];
        err_print(ERR_EXEC, EXEC_CMD_NOT_FOUND, pay);
        exit(127);
    }

    envp = env_to_envp(p->sh->env_store);
    execve(path, p->pl->cmds[i]->argv, envp);
    {
        t_err_payload pay;
        pay.identifier = path;
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
        else
        {
            err_print(ERR_EXEC, EXEC_ERR_EXECUTION, pay);
            code = 126;
        }
        exit(code);
    }
}

t_exec_status execution_run_pipeline(t_pipe *p)
{
    size_t i;
    p->prev[FD_READ] = -1;
    p->prev[FD_WRITE] = -1;
    
    i = 0;
    while(i < p->n)
    {
        p->next[FD_READ]  = -1;
        p->next[FD_WRITE] = -1;

        if (i + 1 < p->n)
        {
            if (pipe(p->next) == -1)
            {
                err_print(ERR_EXEC, EXEC_ERR_PIPE, (t_err_payload){0});
                pipe_close(p);
                return EXEC_ERR_PIPE;
            }
        }

        p->pids[i] = fork();
        if (p->pids[i] == -1)
        {
            err_print(ERR_EXEC, EXEC_ERR_FORK, (t_err_payload){0});
            pipe_close(p);
            return EXEC_ERR_FORK;
        }

        if (p->pids[i] == 0)
            run_child_process(p, i);

        pipe_parent_rotate(p);
        i++;
    }
    close_fd(&p->prev[FD_READ]);
    return EXEC_OK;
}

t_exec_status execute_pipeline(t_shell *sh, t_pipeline *pl)
{
    t_pipe         p;
    int            last;
    t_exec_status  st;

    if (!pl || pl->count == 0)
    {
        sh->last_status = 0;
        return EXEC_OK;
    }

    if (pl->count == 1 && is_builtin(pl->cmds[0]))
    {
        sh->last_status = run_builtin(sh, pl->cmds[0]);
        return EXEC_OK;
    }

    p.sh = sh;
    p.pl = pl;
    p.n = pl->count;
    p.pids = malloc(sizeof(pid_t) * p.n);
    if (!p.pids)
    {
        err_print(ERR_EXEC, EXEC_ALLOC_ERR, (t_err_payload){0});
        sh->last_status = 1;
        return EXEC_ALLOC_ERR;
    }

    st = execution_run_pipeline(&p);
    if (st == EXEC_OK)
        wait_all(p.pids, p.n, &last);
    else
        last = 1;

    free(p.pids);
    sh->last_status = last;
    return st;
}
