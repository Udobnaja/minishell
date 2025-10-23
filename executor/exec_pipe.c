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


t_exec_result exec_pipe_apply_stdio(t_pipe *p, int std_in, int std_out)
{
    t_exec_result result;
    const char *cmd = "dup2";

    if(std_in == -1)
    {
        pipe_close(p);
        return (
            exec_external_sys_error(EXEC_ERR_GEN, cmd, EBADF));
    }
    if(dup2(std_in, std_out) == -1)
    {
        result = exec_external_sys_error(EXEC_ERR_GEN, cmd, errno);
        pipe_close(p);
        return result;
    }
    return exec_external_result(EXEC_OK, SH_OK);
}

t_exec_result child_process(t_pipe *p, size_t i)
{
    t_exec_result result;

    if (i > 0)
    {
        result = exec_pipe_apply_stdio(p, p->prev[FD_READ], STDIN_FILENO);
        if (result.status != EXEC_OK)
            return (result);
    }
    if (i + 1 < p->n)
    {
        result = exec_pipe_apply_stdio(p, p->next[FD_WRITE], STDOUT_FILENO);
        if (result.status != EXEC_OK)
            return (result);
    }
    pipe_close(p);
    return exec_external_result(EXEC_OK, SH_OK);
}

static pid_t find_last_pid(const pid_t *pids, size_t n)
{
    size_t i;

    i = n;
    while (i > 0)
    {
        i--;
        if (pids[i] > 0)
            return (pids[i]);
    }
    return (0);
}

t_exec_result wait_all(pid_t *pids, size_t n)
{
    size_t i;
    int last_code;
    int st;
    pid_t  w_pid;
    const pid_t last_pid = find_last_pid(pids, n);

    last_code = 0;
    i = 0;
    while(i < n)
    {
        if(pids[i] > 0)
        {
            st = 0;
            while (1)
            {
                w_pid = waitpid(pids[i], &st, 0);
                if (w_pid == -1 && errno == EINTR)
                    continue; 
                break;
            }
            if (w_pid == -1)
                return exec_external_sys_error(EXEC_ERR_GEN, "waitpid", errno);
            if (pids[i] == last_pid)
            {
                last_code = sh_status_from_wait(st);
                sh_report_wait_signal(st);
            }
        }
        i++;
    }
    return exec_external_result(EXEC_OK, last_code);
}

static void child_setup(t_pipe *p, size_t i)
{
    t_exec_result result;

    result = child_process(p, i);
    if (result.status != EXEC_OK)
    {
        exec_child_process_clean(p->sh, p->pids, p->pl);
        exit(result.exit_code);
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

void exec_run_buildin(t_shell *sh, t_cmd *cmd, pid_t *pids,  t_pipeline *pl)
{
    int exit_code;
    if (cmd->builtin_kind != BUILTIN_NONE)
    {
        exit_code = execute_builtin(sh, cmd).exit_code;
        exec_child_process_clean(sh, pids, pl);
        exit(exit_code);
    }
}
void exec_cmd_path(t_pipe *p, t_cmd *cmd, char path[PATH_MAX])
{
    t_exec_result result;

    if (cmd_path(p->sh, cmd->argv[0], path) == 0)
    {
        if (ft_strchr(cmd->argv[0], '/') == NULL)
		    result = exec_external_error_result(
					EXEC_CMD_NOT_FOUND, cmd->argv[0], 0);
		else
		    result = exec_external_error_result(
					EXEC_NO_SUCH_FILE, cmd->argv[0], 0);
        exec_child_process_clean(p->sh, p->pids, p->pl);       
        exit(result.exit_code);
    }
}

void run_child_process(t_pipe *p, size_t i)
{
    t_cmd  *cmd;
    char    path[PATH_MAX];
    t_exec_result result;

    sh_setup_rl_hook(SH_CHILD);
    cmd = p->pl->cmds[i];
    child_setup(p, i);
    result = apply_redirections(cmd);
    if(result.status != EXEC_OK)
    {
        result = exec_external_result(result.status, SH_GENERAL_ERROR);
        exec_child_process_clean(p->sh, p->pids, p->pl);
        exit(result.exit_code);
    }
    if (cmd_is_empty(cmd))
    {
        result = exec_external_result(EXEC_OK, SH_OK);
        exec_child_process_clean(p->sh, p->pids, p->pl);
        exit(result.exit_code);
    }
    exec_run_buildin(p->sh, cmd, p->pids, p->pl);
    path[0] = '\0';
    exec_cmd_path(p, cmd, path);
    exec_child(path, cmd, p->sh, p);
}

t_exec_result exec_make_pipe(t_pipe *p)
{
    t_exec_result result;

    if(pipe(p->next) == -1)
    {
        result = exec_external_sys_error(EXEC_ERR_GEN, "pipe", errno); 
        pipe_close(p);
        return result;
    }
    return exec_external_result(EXEC_OK, SH_OK);
}
t_exec_result exec_fork_child(t_pipe *p, size_t i)
{
    t_exec_result result;

    p->pids[i] = fork();
    if(p->pids[i] == -1)
    {
        result = exec_external_sys_error(EXEC_ERR_GEN, "fork", errno); 
        pipe_close(p);
        return result;
    }
    if(p->pids[i] == 0)
        run_child_process(p, i);
    return exec_external_result(EXEC_OK, SH_OK);
}

t_exec_result execution_run_pipeline(t_pipe *p)
{
    size_t i;
    t_exec_result result_fork;
    t_exec_result result_pipe;

    i = 0; 
    p->prev[FD_READ] = -1;
    p->prev[FD_WRITE] = -1;
    p->next[FD_READ]  = -1;
    p->next[FD_WRITE] = -1;
    while(i < p->n)
    {
        if (i + 1 < p->n)
        {
            result_pipe = exec_make_pipe(p);
            if (result_pipe.status != EXEC_OK)
                return result_pipe;
        }
        result_fork = exec_fork_child(p, i);
        if (result_fork.status != EXEC_OK)
            return result_fork;
        pipe_parent_rotate(p);
        i++;
    }
    pipe_close(p);
    return exec_external_result(EXEC_OK, SH_OK);
}

t_exec_result execute_pipeline(t_shell *sh, t_pipeline *pl)
{
    t_pipe p;
    t_exec_result result;
    t_exec_result w_result;

    p.sh = sh;
    p.pl = pl;
    p.n = pl->count;
    p.pids = ft_calloc(p.n, sizeof(pid_t));
    if(p.pids == NULL)
        return exec_external_sys_error(EXEC_ALLOC_ERR, NULL, 1);
    result = execution_run_pipeline(&p);
    w_result = wait_all(p.pids, p.n);
    free(p.pids);
    if(result.status != EXEC_OK)
        return (result);
    return (w_result);
}
