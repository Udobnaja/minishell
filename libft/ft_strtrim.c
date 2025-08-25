/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavrukh <alavrukh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 18:13:53 by alavrukh          #+#    #+#             */
/*   Updated: 2025/05/02 16:26:18 by alavrukh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	trimcheck(char c, char const *set)
{
	while (*set)
	{
		if (*set == c)
			return (1);
		set++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*trimmed;
	size_t	start;
	size_t	end;

	if (!s1)
		return (ft_strdup(""));
	start = 0;
	end = ft_strlen(s1)-1;
	while (trimcheck(s1[start], set))
		start++;
	while (trimcheck(s1[end], set))
		end--;
	trimmed = ft_substr(s1 + start, 0, end - start + 1);
	if (!trimmed)
		return (NULL);
	return (trimmed);
}
