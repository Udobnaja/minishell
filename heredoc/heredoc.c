
#include "heredoc.h"

static int heredoc_end_of_key(char c) {
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
	if (nw != len)
	{
		if (e == 0)
			e = ENOSPC;
		return (e);
	}
	return (0);
}

static t_heredoc_status heredoc_write_until_expansion(char *str, int fd, size_t *consumed)
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

char *heredoc_create_env_key(char *str)
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

t_heredoc_status heredoc_write_expansion(char *str, int fd, t_shell *sh, size_t *consumed)
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

t_heredoc_status heredoc_expand(char *document, int fd, t_shell *sh)
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
		if (!has_expansion)
		{
			if (heredoc_write(fd, line, ft_strlen(line)) != 0)
			{
				free(line);
				return (HEREDOC_WRITE_ERROR);
			}
			if (heredoc_write(fd, "\n", 1) != 0)
			{
				free(line);
				return (HEREDOC_WRITE_ERROR);
			}	
		}
		else
		{
			status = heredoc_expand(line, fd, sh);
			if (status != HEREDOC_OK)
			{
				free(line);
				return (status);
			}
		}		
		free(line);
		line = NULL;
	}
	return (HEREDOC_OK);
}