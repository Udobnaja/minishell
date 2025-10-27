/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_noninteractive.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: audobnai <audobnai@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 21:44:44 by audobnai          #+#    #+#             */
/*   Updated: 2025/10/27 21:44:45 by audobnai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_internal.h"

void	msh_run_noninteractive(t_shell *sh)
{
	char	*line;

	while (1)
	{
		line = readline(NULL);
		if (!line)
			break ;
		if (msh_signal_check(line, sh))
			continue ;
		if (*line == '\0')
		{
			free(line);
			continue ;
		}
		if (msh_has_only_spaces(line))
		{
			free(line);
			continue ;
		}
		msh_run_pipeline(line, sh, 0);
	}
	msh_clean_and_exit(sh, sh->last_status, 0);
}
