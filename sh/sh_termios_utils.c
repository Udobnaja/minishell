/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_termios_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: audobnai <audobnai@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 22:23:23 by audobnai          #+#    #+#             */
/*   Updated: 2025/10/27 22:23:24 by audobnai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

#if defined(ECHOCTL)

void	sh_echoctl_off(struct termios *t)
{
	t->c_lflag &= ~ECHOCTL;
}

void	sh_echoctl_on(struct termios *t)
{
	t->c_lflag |= ECHOCTL;
}
#else

void	sh_echoctl_off(struct termios *t)
{
	(void)t;
}

void	sh_echoctl_on(struct termios *t)
{
	(void)t;
}
#endif