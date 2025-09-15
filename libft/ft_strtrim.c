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

static int	ft_shouldtrim(char c, const char *set)
{
	while (*set)
	{
		if (c == *set)
			return (1);
		set++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	left;
	size_t	right;
	size_t	alloc_size;
	char	*alloc_memory;

	if (!s1 || !set)
		return (NULL);
	left = 0;
	while (s1[left] && ft_shouldtrim(s1[left], set))
		left++;
	right = ft_strlen(s1);
	while (right > left && ft_shouldtrim(s1[right - 1], set))
		right--;
	alloc_size = (right - left) + 1;
	alloc_memory = malloc(alloc_size * sizeof(char));
	if (!alloc_memory)
		return (NULL);
	ft_strlcpy(alloc_memory, s1 + left, alloc_size);
	return (alloc_memory);
}
