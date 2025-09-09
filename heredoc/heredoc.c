
#include "heredoc.h"

// TODO: status

static int hd_is_end_of_key(char c) {
    return (ft_isspace(c) || c == '\'' || c == '"');
}

// heredoc_write
// static char *heredoc_expand 

static ssize_t hd_write_until_key(char *str, int fd)
{
	size_t i;

	i = 0;
	while (str[i] && str[i] != '$')
		i++;
	return (write(fd, str, i));
}

static ssize_t hd_write_key(char *str, int fd)
{
	size_t i;
	char *key;
	ssize_t write_result;

	i = 0;
	while(str[i] && hd_is_end_of_key(str[i]))
		i++;
	key = malloc(i + 1);
	if (key)
		return (-1);
	ft_memcpy(key, str, i);
	key[i + 1] = '\0';
	// try to do expansion here

	// not forgot to free borrow thing
	write_result = write(fd, key, i); // i + 1. will be \0
	free(key);
	return (write_result);
}

void hd_read_write(char *eof, int fd, int has_expansion)
{
	char *line;

	while(1)
	{
		line = readline("> ");
		if (!line)
			break;
		if (ft_strcmp(line, eof) == 0)
		{
			free(line);
			line = NULL;
			return;
		}
		if (!has_expansion)
		{
			if (ft_putendl_fd(fd, line) < 0)
				return ; // TODO: throw error
		}
		else
		{
			size_t i;
			ssize_t write_result;

			i = 0;
			while (line[i])
			{	
				write_result = hd_write_until_key(line + i, fd);
				if (write_result < 0)
					return ; // TODO: throw error
				i += write_result;
				if (line[i] == '$')
				{
					write_result = hd_write_key(line + i, fd);
					if (write_result < 0)
						return ; // TODO: throw error
					i += write_result;
				}
			}
			if (write(fd, "\n", 1) < 0)
				return ; // TODO: throw error
		}		
		free(line);
		line = NULL;
	}
}