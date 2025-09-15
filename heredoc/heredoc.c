
#include "heredoc.h"

static int heredoc_end_of_key(const char c) {
    if (ft_isspace(c) || c == '\'' || c == '"' || c == '$')
        return (1);
    return !(ft_isalnum(c) || c == '_');
}

static int heredoc_write(int fd, const char *buf, size_t len)
{
	ssize_t	nw;
	int e;
	errno = 0;

	nw = write(fd, buf, len);
	e = errno;
	if (nw != (ssize_t)len)
	{
		if (e == 0)
			e = ENOSPC;
		return (e);
	}
	return (0);
}

static t_heredoc_status heredoc_write_until_expansion(const char *str, int fd, size_t *consumed)
{
	size_t i;
	int e;

	i = 0;
	while (str[i] && str[i] != '$')
		i++;
	e = heredoc_write(fd, str, i);
	if (e)
		return HEREDOC_WRITE_ERROR;
	*consumed += i;	
	return (HEREDOC_OK);
}

char *heredoc_create_env_key(const char *str)
{
	size_t	i;
	char	*key;

	i = 1;
	if (str[i] == '?')
		i++;
	else if (str[i] && (ft_isalpha(str[i]) || str[i] == '_'))
	{
		i++;
		while(str[i] && !heredoc_end_of_key(str[i]))
			i++;
	}	
	key = malloc(i + 1);
	if (!key)
		return NULL;
	ft_memcpy(key, str, i);
	key[i] = '\0';
	return (key);
}

static t_heredoc_status heredoc_write_expansion(const char *str, int fd, t_shell *sh, size_t *consumed)
{
	char	*key;
	char	*expanded;
	int		e;

	key = heredoc_create_env_key(str);
	if (!key)
		return (HEREDOC_ALLOC_ERROR);	
	expanded = expn_expand(key, sh->env_store, sh->last_status);
	if (!expanded)
	{
		free(key);
		return (HEREDOC_ALLOC_ERROR);
	}
	e = heredoc_write(fd, expanded, ft_strlen(expanded));
	if (e)
	{
		free(key);
		free(expanded);
		return (HEREDOC_WRITE_ERROR);
	}	
	*consumed += (ft_strlen(key));	
	free(key);
	free(expanded);
	return (HEREDOC_OK);
}

static t_heredoc_status heredoc_write_line_expanded(int fd, const char *document, t_shell *sh)
{
	size_t i;
	t_heredoc_status status;

	i = 0;
	while (document[i])
	{	
		status = heredoc_write_until_expansion(document + i, fd, &i);
		if (status != HEREDOC_OK)
				return (status);
		if (document[i] == '$')
		{
			status = heredoc_write_expansion(document + i, fd, sh, &i);
			if (status != HEREDOC_OK)
				return (status);
		}
	}
	if (heredoc_write(fd, "\n", 1) != 0)
		return (HEREDOC_WRITE_ERROR);
	return (HEREDOC_OK);	
}

static t_heredoc_status heredoc_write_line_raw(int fd, const char *document)
{
	if (heredoc_write(fd, document, ft_strlen(document)) != 0)
		return (HEREDOC_WRITE_ERROR);
	if (heredoc_write(fd, "\n", 1) != 0)
		return (HEREDOC_WRITE_ERROR);
	return (HEREDOC_OK);
}

t_heredoc_status heredoc_write_line(int fd, const char *document, int has_expansion, t_shell *sh)
{

	if (!has_expansion)
		return (heredoc_write_line_raw(fd, document));
	else
		return (heredoc_write_line_expanded(fd, document, sh));
}


t_heredoc_status heredoc_to_fd(char *eof, int fd, int has_expansion, t_shell *sh)
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

int	heredoc_write_to_tmpfile(t_shell *sh, char *eof, int has_expansion)
{

	int					fd;
	int					fd2;
	char				path[SH_TMPPATH_MAX];
	t_heredoc_status	status;
	int					r;
	
	fd = sh_mktmpfd(sh->env_store, path, "msh-thd");
	if (fd < 0)
		return (-1);
	status = heredoc_to_fd(eof, fd, has_expansion, sh);
	if (status != HEREDOC_OK)
	{
		close(fd);
		if (status == HEREDOC_ALLOC_ERROR)
			errno = ENOMEM;
		else
			errno = EIO;
		unlink(path);
		return (-1);
	}	
	fd2 = open(path, O_RDONLY | SH_O_CLOEXEC);
	if (fd2 < 0)
    {
      r = errno;
      unlink(path);
      close (fd);
      errno = r;
      return (-1);
    }
	close(fd);
	if (unlink (path) < 0)
    {
      r = errno;
      close (fd2);
      errno = r;
      return (-1);
    }

	return (fd2);
}