/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_getcwd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: audobnai <audobnai@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 22:34:24 by audobnai          #+#    #+#             */
/*   Updated: 2025/10/27 22:34:25 by audobnai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

const char	*u_getcwd(void)
{
	static char	buf[PATH_MAX];

	if (!getcwd(buf, sizeof buf))
		return (NULL);
	return (buf);
}
