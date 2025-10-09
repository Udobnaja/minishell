#include "executor_internal.h"

t_exec_status redir_status_from_errno(int err)
{
    if(err == ENOENT || err == ENOTDIR)
        return EXEC_NO_SUCH_FILE;
    if(err == EISDIR)
        return EXEC_ERR_PERMISSION;
    if(err == EROFS)
        return EXEC_ERR_PERMISSION;
    if(err == EACCES || err == EPERM)
        return EXEC_ERR_PERMISSION;
    return EXEC_ERR_PERMISSION;
}

int select_target(const t_redirect *r)
{
    if(r->type == REDIR_IN)
        return STDIN_FILENO;
    if(r->type == REDIR_HEREDOC)
        return STDIN_FILENO;
    if(r->stream == IO_STDERR)
        return STDERR_FILENO;
    return STDOUT_FILENO;
}
t_exec_status dup2_end_close(int fd, int target, const char *name)
{
    int err;

    if(fd == target)
        return EXEC_OK;
    if(dup2(fd, target) < 0)
    {
        err = errno;
        close(fd);
        t_exec_status st = redir_status_from_errno(err);
        err_print(ERR_EXEC, st, (t_err_payload){.identifier = (char *)name}); //identifier? чтобы вывести какой файл выдал ошибку 
        return st;
    }
    close (fd);
    return EXEC_OK;
}

t_exec_status open_for_redirect(const t_redirect *r, int *out_fd)
{
    int fd;

    fd = -1;
    if(r->type == REDIR_IN)
        fd = open(r->target.path, O_RDONLY);
    else if (r->type == REDIR_OUT)
        fd = open(r->target.path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    else if (r->type == REDIR_APPEND)
        fd = open(r->target.path, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else if (r->type == REDIR_HEREDOC)
        fd = r->target.fd;
    *out_fd = fd;
    if (fd < 0)
        return redir_status_from_errno(errno);
    return EXEC_OK;
}

t_exec_status apply_one_redirect(const t_redirect *r)
{
    t_exec_status st;
    int fd;
    int target;

    if(r->type == REDIR_HEREDOC)
    {
        fd = r->target.fd;
        if(fd < 0)
        {
            err_print(ERR_EXEC, EXEC_ERR_PERMISSION, 
                (t_err_payload){.identifier = NULL});
                return EXEC_ERR_PERMISSION;
        }
        return dup2_end_close(fd, STDIN_FILENO, NULL);
    }
    st = open_for_redirect(r, &fd);
    if(st != EXEC_OK)
    {
        err_print(ERR_EXEC, st,
            (t_err_payload){ .identifier = r->target.path });
        return st;
    }
    target = select_target(r);
    return dup2_end_close(fd, target, r->target.path);

}

t_exec_status apply_redirections(t_cmd *cmd)
{
    t_redirect *r;
    t_exec_status st;

    if(cmd == NULL)
        return EXEC_OK;
    r = cmd->redirect_list;
    while(r != NULL)
    {
        st = apply_one_redirect(r);
        if(st != EXEC_OK)
            return st;
        r = r->next;
    }
    return EXEC_OK;
}
