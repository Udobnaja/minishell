/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavrukh <alavrukh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:10:46 by alavrukh          #+#    #+#             */
/*   Updated: 2025/05/02 16:26:15 by alavrukh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	const char	*result;

	result = NULL;
	while (*s)
	{
		if (*s == (unsigned char)c)
			result = s;
		s++;
	}
	if (c == '\0')
		return ((char *) s);
	return ((char *) result);
}
