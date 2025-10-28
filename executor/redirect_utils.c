/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: audobnai <audobnai@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 22:52:08 by alavrukh          #+#    #+#             */
/*   Updated: 2025/10/28 20:46:14 by audobnai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor_internal.h"

t_exec_result	dup2_end_close(int fd, int target, const char *name)
{
	int				error;
	t_exec_status	status;

	if (fd == target)
		return (exec_external_result(EXEC_OK, SH_OK));
	if (dup2(fd, target) < 0)
	{
		error = errno;
		close(fd);
		status = redir_status_from_errno(error);
		return (exec_external_sys_error(status, name, error));
	}
	close(fd);
	return (exec_external_result(EXEC_OK, SH_OK));
}

void	exec_pl_close(t_pipeline *pipeline)
{
	size_t	i;

	if (!pipeline || !pipeline->cmds)
		return ;
	i = 0;
	while (i < pipeline->count)
	{
		if (pipeline->cmds[i] && pipeline->cmds[i]->redirect_list)
		{
			pipeline_free_redirects(pipeline->cmds[i]->redirect_list);
			pipeline->cmds[i]->redirect_list = NULL;
		}
		i++;
	}
}
