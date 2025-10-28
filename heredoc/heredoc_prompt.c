/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_prompt.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: audobnai <audobnai@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 21:59:22 by audobnai          #+#    #+#             */
/*   Updated: 2025/10/28 17:47:10 by audobnai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heredoc_internal.h"

int	heredoc_read_prompt(char *line, t_heredoc_status *status, char *eof)
{
	size_t	str_len;

	if (!line)
		return (0);
	str_len = ft_strlen(line);
	if (str_len && line[str_len - 1] == '\n' && !isatty(STDIN_FILENO))
		line[str_len - 1] = '\0';
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
