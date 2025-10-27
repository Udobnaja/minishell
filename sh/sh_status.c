/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_status.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: audobnai <audobnai@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 22:23:18 by audobnai          #+#    #+#             */
/*   Updated: 2025/10/27 22:23:19 by audobnai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	sh_status_from_signal(int sig)
{
	return (128 + sig);
}

int	sh_status_from_errno_exec(int err)
{
	if (err == ENOENT)
		return (SH_NOT_FOUND);
	if (err == EACCES || err == EISDIR || err == ENOEXEC)
		return (SH_NOT_EXECUTABLE);
	return (SH_NOT_EXECUTABLE);
}

int	sh_status_from_wait(int wst)
{
	if (WIFEXITED(wst))
		return (WEXITSTATUS(wst));
	if (WIFSIGNALED(wst))
		return (sh_status_from_signal(WTERMSIG(wst)));
	return (SH_GENERAL_ERROR);
}
