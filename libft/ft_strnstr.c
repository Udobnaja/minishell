/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavrukh <alavrukh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:13:31 by alavrukh          #+#    #+#             */
/*   Updated: 2025/05/02 16:26:12 by alavrukh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	const unsigned char	*to_find;
	const unsigned char	*str;
	size_t				i;
	size_t				j;

	to_find = (const unsigned char *)little;
	str = (const unsigned char *)big;
	i = 0;
	if (!*to_find)
		return ((char *)str);
	while (str[i])
	{
		j = 0;
		while (str[i + j] == to_find[j] && i + j < len)
		{
			if (to_find[j + 1] == '\0')
			{
				return ((char *)&str[i]);
			}
			j++;
		}
		i++;
	}
	return (NULL);
}
