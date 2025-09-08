
#include "heredoc.h"

// TODO: status

static int hd_is_end_of_key(char c) {
    return (c == '\0' || c == ' ' || c == '\'' || c == '"');
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

			i = 0;
			while (line[i])
			{
				// Go step by step until  '$' or '\0' (1)
				// Then try to expand the substring starting from '$' up to one of: '\0', '"', '\'', or space
				// Repeat: write symbol by symbol and append
				// while (line[i] && line[i] != '$')
				// 	i++;
				// write(fd, line, i);
				

			}
			
			
		}		
		free(line);
		line = NULL;
	}
}