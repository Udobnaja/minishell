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
    t_err_payload payload;

    if(fd == target)
        return EXEC_OK;
    if(dup2(fd, target) < 0)
    {
        close(fd);
        payload.command = name;
        payload.errno_val = errno;

        err_print(ERR_EXEC, EXEC_ERR_GEN, payload);
        return (EXEC_ERR_GEN);
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
    if (fd < 0)
        return redir_status_from_errno(errno);
    *out_fd = fd;
    return EXEC_OK;
}

t_exec_status apply_one_redirect(const t_redirect *r)
{
    t_exec_status status;
    int fd;
    int target;

    if(r->type == REDIR_HEREDOC)
        return dup2_end_close(r->target.fd, STDIN_FILENO, NULL);
    status = open_for_redirect(r, &fd);
    if(status != EXEC_OK)
    { // тут печать либо выше либо возарщать еррно и печатать гену
        err_print(ERR_EXEC, status,
            (t_err_payload){ .command = r->target.path });
        return status;
    }
    target = select_target(r);
    return dup2_end_close(fd, target, r->target.path);

}

t_exec_status apply_redirections(t_cmd *cmd)
{
    t_redirect *r;
    t_exec_status status;

    if(cmd == NULL)
        return EXEC_OK;
    r = cmd->redirect_list;
    while(r != NULL)
    {
        status = apply_one_redirect(r);
        if(status != EXEC_OK)
            return status;
        r = r->next;
    }
    return EXEC_OK;
}
