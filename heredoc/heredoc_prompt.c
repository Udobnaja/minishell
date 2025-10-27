#include "heredoc_internal.h"

int	heredoc_read_prompt(char *line, t_heredoc_status *status, char *eof)
{
	if (!line)
		return (0);
	if (g_last_signal == SIGINT)
	{
		if (line)
			free(line);
		*status = HEREDOC_ABORTED;
		g_last_signal = 0;
		return (0);
	}
	if (ft_strcmp(line, eof) == 0)
	{
		free(line);
		return (0);
	}
	return (1);
}