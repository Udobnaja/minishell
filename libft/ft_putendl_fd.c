/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timlive <timlive@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:36:56 by alavrukh          #+#    #+#             */
/*   Updated: 2025/10/22 16:01:42 by timlive          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// the same way like ft_putstr_fd, it just adds a newline character at the end

ssize_t	ft_putendl_fd(const char *s, int fd)
{
	ssize_t			res;
	const size_t	str_len = ft_strlen(s);

	res = write(fd, s, str_len);
	if (res < 0)
		return (-1);
	res = write(fd, "\n", 1);
	if (res < 0)
		return (-1);
	return ((ssize_t)(str_len + 1));
}
