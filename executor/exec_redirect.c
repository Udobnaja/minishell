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
t_exec_result exec_redirect_result(t_exec_status status, int exit_code)
{
    t_exec_result result;

    result = (t_exec_result){0};
	result.flow = FLOW_OK;
	result.exit_code = (int)(unsigned char)exit_code;
	result.status = status;
	result.errno_val = 0;
	return (result);
}

t_exec_result    exec_redirect_error(t_exec_status status, const char *name, int errno_val)
{
	t_exec_result	result;
	t_err_payload	payload;

    result = (t_exec_result){0};
    payload = (t_err_payload){0};
    result.flow = FLOW_OK;
	if (status == EXEC_ERR_GEN || status == EXEC_NO_SUCH_FILE || status == EXEC_ERR_PERMISSION)	
	result.status = status;
    result.exit_code = SH_GENERAL_ERROR;

	payload.command = name;
    result.errno_val = errno_val;
	err_print(ERR_EXEC, status, payload);
	return (result);
}

t_exec_result dup2_end_close(int fd, int target, const char *name)
{
    int error;
    t_exec_status status;

    if(fd == target)
        return exec_redirect_result(EXEC_OK, 0);
    if(dup2(fd, target) < 0)
    {
        error = errno;
        close(fd);
        status = redir_status_from_errno(error);
        return exec_redirect_error(status, name, error);
    }
    close (fd);
    return exec_redirect_result(EXEC_OK, 0);
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

t_exec_result apply_one_redirect(const t_redirect *r)
{
    t_exec_status status;
    int fd;
    int target;
    int error;

    if(r->type == REDIR_HEREDOC)
        return dup2_end_close(r->target.fd, STDIN_FILENO, NULL);
    status = open_for_redirect(r, &fd);
    if(status != EXEC_OK)
    {
        error = errno;
        return exec_redirect_error(EXEC_ERR_GEN, r->target.path, error);
    }
    target = select_target(r);
    return dup2_end_close(fd, target, r->target.path);

}

t_exec_result apply_redirections(t_cmd *cmd)
{
    t_redirect *r;
    t_exec_result result;

    if(cmd == NULL)
        return exec_redirect_result(EXEC_OK, 0);
    r = cmd->redirect_list;
    while(r != NULL)
    {
        result = apply_one_redirect(r);
        if(result.status != EXEC_OK)
            return result;
        r = r->next;
    }
    return exec_redirect_result(EXEC_OK, 0);
}
