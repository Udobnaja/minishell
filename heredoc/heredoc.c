#include "heredoc_internal.h"

static t_heredoc_result	heredoc_ok(int fd);
static t_heredoc_result	heredoc_err(int errno_val);
static t_heredoc_result	heredoc_write_err(t_heredoc_status status);
static t_heredoc_status heredoc_to_fd(char *eof, int fd, int has_expansion, t_shell *sh);

t_heredoc_result	heredoc_write_to_tmpfile(t_shell *sh, char *eof, int has_expansion)
{
	char				path[SH_TMPPATH_MAX];
	const int			fd = sh_mktmpfd(sh->env_store, path, "msh-thd");
	int					fd2;
	t_heredoc_status	status;
	t_heredoc_result	result;
	
	if (fd < 0)
		return (heredoc_err(errno));
	status = heredoc_to_fd(eof, fd, has_expansion, sh);
	if (status != HEREDOC_OK)
		return (close(fd), unlink(path), heredoc_write_err(status));
	fd2 = open(path, O_RDONLY | SH_O_CLOEXEC);
	if (fd2 < 0)
    {
      result = heredoc_err(errno);
      return (close(fd), unlink(path), result);
    }
	close(fd);
	if (unlink(path) < 0)
    {
      result = heredoc_err(errno); 
      return (close(fd2), result);
    }
	return (heredoc_ok(fd2));
}

static t_heredoc_status heredoc_to_fd(char *eof, int fd, int has_expansion, t_shell *sh)
{
	char *line;
	t_heredoc_status status;

	while(1)
	{
		line = readline("> ");
		if (!line)
			break;
		if (ft_strcmp(line, eof) == 0)
		{
			free(line);
			return (HEREDOC_OK);
		}
		status = heredoc_write_line(fd, line, has_expansion, sh);
		free(line);
		if (status != HEREDOC_OK)
			return (status);	
	}
	return (HEREDOC_OK);
}

static t_heredoc_result	heredoc_ok(int fd)
{
	 t_heredoc_result	result;

	result.status = HEREDOC_OK;
	result.errno_val = 0;
	result.fd = fd;
	return (result);
}

static t_heredoc_result	heredoc_write_err(t_heredoc_status status)
{
	if (status == HEREDOC_ALLOC_ERROR)
		return (heredoc_err(ENOMEM));
	else
		return (heredoc_err(EIO));
}

static t_heredoc_result	heredoc_err(int errno_val)
{
	t_heredoc_result	result;

	result.status = HEREDOC_ERRNO;
	result.errno_val = errno_val;
	result.fd = 0;
	return (result);
}