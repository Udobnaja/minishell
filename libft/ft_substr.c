/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavrukh <alavrukh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 13:04:29 by alavrukh          #+#    #+#             */
/*   Updated: 2025/05/02 16:26:22 by alavrukh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	substr_len;
	size_t	s_len;
	char	*alloc_memory;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len)
		return (ft_strdup(""));
	substr_len = ft_strlen(s + start);
	if (substr_len > len)
		substr_len = len;
	alloc_memory = malloc((substr_len + 1) * sizeof(char));
	if (!alloc_memory)
		return (NULL);
	ft_strlcpy(alloc_memory, s + start, substr_len + 1);
	return (alloc_memory);
}
