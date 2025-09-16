
#include "heredoc_internal.h"

static t_heredoc_status heredoc_write_line_expanded(int fd, const char *document, t_shell *sh);
static t_heredoc_status heredoc_write_line_raw(int fd, const char *document);
static t_heredoc_status heredoc_write_expansion(const char *str, int fd, t_shell *sh, size_t *consumed);
static t_heredoc_status heredoc_write_until_expansion(const char *str, int fd, size_t *consumed);

t_heredoc_status heredoc_write_line(int fd, const char *document, int has_expansion, t_shell *sh)
{

	if (!has_expansion)
		return (heredoc_write_line_raw(fd, document));
	else
		return (heredoc_write_line_expanded(fd, document, sh));
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