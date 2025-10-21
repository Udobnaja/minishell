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

t_exec_result child_process(t_pipe *p, size_t i)
{
    const char *cmd = "dup2";
    t_exec_result result;

    if (i > 0)
    {
        if(p->prev[FD_READ] == -1)
        {
            pipe_close(p);
            return (
                exec_external_sys_error(EXEC_ERR_DUP, cmd, 0)
            );
        }
        if(dup2(p->prev[FD_READ], STDIN_FILENO) == -1)
        {
            result = exec_external_sys_error(EXEC_ERR_GEN, cmd, errno);
            pipe_close(p);
            return result;
        }
    }
    if (i + 1 < p->n)
    {
        if(p->next[FD_WRITE] == -1)
        {
            pipe_close(p);
            return (
                exec_external_sys_error(EXEC_ERR_DUP, cmd, 0)
            );
        }
        if(dup2(p->next[FD_WRITE], STDOUT_FILENO) == -1)
        {
            result = exec_external_sys_error(EXEC_ERR_GEN, cmd, errno);
            pipe_close(p);
            return result;
        }
    }
    pipe_close(p);
    return exec_external_result(EXEC_OK, SH_OK);
}

t_exec_result wait_all(pid_t *pids, size_t n)
{
    size_t i;
    int last;
    int st;
    pid_t  w_pid;

    last = 0;
    i = 0;
    while(i < n)
    {
        if(pids[i] != 0)
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
            last = sh_status_from_wait(st);
        }
        i++;
    }
    return exec_external_result(EXEC_OK, last);
}

static void child_setup(t_pipe *p, size_t i)
{
    t_exec_result result;

    result = child_process(p, i);
    if (result.status != EXEC_OK)
        exit(result.exit_code);
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

static void pipeline_free_redirects(t_redirect *redirect)
{
	t_redirect *next;

	while (redirect)
	{
		next = redirect->next;
		if (redirect->type == REDIR_HEREDOC)
		{
            if (redirect->target.fd >= 0)
                close(redirect->target.fd);
		}
		else if ((redirect->type == REDIR_IN || redirect->type == REDIR_OUT
			|| redirect->type == REDIR_APPEND) && redirect->target.path)
			free(redirect->target.path);
		free(redirect);
		redirect = next;
	}
}

static void	pipeline_argv(char **argv)
{
	size_t	i;

	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}	
	free(argv);
}

static void	pipeline_free_cmd(t_cmd *cmd)
{
	free(cmd->name);
	if (cmd->argv)
		pipeline_argv(cmd->argv);
	if (cmd->redirect_list)
		pipeline_free_redirects(cmd->redirect_list);
	free(cmd);
}
void	pipeline_ddestroy(t_pipeline *pipeline)
{
	size_t	i;

	if (!pipeline || !pipeline->cmds)
		return;
	i = 0;
	while (i < pipeline->count)
	{
		if (pipeline->cmds[i])
			pipeline_free_cmd(pipeline->cmds[i]);
		i++;
	}
	free(pipeline->cmds);
	pipeline->cmds = NULL;
	pipeline->count = 0;
}

void exec_run_buildin(t_shell *sh, t_cmd *cmd, pid_t *pids,  t_pipeline *pl)
{
    int exit_code;
    if (cmd->builtin_kind != BUILTIN_NONE)
    {
        exit_code = execute_builtin(sh, cmd).exit_code;
        if (sh->env_store)
            env_destroy(&sh->env_store);
        if (sh->heredoc_store)
		    heredoc_store_destroy(&sh->heredoc_store);
        pipeline_ddestroy(pl);
        free(pids);
        exit(exit_code);
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
         if (p->sh->env_store)
            env_destroy(&p->sh->env_store);
        if (p->sh->heredoc_store)
		    heredoc_store_destroy(&p->sh->heredoc_store);
        pipeline_ddestroy(p->pl);
        free(p->pids);
        exit(result.exit_code);
    }
    if (cmd_is_empty(cmd))
    {
        result = exec_external_result(EXEC_OK, SH_OK);
        if (p->sh->env_store)
            env_destroy(&p->sh->env_store);
        if (p->sh->heredoc_store)
		    heredoc_store_destroy(&p->sh->heredoc_store);
        pipeline_ddestroy(p->pl);
        free(p->pids);
        exit(result.exit_code);
    }
    exec_run_buildin(p->sh, cmd, p->pids, p->pl);
    path[0] = '\0';
    if (cmd_path(p->sh, cmd->argv[0], path) == 0)
    {
        if (ft_strchr(cmd->argv[0], '/') == NULL)
		    result = exec_external_error_result(
					EXEC_CMD_NOT_FOUND, cmd->argv[0], 0);
		else
		    result = exec_external_error_result(
					EXEC_NO_SUCH_FILE, cmd->argv[0], 0);
        exit(result.exit_code);
    }
    exec_child(path, cmd, p->sh);
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

    p.sh = sh;
    p.pl = pl;
    p.n = pl->count;
    p.pids = malloc(sizeof(pid_t)* p.n);
    if(p.pids == NULL)
        return exec_external_sys_error(EXEC_ALLOC_ERR, NULL, 1);
    result = execution_run_pipeline(&p);
    if(result.status == EXEC_OK)
        result = wait_all(p.pids, p.n);
   free(p.pids);
    // if (p.sh->env_store)
    //     env_destroy(&p.sh->env_store);
    return (result);
}
