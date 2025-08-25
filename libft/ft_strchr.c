/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavrukh <alavrukh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:03:04 by alavrukh          #+#    #+#             */
/*   Updated: 2025/05/02 14:57:24 by alavrukh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	unsigned char		chr;
	unsigned const char	*str;

	chr = (unsigned char)c;
	str = (unsigned const char *)s;
	while (*str)
	{
		if (*str == chr)
			return ((char *)str);
		str++;
	}
	if (*str == chr)
		return ((char *)str);
	return (NULL);
}
