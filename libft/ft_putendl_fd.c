/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavrukh <alavrukh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:36:56 by alavrukh          #+#    #+#             */
/*   Updated: 2025/05/02 14:57:04 by alavrukh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
// the same way like ft_putstr_fd, it just adds a newline character at the end

ssize_t	ft_putendl_fd(const char *s, int fd)
{
	ssize_t res;
	const size_t str_len = ft_strlen(s);

	res = write(fd, s, str_len);
	if (res < 0)
		return (-1);
	res = write(fd, "\n", 1);
	if (res < 0)
		return (-1);
	return (ssize_t)(str_len + 1);
}
